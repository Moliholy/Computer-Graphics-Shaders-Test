/*
 * TransformationMatrix.cpp
 *
 *  Created on: 06/02/2014
 *      Author: MOLINA
 */

#include "TransformationMatrix.h"

TransformationMatrix::TransformationMatrix() {
	//let's create the identity matrix
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			if (i == j)
				matrix[i * MATRIX_SIZE + j] = 1.0f;
			else
				matrix[i * MATRIX_SIZE + j] = 0.0f;
		}
	}
}

TransformationMatrix::~TransformationMatrix() {
}

void TransformationMatrix::translate(float x, float y) {
	matrix[12] += x;
	matrix[13] += y;
}

void TransformationMatrix::translate(float x, float y, float z) {
	matrix[12] += x;
	matrix[13] += y;
	matrix[14] += z;
}

TransformationMatrix::TransformationMatrix(Vector& v1, Vector& v2, Vector& v3) {
	int count = 0;
	for (int i = 0; i < MATRIX_SIZE; i++) {
		matrix[count++] = v1[i];
		matrix[count++] = v2[i];
		matrix[count++] = v3[i];
		matrix[count++] = 0.0f;
	}
	matrix[15] = 1.0f;
}

void TransformationMatrix::scale(float factor) {
	TransformationMatrix result, scale;
	scale.matrix[0] = scale.matrix[5] = scale.matrix[10] = factor;

	// Matrix multiplication
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			for (int k = 0; k < MATRIX_SIZE; k++)
				result.matrix[i * MATRIX_SIZE + j] +=
						matrix[i * MATRIX_SIZE + k]
								* scale.matrix[k * MATRIX_SIZE + j];

	// copying from the result matrix
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			matrix[i * MATRIX_SIZE + j] = result.matrix[i * MATRIX_SIZE + j];
}

TransformationMatrix::TransformationMatrix(float initialAngle) {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			if (i == j)
				matrix[i * MATRIX_SIZE + j] = 1.0f;
			else
				matrix[i * MATRIX_SIZE + j] = 0.0f;
		}
	}

	float transformationToRadians = 2 * 3.1415 / 360;

	float cosine = cos(initialAngle * transformationToRadians);
	float sine = sin(initialAngle * transformationToRadians);

	matrix[0] = cosine;
	matrix[1] = sine;
	matrix[4] = -sine;
	matrix[5] = cosine;
}

void TransformationMatrix::rotateX(float angle) {
	float transformationToRadians = 2 * 3.1415 / 360;
	float cosine = cos(angle * transformationToRadians);
	float sine = sin(angle * transformationToRadians);

	TransformationMatrix R, result;
	R.matrix[5] = cosine;
	R.matrix[6] = sine;
	R.matrix[9] = -sine;
	R.matrix[10] = cosine;

	result.matrix[0] = result.matrix[5] = result.matrix[10] =
			result.matrix[15] = 0.0f;

	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			for (int k = 0; k < MATRIX_SIZE; k++)
				result.matrix[i * MATRIX_SIZE + j] +=
						matrix[i * MATRIX_SIZE + k]
								* R.matrix[k * MATRIX_SIZE + j];

	// copying from the result matrix
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			matrix[i * MATRIX_SIZE + j] = result.matrix[i * MATRIX_SIZE + j];
}

void TransformationMatrix::rotateY(float angle) {
	float transformationToRadians = 2 * 3.1415 / 360;
	float cosine = cos(angle * transformationToRadians);
	float sine = sin(angle * transformationToRadians);

	TransformationMatrix R, result;
	R.matrix[0] = cosine;
	R.matrix[2] = sine;
	R.matrix[8] = -sine;
	R.matrix[10] = cosine;

	result.matrix[0] = result.matrix[5] = result.matrix[10] =
			result.matrix[15] = 0.0f;

	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			for (int k = 0; k < MATRIX_SIZE; k++)
				result.matrix[i * MATRIX_SIZE + j] +=
						matrix[i * MATRIX_SIZE + k]
								* R.matrix[k * MATRIX_SIZE + j];

	// copying from the result matrix
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			matrix[i * MATRIX_SIZE + j] = result.matrix[i * MATRIX_SIZE + j];
}

void TransformationMatrix::rotateZ(float angle) {
	TransformationMatrix result, rotation(angle);
	result.matrix[0] = result.matrix[5] = result.matrix[10] =
			result.matrix[15] = 0.0f;

	// Matrix multiplication
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			for (int k = 0; k < MATRIX_SIZE; k++)
				result.matrix[i * MATRIX_SIZE + j] +=
						matrix[i * MATRIX_SIZE + k]
								* rotation.matrix[k * MATRIX_SIZE + j];

	// copying from the result matrix
	for (int i = 0; i < MATRIX_SIZE; i++)
		for (int j = 0; j < MATRIX_SIZE; j++)
			matrix[i * MATRIX_SIZE + j] = result.matrix[i * MATRIX_SIZE + j];
}

void TransformationMatrix::transpose() {
	float aux;
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = i; j < MATRIX_SIZE; j++) {
			aux = matrix[i * MATRIX_SIZE + j];
			matrix[i * MATRIX_SIZE + j] = matrix[j * MATRIX_SIZE + i];
			matrix[j * MATRIX_SIZE + i] = aux;
		}
	}
}

float& TransformationMatrix::operator [](int pos) {
	return matrix[pos];
}
