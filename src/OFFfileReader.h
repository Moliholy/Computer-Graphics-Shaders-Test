/*
 * OOFfileReader.h
 *
 *  Created on: 06/02/2014
 *      Author: MOLINA
 */

#ifndef OFFFILEREADER_H_
#define OFFFILEREADER_H_

#define DEFAULT_BUFFER_SIZE 4096 * 32

typedef unsigned int uint;

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Vector.h"

using namespace std;

/**
 * @brief This class is able to open and read OFF files,
 * extract the relevant information and process it
 */
class OFFfileReader {

private:
	uint nVertices;
	uint nFaces;
	uint nEdges;

	float* vertices;
	float* normals;
	uint indices[DEFAULT_BUFFER_SIZE];

private:
	void normalizeNormals();
	void triangleNormal(uint index1, uint index2, uint index3);

public:
	OFFfileReader();
	virtual ~OFFfileReader();

	bool open(string filename);
	void flipNormals();

	const uint* getIndices() const {
		return indices;
	}

	uint getNumberOfEdges() const {
		return nEdges;
	}

	uint getNumberOfFaces() const {
		return nFaces;
	}

	uint getNumberOfVertices() const {
		return nVertices;
	}

	float* getVertices() const {
		return vertices;
	}

	float* getNormals() const {
		return normals;
	}
};

#endif /* OFFFILEREADER_H_ */
