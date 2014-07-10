#include "Scene.h"

CameraMode cameraMode = CAMERA_MODE_NORMAL;
float cameraVelocity = 0.01f;
bool cameraMoving = false;
time_t lastMeasure;
Camera camera;

QuadraticBezier bezierCurve;

float K[9];
float I[6];
int numLights = 1;
float lights[MAX_LIGHTS * 3];
float phong = 0.0f;
Vector mobileLightDirection(-1, 0, 0);

TransformationMatrix mainMatrix;
GLuint idCameraMatrix;
GLuint idProjectionMatrix;
OFFfileReader fr;

GLuint buffer;
GLuint loc;
GLuint normal;
GLuint vao;
GLuint program;
GLuint idTransMat;
GLuint idElements;
GLuint idLights;
GLuint idNumLights;
GLuint idI;
GLuint idK;
GLuint idPhong;
GLuint idP0;

//for mouse function
int mouseX = -1;
int mouseY = -1;
bool shiftActive = false;

void printCameraPoints() {
	const float* P0 = camera.getP0();
	const float* Pref = camera.getPref();
	cout << "P0 = (" << P0[0] << ", " << P0[1] << ", " << P0[2] << ")" << endl;
	cout << "Pref = (" << Pref[0] << ", " << Pref[1] << ", " << Pref[2] << ")"
			<< endl;
}

void refreshNumLights() {
	glUniform1i(idNumLights, numLights);
}

void refreshCameraMatrix(bool printCameraPosition) {
	/*std::cout << std::endl << "Loading a new V:";
	 camera->printMatrix((float*) camera->getV());*/
	glUniformMatrix4fv(idCameraMatrix, 1, GL_FALSE, camera.getV());
	refreshP0();
	if (printCameraPosition)
		printCameraPoints();
}

void refreshProjectionMatrix(void) {
	/*std::cout << std::endl << "Loading a new P:";
	 camera->printMatrix((float*) camera->getP());*/
	glUniformMatrix4fv(idProjectionMatrix, 1, GL_FALSE, camera.getP());
}

void refreshVertexCoordinates() {
	glBufferSubData(GL_ARRAY_BUFFER, 0,
			3 * fr.getNumberOfVertices() * sizeof(float), fr.getVertices());
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
			(GLvoid*) BUFFER_OFFSET(0));
}

void refreshVertexNormals() {
	glBufferSubData(GL_ARRAY_BUFFER,
			3 * fr.getNumberOfVertices() * sizeof(float),
			3 * fr.getNumberOfVertices() * sizeof(float), fr.getNormals());
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0,
			(GLvoid*) BUFFER_OFFSET(
					3 * sizeof(float) * fr.getNumberOfVertices()));
}

void refreshReferenceArray() {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			3 * fr.getNumberOfFaces() * sizeof(uint), fr.getIndices(),
			GL_STATIC_DRAW);
}

void refreshLights() {
	glUniform3fv(idLights, numLights, (GLfloat*) lights);
}

void refreshK() {
	glUniform3fv(idK, 3, (GLfloat*) K);
}

void refreshI() {
	glUniform3fv(idI, 2, (GLfloat*) I);
}

void refreshPhong() {
	glUniform1f(idPhong, phong);
}

void refreshP0() {
	glUniform3fv(idP0, 1, camera.getP0());
}

bool readFile(string filename) {
	bool result = fr.open(filename);
	if (result) {
		//vertex array
		refreshVertexCoordinates();
		refreshVertexNormals();

		//references array
		refreshReferenceArray();

		glutPostRedisplay();
		return true;
	}

	return false;
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	float relation = width / (float) height;
	camera.setAspect(relation);
	refreshProjectionMatrix();
	glutPostRedisplay();
}

void atclose(void) {
	/* Buffers, programs and shaders should be deallocated */
	glDeleteProgram(program);
	glDeleteBuffers(1, &buffer);
	glDeleteBuffers(1, &idElements);
	glDeleteVertexArrays(1, &vao);
	//delete[] lights;
	printf("closing\n");
}

void initGlut(int argc, char **argv) {
	/* Initialize glut */
	glutInit(&argc, argv);

	/* Display Mode
	 GLUT_DOUBLE together with glutSwapBuffers(); for double buffering */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	/* Window Size */
	glutInitWindowSize(500, 500);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	/* GL Version
	 Check with glxinfo | grep Open for supported version */
	glutInitContextVersion(3, 3);

	/* Window label */
	glutCreateWindow("Assignment 3: Jose Molina Colmenero");
}

void initGL(void) {

	/* Setting up GL Extensions */
	glewExperimental = GL_TRUE;
	glewInit();

	/* Create and initialize a program object with shaders */
	program = initProgram("vshader.glsl", "fshader.glsl");

	/* installs the program object as part of current rendering state */
	glUseProgram(program);

	/* Create a vertex array object */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Create buffer in the shared display list space and
	 bind it as GL_ARRAY_BUFFER */
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
	BUFFER_SIZE * 3 * sizeof(float) * 2, NULL,
	GL_STATIC_DRAW);

	/* NEW: create a buffer to store the indices.
	 * This time we will use GL_ELEMENT_ARRAY_BUFFER */
	glGenBuffers(1, &idElements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idElements);

	/* Initialize attribute vPosition in program */
	loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);

	/* Initialize attribute vNormal in program */
	normal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(normal);

	/* Initialize attribute T in program */
	idTransMat = glGetUniformLocation(program, "T");
	glUniformMatrix4fv(idTransMat, 1, GL_FALSE, mainMatrix.getMatrix());

	/* Initialize attribute V in program */
	idCameraMatrix = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(idCameraMatrix, 1, GL_FALSE, camera.getV());

	/* Initialize attribute P in program */
	idProjectionMatrix = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(idProjectionMatrix, 1, GL_FALSE, camera.getP());

	/* Initialize parameter lights in program */
	idLights = glGetUniformLocation(program, "lights");

	/* Initialize parameter numLights in program */
	idNumLights = glGetUniformLocation(program, "numLights");

	/* Initialize parameter I in program */
	idI = glGetUniformLocation(program, "I");

	/* Initialize parameter K in program */
	idK = glGetUniformLocation(program, "K");

	/* Initialize parameter phong in program */
	idPhong = glGetUniformLocation(program, "phong");

	/* Initialize attribute P0 in program */
	idP0 = glGetUniformLocation(program, "P0");

	/* Set graphics attributes */
	glLineWidth(1.0f);
	glPointSize(1.0f);
	glClearColor(0.33f, 0.33f, 0.33f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	//setting the periodic function
	signal(SIGALRM, periodicRefreshing);
	ualarm(10000, 300000);
}

/**
 * This function is called once per frame
 */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, fr.getNumberOfFaces() * 3, GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
}

/**
 * Controls the user interation with the keyboard
 */
void keyboardFunction(unsigned char key, int x, int y) {
	float amount = 0.1f;
	switch (key) {
	case 'A':
		camera.rotateZExperimental(-10.0f);
		refreshCameraMatrix(true);
		break;
	case 'a':
		//move left
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraLeft(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionLeft() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
		break;
	case 'D':
		camera.rotateZExperimental(10.0f);
		refreshCameraMatrix(true);
		break;
	case 'd':
		//move right
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraRight(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionRight() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
		break;
	case 'W':
		camera.rotateXExperimental(10.0f);
		refreshCameraMatrix(true);
		break;
	case 'w':
		//move forward
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraForward(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionUp() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
		break;
	case 'S':
		camera.rotateXExperimental(-10.0f);
		refreshCameraMatrix(true);
		break;
	case 's':
		//move backward
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraBackwards(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionDown() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
		break;
	case 'Z':
		camera.rotateYExperimental(10.0f);
		refreshCameraMatrix(false);
		break;
	case 'z':
		//move up
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraUp(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionUp() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
		break;
	case 'X':
		camera.rotateYExperimental(-10.0f);
		refreshCameraMatrix(false);
		break;
	case 'x':
		//move down
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraDown(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionDown() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
		break;
	case 'I':
	case 'i':
		initParameters();
		break;
	case '+':
		cameraVelocity += 0.01f;
		break;
	case '-':
		cameraVelocity = max(cameraVelocity - 0.01f, 0.01f);
		break;
	}
	glutPostRedisplay();
}

/**
 * Set the initial GUI parameters
 */
void initialGuiParameters() {
	GdkColor c;
	c.red = MAX_RANGE / 3;
	c.green = MAX_RANGE / 4;
	c.blue = MAX_RANGE;
	gui_set_I_ambient(c);

	c.red = MAX_RANGE;
	c.green = MAX_RANGE / 4;
	c.blue = MAX_RANGE / 3;
	gui_set_I_light(c);

	double ka[] { 0.5, 0.4, 0.1 };
	gui_set_k_ambient(ka);

	double kd[] { 0.2, 0.9, 0.7 };
	gui_set_k_diffuse(kd);

	double ks[] { 0.1, 0.9, 0.5 };
	gui_set_k_specular(ks);

	float pos[] { 999, -999, -999 };
	gui_set_light_pos(pos);

	gui_set_move_mode(0);
}

/**
 * Initializes inner parameters, as well as those that belong
 * to the graphic interface
 */
void initParameters() {

	initialGuiParameters();

	cameraMode = (CameraMode) gui_get_move_mode();
	cameraMoving = !(cameraMode == CAMERA_MODE_NORMAL);

	//light positions
	numLights = 3;
	lights[3] = 0;
	lights[4] = 10;
	lights[5] = 0;

	lights[6] = 10;
	lights[7] = -7;
	lights[8] = 3;

	//values of I
	GdkColor colorAmbient = gui_get_I_ambient();
	I[0] = (float) colorAmbient.red / MAX_RANGE;
	I[1] = (float) colorAmbient.green / MAX_RANGE;
	I[2] = (float) colorAmbient.blue / MAX_RANGE;

	GdkColor colorDifuse = gui_get_I_light();
	I[3] = (float) colorDifuse.red / MAX_RANGE;
	I[4] = (float) colorDifuse.green / MAX_RANGE;
	I[5] = (float) colorDifuse.blue / MAX_RANGE;

	//values of K
	gui_get_k_ambient(K);
	gui_get_k_diffuse(K + 3);
	gui_get_k_specular(K + 6);

	//phong coefficient
	phong = 60.0f;
	gui_set_phong((int) phong);

	//camera
	camera.lookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);

	//refreshing
	refreshCameraMatrix(true);
	refreshPhong();
	refreshLights();
	refreshNumLights();
	refreshI();
	refreshK();
}

void periodicRefreshing(int ig) {
	//here is the code to process changes in camera's movement
	if (cameraMode == CAMERA_MODE_BEZIER && cameraMoving) {
		time_t now = time(NULL);
		time_t period = now - lastMeasure;
		float displacement = period * cameraVelocity;
		Vector nextP0 = bezierCurve.getNextPoint(displacement);

		Vector nextPref = bezierCurve.getTangent() + nextP0;
		camera.lookAt(nextP0[0], nextP0[1], nextP0[2], nextPref[0], nextPref[1],
				nextPref[2]);

		refreshCameraMatrix(false);
		glutPostRedisplay();
		lastMeasure = now;
	}
	if (numLights >= 2) {
		if (lights[3] > 20.0f || lights[3] < -20.0f)
			mobileLightDirection = mobileLightDirection * -1.0f;
		lights[3] += mobileLightDirection[0];
		refreshLights();
		glutPostRedisplay();
	}
}

void mouseFunction(int button, int state, int x, int y) {
	float amount = 0.1;
	if (button == 3) {
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraForward(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionUp() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
	} else if (button == 4) {
		if (cameraMode == CAMERA_MODE_NORMAL) {
			camera.moveCameraBackwards(amount);
			refreshCameraMatrix(true);
		} else {
			if (!cameraMoving) {
				cameraMoving = true;
			}
			Vector P0 = Vector((float*) camera.getP0());
			Vector P1 = P0 + camera.directionForward() * 0.2f;
			Vector dir = camera.directionDown() + camera.directionForward();
			dir.normalize();
			Vector P2 = P1 + dir * 0.2f;
			bezierCurve.set(P0, P1, P2);

			lastMeasure = time(NULL);
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouseX = mouseY = -1;
		shiftActive = false;
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
			shiftActive = true;
		//mouse vector calculation
		if (mouseX < 0 || mouseY < 0) {
			mouseX = x;
			mouseY = y;
		}
	}
	glutPostRedisplay();
}

void motionFunction(int x, int y) {
	if (mouseX > 0 && mouseY > 0) {
		float amount = 0.02f;
		Vector direction(x - mouseX, y - mouseY, 0, 0);
		mouseX = x;
		mouseY = y;

		if (shiftActive) {
			float rotation = 0.5f;
			//we have to rotate
			direction[0] > 0 ?
					camera.rotateZExperimental(-rotation) :
					camera.rotateZExperimental(rotation);

			direction[1] > 0 ?
					camera.rotateXExperimental(rotation) :
					camera.rotateXExperimental(-rotation);

			refreshCameraMatrix(true);
		} else {
			//translations
			if (direction[0] < 0) {
				//keyboard a
				if (cameraMode == CAMERA_MODE_NORMAL) {
					camera.moveCameraLeft(amount);
					refreshCameraMatrix(true);
				} else {
					if (!cameraMoving) {
						cameraMoving = true;
					}
					Vector P0 = Vector((float*) camera.getP0());
					Vector P1 = P0 + camera.directionForward() * 0.2f;
					Vector dir = camera.directionLeft()
							+ camera.directionForward();
					dir.normalize();
					Vector P2 = P1 + dir * 0.2f;
					bezierCurve.set(P0, P1, P2);

					lastMeasure = time(NULL);
				}
			}

			else {
				//keyboard d
				if (cameraMode == CAMERA_MODE_NORMAL) {
					camera.moveCameraRight(amount);
					refreshCameraMatrix(true);
				} else {
					if (!cameraMoving) {
						cameraMoving = true;
					}
					Vector P0 = Vector((float*) camera.getP0());
					Vector P1 = P0 + camera.directionForward() * 0.2f;
					Vector dir = camera.directionRight()
							+ camera.directionForward();
					dir.normalize();
					Vector P2 = P1 + dir * 0.2f;
					bezierCurve.set(P0, P1, P2);

					lastMeasure = time(NULL);
				}
			}
			if (direction[1] < 0) {
				//keyboard z
				if (cameraMode == CAMERA_MODE_NORMAL) {
					camera.moveCameraUp(amount);
					refreshCameraMatrix(true);
				} else {
					if (!cameraMoving) {
						cameraMoving = true;
					}
					Vector P0 = Vector((float*) camera.getP0());
					Vector P1 = P0 + camera.directionForward() * 0.2f;
					Vector dir = camera.directionUp()
							+ camera.directionForward();
					dir.normalize();
					Vector P2 = P1 + dir * 0.2f;
					bezierCurve.set(P0, P1, P2);

					lastMeasure = time(NULL);
				}
			} else {
				//keyboard x
				if (cameraMode == CAMERA_MODE_NORMAL) {
					camera.moveCameraDown(amount);
					refreshCameraMatrix(true);
				} else {
					if (!cameraMoving) {
						cameraMoving = true;
					}
					Vector P0 = Vector((float*) camera.getP0());
					Vector P1 = P0 + camera.directionForward() * 0.2f;
					Vector dir = camera.directionDown()
							+ camera.directionForward();
					dir.normalize();
					Vector P2 = P1 + dir * 0.2f;
					bezierCurve.set(P0, P1, P2);

					lastMeasure = time(NULL);
				}
			}

		}
		glutPostRedisplay();
	}
}

