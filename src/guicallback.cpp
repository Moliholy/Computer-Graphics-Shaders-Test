#include "guicallback.h"

GdkColor gui_light_color;
GdkColor gui_ambient_color;

void on_off_chooser_selection_changed(GtkFileChooser *filechooser,
		gpointer user_data) {
	gchar* filename = gtk_file_chooser_get_filename(filechooser);
	readFile((const char*) filename);
}

void on_flip_normals_toggled(GtkToggleButton *togglebutton,
		gpointer user_data) {
	gboolean b = togglebutton->active;

	fr.flipNormals();
	refreshVertexNormals();
	glutPostRedisplay();
}

void on_move_mode_changed(GtkComboBox *combobox, gpointer user_data) {
	gint index = gtk_combo_box_get_active(combobox);
	cameraMode = (CameraMode) index;
	if (cameraMode == CAMERA_MODE_NORMAL) {
		cameraMoving = false;
	}
}

void on_light_x_value_changed(GtkSpinButton*spinbutton, gpointer user_data) {
	gdouble value = gtk_spin_button_get_value(spinbutton);

	lights[0] = value;
	refreshLights();
	glutPostRedisplay();
}

void on_light_y_value_changed(GtkSpinButton *spinbutton, gpointer user_data) {
	gdouble value = gtk_spin_button_get_value(spinbutton);

	lights[1] = value;
	refreshLights();
	glutPostRedisplay();
}

void on_light_z_value_changed(GtkSpinButton*spinbutton, gpointer user_data) {
	gdouble value = gtk_spin_button_get_value(spinbutton);

	lights[2] = value;
	refreshLights();
	glutPostRedisplay();
}

gboolean on_I_light_r_value_changing(GtkRange *range, gpointer user_data) {
	gui_set_color(range, &gui_light_color.red);
	gui_draw_color_shower(gui_light_shower, &gui_light_color);
	gtk_widget_queue_draw(gui_light_shower);

	return FALSE;
}

gboolean on_I_light_g_value_changing(GtkRange *range, gpointer user_data) {
	gui_set_color(range, &gui_light_color.green);
	gui_draw_color_shower(gui_light_shower, &gui_light_color);
	gtk_widget_queue_draw(gui_light_shower);

	return FALSE;
}

gboolean on_I_light_b_value_changing(GtkRange *range, gpointer user_data) {
	gui_set_color(range, &gui_light_color.blue);
	gui_draw_color_shower(gui_light_shower, &gui_light_color);
	gtk_widget_queue_draw(gui_light_shower);

	return FALSE;
}

void on_I_light_value_changed(GdkColor color) {
	I[3] = (float) color.red / MAX_RANGE;
	I[4] = (float) color.green / MAX_RANGE;
	I[5] = (float) color.blue / MAX_RANGE;
	refreshI();
	glutPostRedisplay();
}

void on_I_light_r_value_changed(GtkRange *range, gpointer user_data) {
	on_I_light_r_value_changing(range, user_data);
	on_I_light_value_changed(gui_light_color);
}

void on_I_light_g_value_changed(GtkRange *range, gpointer user_data) {
	on_I_light_g_value_changing(range, user_data);
	on_I_light_value_changed(gui_light_color);
}

void on_I_light_b_value_changed(GtkRange *range, gpointer user_data) {
	on_I_light_b_value_changing(range, user_data);
	on_I_light_value_changed(gui_light_color);
}

gboolean on_I_ambient_r_value_changing(GtkRange *range, gpointer user_data) {
	gui_set_color(range, &gui_ambient_color.red);
	gui_draw_color_shower(gui_ambient_shower, &gui_ambient_color);
	gtk_widget_queue_draw(gui_ambient_shower);

	return FALSE;
}

gboolean on_I_ambient_g_value_changing(GtkRange *range, gpointer user_data) {
	gui_set_color(range, &gui_ambient_color.green);
	gui_draw_color_shower(gui_ambient_shower, &gui_ambient_color);
	gtk_widget_queue_draw(gui_ambient_shower);

	return FALSE;
}

gboolean on_I_ambient_b_value_changing(GtkRange *range, gpointer user_data) {
	gui_set_color(range, &gui_ambient_color.blue);
	gui_draw_color_shower(gui_ambient_shower, &gui_ambient_color);
	gtk_widget_queue_draw(gui_ambient_shower);

	return FALSE;
}

void on_I_ambient_value_changed(GdkColor color) {
	I[0] = (float) color.red / MAX_RANGE;
	I[1] = (float) color.green / MAX_RANGE;
	I[2] = (float) color.blue / MAX_RANGE;
	refreshI();
	glutPostRedisplay();
}

void on_I_ambient_r_value_changed(GtkRange *range, gpointer user_data) {
	on_I_ambient_r_value_changing(range, user_data);
	on_I_ambient_value_changed(gui_ambient_color);
}

void on_I_ambient_g_value_changed(GtkRange *range, gpointer user_data) {
	on_I_ambient_g_value_changing(range, user_data);
	on_I_ambient_value_changed(gui_ambient_color);
}

void on_I_ambient_b_value_changed(GtkRange *range, gpointer user_data) {
	on_I_ambient_b_value_changing(range, user_data);
	on_I_ambient_value_changed(gui_ambient_color);
}

void on_ambient_value_changed(GdkColor color) {
	I[0] = (float) color.red / MAX_RANGE;
	I[1] = (float) color.green / MAX_RANGE;
	I[2] = (float) color.blue / MAX_RANGE;
	refreshI();
}

void on_k_ambient_r_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[0] = value;
	refreshK();
	glutPostRedisplay();

}

void on_k_ambient_g_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[1] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_ambient_b_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[2] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_diffuse_r_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[3] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_diffuse_g_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[4] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_diffuse_b_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[5] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_specular_r_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[6] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_specular_g_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[7] = value;
	refreshK();
	glutPostRedisplay();
}

void on_k_specular_b_value_changed(GtkRange *range, gpointer user_data) {
	gdouble value = gtk_range_get_value(range);

	K[8] = value;
	refreshK();
	glutPostRedisplay();
}

void on_phong_value_changed(GtkSpinButton *spinbutton, gpointer user_data) {
	gint32 value = (gint32) round(gtk_spin_button_get_value(spinbutton));

	phong = value;
	refreshPhong();
	glutPostRedisplay();
}

void on_win_destroy_event() {
	exit(0);
}

gboolean on_ambient_shower_expose_event(GtkWidget *widget,
		GdkEventExpose *event, gpointer user_data) {
	gui_draw_color_shower(widget, &gui_ambient_color);

	return TRUE;
}

gboolean on_light_shower_expose_event(GtkWidget *widget, GdkEventExpose *event,
		gpointer user_data) {
	gui_draw_color_shower(widget, &gui_light_color);

	return TRUE;
}
