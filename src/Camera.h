/*
 * Camera.h
 *
 *  Created on: 16/02/2014
 *      Author: molina
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#define SIZE 4
#define PI 3.1415

#include <cmath>
#include "Vector.h"
#include "TransformationMatrix.h"

typedef enum {
	ORTHOGRAPHIC = 1, OBLIQUE = 2, PERSPECTIVE = 3
} Projection;



class Camera {

	///////////////////////// CAMERA ATTRIBUTES //////////////////////////////
private:
	float V[16];
	Vector n;
	Vector u;
	Vector v;
	Vector UP;
	float P0[4];
	float Pref[4];

	///////////////////////// PERSPECTIVE ATTRIBUTES /////////////////////////
private:
	float P[16];
	float left;
	float right;
	float near;
	float far;
	float bottom;
	float top;
	float rAspect;
	Projection projection;

private:
	void refreshV();
	void redoProcess();
	void refreshP();
	void createOrthographicMatrix(float * p);
	void createObliqueMatrix(float * p);
	void createPerspectiveMatrix(float* p);
	static void matrixMultiplication(float* A, float* B, float* result);
public:
	Camera();
	virtual ~Camera();
	void moveCamera(float despX, float despY, float despZ);
	void moveTarget(float despX, float despY, float despZ);
	const float* getP0() const;
	const float* getV() const;
	void lookAt(float P0x, float P0y, float P0z, float destX, float destY,
			float destZ);
	void lookAt(float P0x, float P0y, float P0z, float destX, float destY,
			float destZ, float UPx, float UPy, float UPz);
	void setUPvector(const Vector& v);
	const float* getP() const;
	Projection getProjection() const;
	void setProjection(Projection projection);
	float getAspect() const;
	const float* getPref() const;
	void setAspect(float aspect);
	void printMatrix(float* p);
	void inverseV(float pointer[16]);

	////////////// MOVEMENTS ///////////////

public:
	void moveCameraUp(float c);
	void moveCameraDown(float c);
	void moveCameraRight(float c);
	void moveCameraLeft(float c);
	void moveCameraForward(float c);
	void moveCameraBackwards(float c);


	///////////// ROTATIONS ////////////////

public:
	void rotateXExperimental(float angle);
	void rotateYExperimental(float angle);
	void rotateZExperimental(float angle);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);


	////////////// DIRECTIONS //////////////
public:
	Vector directionLeft();
	Vector directionRight();
	Vector directionUp();
	Vector directionDown();
	Vector directionForward();
	Vector directionBackwards();

};

#endif /* CAMERA_H_ */
