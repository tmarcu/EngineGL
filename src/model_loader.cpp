/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include <iostream>
#include <string>
#include <cstring>
#include "model_loader.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

static void exerr(string msg)
{
	cout << msg << endl;
}

void ModelLoader::getvertex(char *str, vector<struct Vector3D> *vertex)
{
	struct Vector3D vert;

	if (sscanf(str, "v %f %f %f", &vert.x, &vert.y, &vert.z) != 3) {
		exerr("failed to scan vertices! Read:");
		exerr(str);
		exit(EXIT_FAILURE);
	}
	vertex->push_back(vert);
}

void ModelLoader::getuv(char *str, vector<struct Vector3D> *uv)
{
	struct Vector3D vert;

	if (sscanf(str, "vt %f %f", &vert.x, &vert.y) != 2) {
		exerr("failed to scan UVs! Read:");
		exerr(str);
		exit(EXIT_FAILURE);
	}
	vert.z = 0;
	uv->push_back(vert);
}

void ModelLoader::getnormal(char *str, vector<struct Vector3D> *normal)
{
	struct Vector3D vert;

	if (sscanf(str, "vn %f %f %f", &vert.x, &vert.y, &vert.z) <= 0) {
		exerr("failed to scan normals!");
		exit(EXIT_FAILURE);
	}
	normal->push_back(vert);
}

void ModelLoader::getface(char *str, vector<unsigned int> *vertindex, vector<unsigned int> *uvindex, vector<unsigned int> *normindex)
{
	struct face temp;

	if (sscanf(str, "f %i %i %i", &temp.vertindex[0], &temp.vertindex[1], &temp.vertindex[2]) == 3) {
		std::fill(temp.uvindex, temp.uvindex + 3, 0);
		std::fill(temp.normindex, temp.normindex + 3, 0);
		/* f v1 v2 v3 */
	} else if (sscanf(str, "f %i/%i %i/%i %i/%i",
				&temp.vertindex[0],&temp.uvindex[0],
				&temp.vertindex[1], &temp.uvindex[1],
				&temp.vertindex[2], & temp.uvindex[2]) == 6) {
		std::fill(temp.normindex, temp.normindex + 3, 0);
		/* f v1/vt1 v2/vt2 v3/vt3 */
	} else if (sscanf(str, "f %i//%i %i//%i %i//%i",
				&temp.vertindex[0],&temp.normindex[0],
				&temp.vertindex[1], &temp.normindex[1],
				&temp.vertindex[2], & temp.normindex[2]) == 6) {
		std::fill(temp.uvindex, temp.uvindex + 3, 0);
		/* f v1//vn1 v2//vn2 v3//vn3 */
	} else if (sscanf(str, "f %i/%i/%i %i/%i/%i %i/%i/%i",
				&temp.vertindex[0], &temp.uvindex[0], &temp.normindex[0],
				&temp.vertindex[1], &temp.uvindex[1], &temp.normindex[1],
				&temp.vertindex[2],	&temp.uvindex[2], &temp.normindex[2]) == 9) {
		/* f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 */
	}
	
	vertindex->push_back(temp.vertindex[0]);
	vertindex->push_back(temp.vertindex[1]);
	vertindex->push_back(temp.vertindex[2]);
	uvindex->push_back(temp.uvindex[0]);
	uvindex->push_back(temp.uvindex[1]);
	uvindex->push_back(temp.uvindex[2]);
	normindex->push_back(temp.normindex[0]);
	normindex->push_back(temp.normindex[1]);
	normindex->push_back(temp.normindex[2]);
}

void ModelLoader::loadmodel(string name)
{
	FILE *file;
	char line[128];
	std::vector<struct Vector3D> temp_verts;
	std::vector<struct Vector3D> temp_uvs;
	std::vector<struct Vector3D> temp_norms;

	file =  fopen(name.c_str(), "r");
	if (file == NULL) {
		cout << "Unable to open file!\n" << endl;
		exit(EXIT_FAILURE);
	}

	/* Read in data */
	while (fgets(line, 128, file) != NULL) {
		if (strncmp(line, "vt", 2) == 0) {
			getuv(line, &temp_uvs);
		} else if (strncmp(line, "vn", 2) == 0) {
			getnormal(line, &temp_norms);
		} else if (strncmp(line, "v", 1) == 0) {
			getvertex(line, &temp_verts);
		} else if (strncmp(line, "f", 1) == 0) {
			getface(line, &vertindices, &uvindices, &normindices);
		}
	}

	struct Vector3D point;
	unsigned int vertindex;
	for (size_t i = 0; i < vertindices.size(); i++) {
		vertindex = vertindices[i];
		point = temp_verts[vertindex - 1];
		vertices.push_back(point);
	}

	cout << vertices.size() << " vertices\n" <<
	temp_norms.size() << " normals\n" <<
	vertices.size()/3 << " Triangles (faces)" << endl;
}

