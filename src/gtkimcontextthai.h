/* GTK-IM-LibThai - Thai GTK+ Input Method based-on LibThai
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
 * Author:  Theppitak Karoonboonyanan <theppitak@gmail.com>
 *
 */

#ifndef __GTK_IM_CONTEXT_THAI_H__
#define __GTK_IM_CONTEXT_THAI_H__

#include <gtk/gtk.h>
#include <thai/thinp.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern GType gtk_type_im_context_libthai;

#define GTK_TYPE_IM_CONTEXT_LIBTHAI \
          gtk_type_im_context_libthai
#define GTK_IM_CONTEXT_LIBTHAI(obj) \
          (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                       GTK_TYPE_IM_CONTEXT_LIBTHAI, \
                                       GtkIMContextLibThai))
#define GTK_IM_CONTEXT_LIBTHAI_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_CAST ((klass), \
                                    GTK_TYPE_IM_CONTEXT_LIBTHAI, \
                                    GtkIMContextLibThaiClass))
#define GTK_IS_IM_CONTEXT_LIBTHAI(obj) \
          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                       GTK_TYPE_IM_CONTEXT_LIBTHAI))
#define GTK_IS_IM_CONTEXT_LIBTHAI_CLASS(klass) \
          (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                                    GTK_TYPE_IM_CONTEXT_LIBTHAI))
#define GTK_IM_CONTEXT_LIBTHAI_GET_CLASS(obj) \
          (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                      GTK_TYPE_IM_CONTEXT_LIBTHAI, \
                                      GtkIMContextLibThaiClass))


typedef guchar tischar_t;

typedef struct _GtkIMContextLibThai       GtkIMContextLibThai;
typedef struct _GtkIMContextLibThaiClass  GtkIMContextLibThaiClass;

/*
typedef enum
{
  ISC_PASSTHROUGH,
  ISC_BASICCHECK,
  ISC_STRICT
} GtkIMContextThaiISCMode;
*/
typedef thstrict_t GtkIMContextLibThaiISCMode;
#define GTK_IM_CONTEXT_LIBTHAI_BUFF_SIZE 4

void
gtk_im_context_libthai_register_type (GTypeModule *type_module);

GtkIMContext *
gtk_im_context_libthai_new (void);

GtkIMContextLibThaiISCMode
gtk_im_context_libthai_get_isc_mode (GtkIMContextLibThai *context_libthai);

GtkIMContextLibThaiISCMode
gtk_im_context_libthai_set_isc_mode (GtkIMContextLibThai *context_libthai,
                                     GtkIMContextLibThaiISCMode mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_IM_CONTEXT_THAI_H__ */
