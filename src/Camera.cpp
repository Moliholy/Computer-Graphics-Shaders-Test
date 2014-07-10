/*
 * Camera.cpp
 *
 *  Created on: 16/02/2014
 *      Author: Volina
 */

#include "Camera.h"

Camera::Camera() {
	//initial position in (0,0,1) looking at (0,0,0) with UP vector (0,1,0)
	P0[0] = 0.0f;
	P0[1] = 0.0f;
	P0[2] = 1.0f;
	P0[3] = 1.0f;

	Pref[0] = Pref[1] = Pref[2] = 0.0f;

	UP.set(0.0f, 1.0f, 0.0f);

	redoProcess();

	projection = ORTHOGRAPHIC;
	left = -1.0f;
	right = 1.0f;
	top = 1.0f;
	bottom = -1.0f;
	near = 1.0f;
	far = 1000.0f; //far enough
	rAspect = 1.0f;

	refreshP();
}

void Camera::refreshV() {
	Vector eVec(P0[0], P0[1], P0[2]);
	//vectors by columns!
	V[0] = u[0];
	V[4] = u[1];
	V[8] = u[2];
	V[12] = -eVec.dotProduct(u);

	V[1] = v[0];
	V[5] = v[1];
	V[9] = v[2];
	V[13] = -eVec.dotProduct(v);

	V[2] = n[0];
	V[6] = n[1];
	V[10] = n[2];
	V[14] = -eVec.dotProduct(n);

	V[3] = 0.0f;
	V[7] = 0.0f;
	V[11] = 0.0f;
	V[15] = 1.0f;
}

Camera::~Camera() {
}

void Camera::lookAt(float P0x, float P0y, float P0z, float destX, float destY,
		float destZ) {
	//now we have to redo all
	//we DO have UP vector untouched
	P0[0] = P0x;
	P0[1] = P0y;
	P0[2] = P0z;

	Pref[0] = destX;
	Pref[1] = destY;
	Pref[2] = destZ;

	redoProcess();
}

void Camera::lookAt(float P0x, float P0y, float P0z, float destX, float destY,
		float destZ, float UPx, float UPy, float UPz) {
	P0[0] = P0x;
	P0[1] = P0y;
	P0[2] = P0z;

	Pref[0] = destX;
	Pref[1] = destY;
	Pref[2] = destZ;

	redoProcess();
}

const float* Camera::getP0() const {
	return P0;
}

const float* Camera::getV() const {
	return V;
}

void Camera::moveCamera(float despX, float despY, float despZ) {
	P0[0] += despX;
	P0[1] += despY;
	P0[2] += despZ;

	redoProcess();
}

void Camera::moveTarget(float despX, float despY, float despZ) {
	Pref[0] += despX;
	Pref[1] += despY;
	Pref[2] += despZ;

	redoProcess();
}

void Camera::redoProcess() {
	n.set(P0[0] - Pref[0], P0[1] - Pref[1], P0[2] - Pref[2]);
	n.normalize();

	u = UP.crossProduct(n);
	u.normalize();

	v = n.crossProduct(u);

	refreshV();
}

void Camera::setUPvector(const Vector& vector) {
	UP = vector;

	u = UP.crossProduct(n);
	u.normalize();

	v = n.crossProduct(u);

	refreshV();
}

void Camera::moveCameraUp(float c) {
	Vector vector = v * c;
	P0[0] += vector[0];
	P0[1] += vector[1];
	P0[2] += vector[2];

	Pref[0] += vector[0];
	Pref[1] += vector[1];
	Pref[2] += vector[2];

	refreshV();
}

void Camera::moveCameraDown(float c) {
	Vector vector = v * (-c);
	P0[0] += vector[0];
	P0[1] += vector[1];
	P0[2] += vector[2];

	Pref[0] += vector[0];
	Pref[1] += vector[1];
	Pref[2] += vector[2];

	refreshV();
}

void Camera::moveCameraRight(float c) {
	Vector vector = u * c;
	P0[0] += vector[0];
	P0[1] += vector[1];
	P0[2] += vector[2];

	Pref[0] += vector[0];
	Pref[1] += vector[1];
	Pref[2] += vector[2];

	refreshV();
}

void Camera::moveCameraLeft(float c) {
	Vector vector = u * (-c);
	P0[0] += vector[0];
	P0[1] += vector[1];
	P0[2] += vector[2];

	Pref[0] += vector[0];
	Pref[1] += vector[1];
	Pref[2] += vector[2];

	refreshV();
}

void Camera::moveCameraForward(float c) {
	Vector vector = n * (-c);
	P0[0] += vector[0];
	P0[1] += vector[1];
	P0[2] += vector[2];

	Pref[0] += vector[0];
	Pref[1] += vector[1];
	Pref[2] += vector[2];

	refreshV();
}

const float* Camera::getP() const {
	return P;
}

Projection Camera::getProjection() const {
	return projection;
}

void Camera::refreshP() {

	switch (projection) {
	case ORTHOGRAPHIC:
		createOrthographicMatrix(P);
		break;
	case OBLIQUE:
		createObliqueMatrix(P);
		break;
	case PERSPECTIVE:
		createPerspectiveMatrix(P);
		break;
	}
}

void Camera::setProjection(Projection projection) {
	this->projection = projection;
	refreshP();
}

float Camera::getAspect() const {
	return rAspect;
}

void Camera::setAspect(float aspect) {
	rAspect = aspect;
	refreshP();
}

void Camera::createOrthographicMatrix(float* p) {
	for (int i = 0; i < SIZE * SIZE; i++)
		p[i] = 0.0f;
	//row 1
	p[0] = 2.0f / (right - left);
	p[3] = -(left + right) / (right - left);
	//row 2
	p[5] = 2.0f / (top - bottom);
	p[7] = -(top + bottom) / (top - bottom);
	//row 3
	p[10] = -2.0f / (top - bottom);
	p[11] = -(far + near) / (far - near);
	//row 4
	p[15] = 1.0f;
}

void Camera::createObliqueMatrix(float* p) {
	float ortho[16];
	createOrthographicMatrix(ortho);

	float H[16];
	for (int i = 0; i < SIZE * SIZE; i++)
		H[i] = 0.0f;

	//CABINET PROJECTION
	H[0] = H[5] = H[15] = 1.0f;
	H[2] = sqrt(3.0f) / 4.0f;
	H[6] = 0.25f;

	matrixMultiplication(ortho, H, p);
}

void Camera::createPerspectiveMatrix(float* p) {
	for (int i = 0; i < SIZE * SIZE; i++)
		p[i] = 0.0f;

	float ntop = near * tanf(45.0f * 2 * 3.1415 / 360);
	float nright = -top * rAspect;

	//row 1
	p[0] = near / nright;

	//ro2 2
	p[5] = near / ntop;

	//row 3
	p[10] = -(far + near) / (far - near);
	p[11] = (-2.0f * far * near) / (far - near);

	//row 4
	p[14] = -1.0f;

}

void Camera::matrixMultiplication(float* A, float* B, float* result) {
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++) {
			result[i * SIZE + j] = 0.0f;
			for (int k = 0; k < SIZE; k++)
				result[i * SIZE + j] += A[i * SIZE + k] * B[k * SIZE + j];
		}
}

void Camera::printMatrix(float* p) {
	for (int i = 0; i < SIZE; i++) {
		std::cout << std::endl;
		for (int j = 0; j < SIZE; j++) {
			std::cout << p[i * SIZE + j] << " ";
		}
	}
	std::cout << std::endl;
}

const float* Camera::getPref() const {
	return Pref;
}

void Camera::moveCameraBackwards(float c) {
	Vector vector = n * c;
	P0[0] += vector[0];
	P0[1] += vector[1];
	P0[2] += vector[2];

	Pref[0] += vector[0];
	Pref[1] += vector[1];
	Pref[2] += vector[2];

	refreshV();
}

Vector Camera::directionLeft() {
	return u * -1;
}

Vector Camera::directionRight() {
	return u;
}

Vector Camera::directionUp() {
	return v;
}

Vector Camera::directionDown() {
	return v * -1;
}

Vector Camera::directionForward() {
	return n * -1;
}

void Camera::inverseV(float pointer[16]) {
	/*TransformationMatrix T;
	 T.translate(P0[0], P0[1], P0[2]);
	 T.transpose();*/

	TransformationMatrix M(u, v, n);
	//M.transpose();
	M[3] = P0[0];
	M[7] = P0[1];
	M[11] = P0[2];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			pointer[i * 4 + j] = M[j * 4 + i];

	/*float aux;
	 for (int i = 0; i < 4; i++) {
	 for (int j = i; j < 4; j++) {
	 aux = pointer[i * 4 + j];
	 pointer[i * 4 + j] = pointer[j * 4 + i];
	 pointer[j * 4 + i] = aux;
	 }
	 }*/
}

void Camera::rotateX(float angle) {
	float invV[16];
	inverseV(invV);

	/*float identity[16];
	 matrixMultiplication(V, invV, identity);*/

	TransformationMatrix R;
	R.rotateX(angle);

	Vector Pr(0, 0, -2, 1);
	Vector finalResult = Pr.matrixMultiplication(R.getMatrix());

	finalResult = finalResult.matrixMultiplication(invV);

	Pref[0] = finalResult[0];
	Pref[1] = finalResult[1];
	Pref[2] = finalResult[2];

	redoProcess();
}

void Camera::rotateY(float angle) {
	float invV[16];
	inverseV(invV);

	TransformationMatrix R;
	R.rotateY(angle);

	float result[16];
	matrixMultiplication(invV, R.getMatrix(), result);

	Vector Pr(Pref);
	Vector finalResult = Pr.matrixMultiplication(result);

	Pref[0] = Pr[0];
	Pref[1] = Pr[1];
	Pref[2] = Pr[2];

	redoProcess();
}

void Camera::rotateZ(float angle) {
	float invV[16];
	inverseV(invV);

	TransformationMatrix R;
	R.rotateZ(angle);

	float result[16];
	matrixMultiplication(invV, R.getMatrix(), result);

	Vector Pr(Pref);
	Vector finalResult = Pr.matrixMultiplication(result);

	Pref[0] = Pr[0];
	Pref[1] = Pr[1];
	Pref[2] = Pr[2];

	redoProcess();
}

void Camera::rotateXExperimental(float angle) {
	float transformationToRadians = 2 * PI / 360;
	float angleRadians = angle * transformationToRadians;

	Vector up = v;

	float theta = acos(up.dotProduct(n));

	float thetaComplementary = PI - theta;
	float currentAngle = PI / 2 - thetaComplementary;

	if (std::abs(currentAngle + angleRadians) < PI / 2) {
		float alpha = PI - theta - angleRadians;

		float distance = Vector(Pref[0] - P0[0], Pref[1] - P0[1],
				Pref[2] - P0[2]).mod();

		float displacement = distance * sin(angleRadians) / sin(alpha);

		Pref[1] += displacement;
		redoProcess();
	}
}

void Camera::rotateYExperimental(float angle) {
	float transformationToRadians = 2 * PI / 360;
	float angleRadians = angle * transformationToRadians;

	float displacement = tan(angleRadians);
	Vector direction = UP.crossProduct(n);

	Vector d = direction * displacement;

	UP = UP + d;
	UP.normalize();

	redoProcess();

}

void Camera::rotateZExperimental(float angle) {
	float transformationToRadians = 2 * PI / 360;
	float angleRadians = angle * transformationToRadians;
	Vector up(0, 1, 0);

	float theta = acos(up.dotProduct(n));

	float thetaComplementary = PI - theta;
	float currentAngle = PI / 2 - thetaComplementary;

	if (std::abs(currentAngle + angleRadians) < PI / 2) {
		float alpha = PI - theta - angleRadians;

		float distance = Vector(Pref[0] - P0[0], Pref[1] - P0[1],
				Pref[2] - P0[2]).mod();

		float displacement = distance * sin(angleRadians) / sin(alpha);

		Vector d = u * displacement;

		Pref[0] += d[0];
		Pref[1] += d[1];
		Pref[2] += d[2];

		redoProcess();
	}
}

Vector Camera::directionBackwards() {
	return n;
}
