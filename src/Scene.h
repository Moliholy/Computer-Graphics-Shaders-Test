#ifndef SCENE_H_
#define SCENE_H_

/************************* MAIN FILE ***********************/

/* This file contains all user interations as well as
 * necessary controls for the graphic interface */

#include "shadertools.h"
#include "OFFfileReader.h"
#include "Camera.h"
#include "TransformationMatrix.h"
#include "guicontrol.h"
#include "QuadraticBezier.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <gtk/gtk.h>
#include <ctime>

/* For alarms */
#include <signal.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 2048
#define MAX_LIGHTS 32
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define MINDEX(r,c) ((r-1)+(c+1)*4)

enum CameraMode {
	CAMERA_MODE_NORMAL = 0, CAMERA_MODE_BEZIER = 1
};

/* Camera data */
extern CameraMode cameraMode;
extern float cameraVelocity;
extern bool cameraMoving;
extern time_t lastMeasure;

/* Main Quadratic Bezier Curve */
extern QuadraticBezier bezierCurve;

/* Main camera */
extern Camera camera;

/*
 * Ambient (k[0], k[1], k[2])
 * Diffuse (k[3], k[4], k[5])
 * Specular (k[6], k[7], k[8])
 */
extern float K[];

/*
 * Ambient (I[0], I[1], I[2])
 * Intensity (I[3], I[4], I[5])
 */
extern float I[];

/* Phong coefficient */
extern float phong;

/* Lights we are going to use */
extern int numLights;
extern float lights[];

/* OFF file reader we shall use for reading the files and loading objects */
extern OFFfileReader fr;

/* Transformation matrix. In this program it will be the identity */
extern TransformationMatrix mainMatrix;

/* IDs we are going to use in the program */
extern GLuint idProjectionMatrix;
extern GLuint idCameraMatrix;
extern GLuint idTransMat;
extern GLuint idElements;
extern GLuint idLights;
extern GLuint idNumLights;
extern GLuint buffer;
extern GLuint loc;
extern GLuint normal;
extern GLuint vao;
extern GLuint program;
extern GLuint idPhong;
extern GLuint idP0;

/* For signals */
extern "C" void periodicRefreshing(int ig);

/* INITIALIZERS */

/* Initializes the glade gui.
 input:
 argc: number of string items in argv.
 argv: command line arguments as a string array.
 */
extern "C" void guiInitWindow(const char*);

/* Initialize GTK.
 */
extern "C" void guiInit(int *, char**);

/* Calls a single GTK main loop iteration to process
 * events (non-blocking).
 */
extern "C" void guiMainIteration();

/* Functions to refresh the parameters */
void refreshCameraMatrix(bool printCameraPosition);
void refreshProjectionMatrix(void);
void refreshVertexCoordinates();
void refreshVertexNormals();
void refreshReferenceArray();
void refreshLights();
void refreshK();
void refreshI();
void refreshPhong();
void refreshP0();
void refreshNumLights();

//Reads a file using our OFFfileReader
bool readFile(string filename);

//Called when the program finishes
void atclose(void);

/* GLUT functions */
void initParameters();
void reshape(int, int);
void initGlut(int argc, char **argv);
void initGL(void);
void display(void);
void keyboardFunction(unsigned char key, int x, int y);
void mouseFunction(int button, int state, int x, int y);
void motionFunction(int x, int y);

#endif /* SCENE_H_ */
