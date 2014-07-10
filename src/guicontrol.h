#ifndef _GUICONTROL_H_
#define _GUICONTROL_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "guicallback.h"
#include "Camera.h"
#include "OFFfileReader.h"
#include "TransformationMatrix.h"
#include "Scene.h"




/* GETTERS AND SETTERS */

extern "C" gchar* gui_get_off_file();
extern "C" void gui_set_off_file(gchar* filename);
extern "C" void gui_set_move_mode(gint index);
extern "C" gint gui_get_move_mode();
extern "C" void gui_set_flip_normals(gboolean b);
extern "C" gboolean gui_get_flip_normals(void);
extern "C" void gui_set_I_light(GdkColor color);
extern "C" GdkColor gui_get_I_light();
extern "C" void gui_set_I_ambient(GdkColor color);
extern "C" GdkColor gui_get_I_ambient();
extern "C" void gui_set_k_ambient(gdouble k[3]);
extern "C" void gui_get_k_ambient(float k[3]);
extern "C" void gui_set_k_diffuse(gdouble k[3]);
extern "C" void gui_get_k_diffuse(float k[3]);
extern "C" void gui_set_k_specular(gdouble k[3]);
extern "C" void gui_get_k_specular(float k[3]);
extern "C" void gui_set_light_pos(float p[3]);
extern "C" void gui_get_light_pos(float p[3]);
extern "C" void gui_set_phong(int phong);
extern "C" gdouble gui_get_phong();

/* HELP FUNCTIONS */

extern "C" void gui_draw_color_shower(GtkWidget *widget, GdkColor *color);
extern "C" void gui_set_color(GtkRange *range, guint16 *color);



/* GLOBAL VARIABLES */

extern GtkWidget *gui_light_shower;
extern GtkWidget *gui_ambient_shower;

#endif
