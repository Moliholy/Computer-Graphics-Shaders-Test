/*
 * OOFfileReader.cpp
 *
 *  Created on: 06/02/2014
 *      Author: MOLINA
 */

#include "OFFfileReader.h"
#include <fstream>
#include <cmath>

OFFfileReader::OFFfileReader() {
	nVertices = nFaces = nEdges = 0;
	vertices = normals = 0;
}

OFFfileReader::~OFFfileReader() {
	delete[] vertices;
	delete[] normals;
}

/**
 * @brief
 */
bool OFFfileReader::open(string filename) {
	//first we have to clear everything
	nVertices = nFaces = nEdges = 0;
	delete[] vertices;
	delete[] normals;
	vertices = normals = 0;

	//opening the file
	ifstream is;
	is.open(filename.c_str());
	if (is.good()) {
		string head;
		getline(is, head); //that should be the OFF header

		//gathering the data
		is >> nVertices >> nFaces >> nEdges;

		//step 1: allocation of memory
		vertices = new float[nVertices * 3];
		normals = new float[nVertices * 3];

		//step 2: copying values to the vertex array and initializing normals
		float max = 0.0f;
		for (uint i = 0; i < nVertices * 3; ++i) {
			is >> vertices[i];
			normals[i] = 0.0f;
			float number = abs(vertices[i]);
			if (number > max)
				max = number;
		}

		Vector gravityCenter = Vector(0.0f, 0.0f, 0.0f);
		for (uint i = 0; i < nVertices * 3; i += 3) {
			vertices[i] /= max;
			vertices[i + 1] /= max;
			vertices[i + 2] /= max;
			gravityCenter += Vector(vertices[i], vertices[i + 1],
					vertices[i + 2]);
		}
		gravityCenter /= (float) nVertices;

		//once we have the gravity center we have to translate every point
		for (uint i = 0; i < nVertices * 3; i += 3) {
			vertices[i] -= gravityCenter[0];
			vertices[i + 1] -= gravityCenter[1];
			vertices[i + 2] -= gravityCenter[2];
		}

		//step 3: copying values to the indices array
		uint size;
		uint first, second, third;
		uint count = 0;
		while (!is.eof()) {
			is >> size >> first >> second;
			for (uint j = 2; j < size; ++j) {
				is >> third;
				indices[count++] = first;
				indices[count++] = second;
				indices[count++] = third;
				triangleNormal(first, second, third);

				second = third;
			}
		}
		nFaces = (count - 1) / 3;
		//closing the file
		is.close();

		//normalising normals before exiting
		normalizeNormals();

		return true;
	}
	return false;
}

void OFFfileReader::normalizeNormals() {
	for (uint i = 0; i < nVertices; ++i) {
		int index = i * 3;
		float mod = sqrt(
				normals[index] * normals[index]
						+ normals[index + 1] * normals[index + 1]
						+ normals[index + 2] * normals[index + 2]);
		normals[index] /= mod;
		normals[index + 1] /= mod;
		normals[index + 2] /= mod;
	}
}

void OFFfileReader::triangleNormal(uint index1, uint index2, uint index3) {
	Vector edge1 = Vector(vertices + index2 * 3)
			- Vector(vertices + index1 * 3);
	Vector edge2 = Vector(vertices + index3 * 3)
			- Vector(vertices + index2 * 3);
	Vector result = edge1.crossProduct(edge2);
	result.normalize();

	//first vertex
	normals[index1 * 3] += result[0];
	normals[index1 * 3 + 1] += result[1];
	normals[index1 * 3 + 2] += result[2];

	//second vertex
	normals[index2 * 3] += result[0];
	normals[index2 * 3 + 1] += result[1];
	normals[index2 * 3 + 2] += result[2];

	//third vertex
	normals[index3 * 3] += result[0];
	normals[index3 * 3 + 1] += result[1];
	normals[index3 * 3 + 2] += result[2];
}

void OFFfileReader::flipNormals() {
	for (uint i = 0; i < nVertices * 3; ++i) {
		normals[i] *= -1;
	}
}
