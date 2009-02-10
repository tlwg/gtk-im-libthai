/* GTK - The GIMP Toolkit
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 *
 */

#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gdk/gdkkeys.h>
#include "gtkimcontextthai.h"

#include <thai/thcell.h>
#include <thai/thinp.h>

static void     gtk_im_context_libthai_class_init      (GtkIMContextLibThaiClass *class);
static void     gtk_im_context_libthai_init            (GtkIMContextLibThai      *im_context_libthai);
static gboolean gtk_im_context_libthai_filter_keypress (GtkIMContext             *context,
						        GdkEventKey              *key);

#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
static void     forget_previous_chars (GtkIMContextLibThai *context_libthai);
static void     remember_previous_char (GtkIMContextLibThai *context_libthai,
                                        tischar_t new_char);
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */

static GObjectClass *parent_class;

GType gtk_type_im_context_libthai = 0;

void
gtk_im_context_libthai_register_type (GTypeModule *type_module)
{
  static const GTypeInfo im_context_libthai_info =
  {
    sizeof (GtkIMContextLibThaiClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) gtk_im_context_libthai_class_init,
    NULL,           /* class_finalize */    
    NULL,           /* class_data */
    sizeof (GtkIMContextLibThai),
    0,
    (GInstanceInitFunc) gtk_im_context_libthai_init,
  };

  gtk_type_im_context_libthai = 
    g_type_module_register_type (type_module,
                                 GTK_TYPE_IM_CONTEXT,
                                 "GtkIMContextLibThai",
                                 &im_context_libthai_info, 0);
}

static void
gtk_im_context_libthai_class_init (GtkIMContextLibThaiClass *class)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS (class);

  parent_class = g_type_class_peek_parent (class);

  im_context_class->filter_keypress = gtk_im_context_libthai_filter_keypress;
}

static void
gtk_im_context_libthai_init (GtkIMContextLibThai *im_context_libthai)
{
#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
  forget_previous_chars (im_context_libthai);
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */
  im_context_libthai->isc_mode = ISC_BASICCHECK;
}

GtkIMContext *
gtk_im_context_libthai_new (void)
{
  GtkIMContextLibThai *result;

  result = GTK_IM_CONTEXT_LIBTHAI (g_object_new (GTK_TYPE_IM_CONTEXT_LIBTHAI, NULL));

  return GTK_IM_CONTEXT (result);
}

GtkIMContextLibThaiISCMode
gtk_im_context_libthai_get_isc_mode (GtkIMContextLibThai *context_libthai)
{
  return context_libthai->isc_mode;
}

GtkIMContextLibThaiISCMode
gtk_im_context_libthai_set_isc_mode (GtkIMContextLibThai *context_libthai,
                                     GtkIMContextLibThaiISCMode mode)
{
  GtkIMContextLibThaiISCMode prev_mode = context_libthai->isc_mode;
  context_libthai->isc_mode = mode;
  return prev_mode;
}

static gboolean
is_context_lost_key (guint keyval)
{
  return ((keyval & 0xFF00) == 0xFF00) &&
         (keyval == GDK_BackSpace ||
          keyval == GDK_Tab ||
          keyval == GDK_Linefeed ||
          keyval == GDK_Clear ||
          keyval == GDK_Return ||
          keyval == GDK_Pause ||
          keyval == GDK_Scroll_Lock ||
          keyval == GDK_Sys_Req ||
          keyval == GDK_Escape ||
          keyval == GDK_Delete ||
          (GDK_Home <= keyval && keyval <= GDK_Begin) || /* IsCursorkey */
          (GDK_KP_Space <= keyval && keyval <= GDK_KP_Delete) || /* IsKeypadKey, non-chars only */
          (GDK_Select <= keyval && keyval <= GDK_Break) || /* IsMiscFunctionKey */
          (GDK_F1 <= keyval && keyval <= GDK_F35)); /* IsFunctionKey */
}

static gboolean
is_context_intact_key (guint keyval)
{
  return (((keyval & 0xFF00) == 0xFF00) &&
           ((GDK_Shift_L <= keyval && keyval <= GDK_Hyper_R) || /* IsModifierKey */
            (keyval == GDK_Mode_switch) ||
            (keyval == GDK_Num_Lock))) ||
         (((keyval & 0xFE00) == 0xFE00) &&
          (GDK_ISO_Lock <= keyval && keyval <= GDK_ISO_Last_Group_Lock));
}

static tischar_t
mygdk_keyval_to_tis (guint keyval)
{
  gchar     key_utf8[6];
  int       len;
  tischar_t key_tis;

  key_tis = 0;

  len = g_unichar_to_utf8 (gdk_keyval_to_unicode (keyval), key_utf8);
  if (len > 0)
    {
      gchar *key_tis_str;
      key_tis_str = g_convert (key_utf8, len, "TIS-620", "UTF-8",
                               NULL, NULL, NULL);
      if (key_tis_str)
        {
          key_tis = key_tis_str[0];
          g_free (key_tis_str);
        }
    }

  return key_tis;
}

#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
static void
forget_previous_chars (GtkIMContextLibThai *context_libthai)
{
  memset (context_libthai->char_buff, 0, GTK_IM_CONTEXT_LIBTHAI_BUFF_SIZE);
  context_libthai->buff_tail = 0;
}

static void
remember_previous_char (GtkIMContextLibThai *context_libthai, tischar_t new_char)
{
  if (context_libthai->buff_tail == GTK_IM_CONTEXT_LIBTHAI_BUFF_SIZE)
    {
      memmove (context_libthai->char_buff, context_libthai->char_buff + 1,
               GTK_IM_CONTEXT_LIBTHAI_BUFF_SIZE - 1);
      --context_libthai->buff_tail;
    }
  context_libthai->char_buff[context_libthai->buff_tail++] = new_char;
}
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */

static struct thcell_t
get_previous_cell (GtkIMContextLibThai *context_libthai)
{
  gchar *surrounding;
  gint  cursor_index;
  struct thcell_t the_cell;

  th_init_cell (&the_cell);

  if (gtk_im_context_get_surrounding ((GtkIMContext *)context_libthai,
                                      &surrounding, &cursor_index))
    {
      gchar *s;
      gchar *tis_text = NULL;

      s = surrounding;
      while (*s)
        {
          gchar *t;

          tis_text = g_convert (s, cursor_index, "TIS-620", "UTF-8",
                                NULL, NULL, NULL);
          if (tis_text)
            break;

          t = g_utf8_next_char (s);
          cursor_index -= t - s;
          s = t;
        }
      if (tis_text)
        {
          int char_index;

          char_index = g_utf8_pointer_to_offset (s, s + cursor_index);
          th_prev_cell ((thchar_t *) tis_text, char_index, &the_cell, TRUE);
          g_free (tis_text);
        }
#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
      else
        {
          th_prev_cell (context_libthai->char_buff, context_libthai->buff_tail,
                        &the_cell, TRUE);
        }
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */
      g_free (surrounding);
    }
#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
  else
    {
      th_prev_cell (context_libthai->char_buff, context_libthai->buff_tail,
                    &the_cell, TRUE);
    }
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */

    return the_cell;
}

static gboolean
gtk_im_context_libthai_commit_chars (GtkIMContextLibThai *context_libthai,
                                     tischar_t *s, gsize len)
{
  gchar *utf8;

  utf8 = g_convert ((gchar *) s, len, "UTF-8", "TIS-620", NULL, NULL, NULL);
  if (!utf8)
    return FALSE;

  g_signal_emit_by_name (context_libthai, "commit", utf8);

  g_free (utf8);
  return TRUE;
}

static gboolean
gtk_im_context_libthai_filter_keypress (GtkIMContext *context,
                                        GdkEventKey  *event)
{
  GtkIMContextLibThai *context_libthai = GTK_IM_CONTEXT_LIBTHAI (context);
  struct thcell_t context_cell;
  struct thinpconv_t conv;
  tischar_t new_char;

  if (event->type != GDK_KEY_PRESS)
    return FALSE;

  if (event->state & (GDK_MODIFIER_MASK
                      & ~(GDK_SHIFT_MASK | GDK_LOCK_MASK | GDK_MOD2_MASK)) ||
      is_context_lost_key (event->keyval))
    {
#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
      forget_previous_chars (context_libthai);
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */
      return FALSE;
    }
  if (event->keyval == 0 || is_context_intact_key (event->keyval))
    {
      return FALSE;
    }

  context_cell = get_previous_cell (context_libthai);
  new_char = mygdk_keyval_to_tis (event->keyval);
  if (th_validate(context_cell, new_char, &conv))
    {
      if (conv.offset < 0)
        {
          if (!gtk_im_context_delete_surrounding (GTK_IM_CONTEXT (context_libthai),
                                                  conv.offset, -conv.offset))
            {
              return FALSE;
            }
        }
#ifndef GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK
      forget_previous_chars (context_libthai);
      remember_previous_char (context_libthai, new_char);
#endif /* !GTK_IM_CONTEXT_LIBTHAI_NO_FALLBACK */

      return gtk_im_context_libthai_commit_chars (context_libthai,
                                                  conv.conv,
                                                  strlen((char *) conv.conv));
    }
  else
    {
      /* reject character */
      gdk_beep ();
    }
  return TRUE;
}

