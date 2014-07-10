#include "Scene.h"
#include <gtk/gtk.h>
#include "guicontrol.h"

//////////////////////// MAIN FUNCTION //////////////////////////

int main(int argc, char **argv) {

	////////////////// STARTING WITH GLUT ////////////////

	initGlut(argc, argv);
	initGL();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardFunction);
	glutIdleFunc(guiMainIteration);
	glutMouseFunc(mouseFunction);
	glutMotionFunc(motionFunction);

	//reading a file as an example
	readFile("sphere2.off");

	/////////////////////// STARTINNG THE GUI /////////////////

	GdkColor color;
	color.red = 65535;
	color.green = 0;
	color.blue = 32000;

	/* Initialize GUI */
	guiInit(&argc, argv);
	guiInitWindow("ass3gui.glade");

	gui_set_move_mode(0);
	gui_set_flip_normals(FALSE);
	gui_set_I_light(color);

	/* Set up exit function */
	atexit(&atclose);

	initParameters();
	glutMainLoop();

	return 0;
}

