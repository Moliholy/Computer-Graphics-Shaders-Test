/*
 * TransformationMatrix.h
 *
 *  Created on: 06/02/2014
 *      Author: MOLINA
 */

#ifndef TRANSFORMATIONMATRIX_H_
#define TRANSFORMATIONMATRIX_H_

#include <cmath>
#include <iostream>
#include "Vector.h"

class TransformationMatrix {
public:
	static const int MATRIX_SIZE = 4;

public:
	TransformationMatrix();
	TransformationMatrix(float initialAngle);
	TransformationMatrix(Vector& v1, Vector& v2, Vector& v3);
	virtual ~TransformationMatrix();

	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void translate(float x, float y);
	void translate(float x, float y, float z);
	void scale(float factor);
	void transpose();
	float& operator[](int pos);

	float* getMatrix() const {
		return (float*)matrix;
	}

private:
	float matrix[16];
};

#endif /* TRANSFORMATIONMATRIX_H_ */
