/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include <iostream>
#include <string>
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

void ModelLoader::getvertex(char *c, char *n, FILE *f, vector<struct Vector3D> *vertex)
{
	struct Vector3D vert;

	if (fscanf(f, "%f %f %f", &vert.x, &vert.y, &vert.z) <= 0)
		exerr("failed to scan vertices!");
	vertex->push_back(vert);
}

void ModelLoader::getnormal(char *c, char *n, FILE *f, vector<struct Vector3D> *normal)
{
	struct Vector3D vert;

	if (fscanf(f, "%f %f %f", &vert.x, &vert.y, &vert.z) <= 0)
		exerr("failed to scan normals!");
	normal->push_back(vert);
}

void ModelLoader::getface(char *c, char *n, FILE *f, vector<unsigned int> *vertindex, vector<unsigned int> *uvindex, vector<unsigned int> *normindex)
{
	struct face temp;
	char line[128]; /* Should accomodate very big models... */

	fgets(line, 128, f);
	if (sscanf(line, "%i %i %i", &temp.vertindex[0], &temp.vertindex[1], &temp.vertindex[2]) == 3) {
		std::fill(temp.uvindex, temp.uvindex + 3, 0);
		std::fill(temp.normindex, temp.normindex + 3, 0);
		/* f v1 v2 v3 */
	} else if (sscanf(line, "%i/%i %i/%i %i/%i",
				&temp.vertindex[0],&temp.uvindex[0],
				&temp.vertindex[1], &temp.uvindex[1],
				&temp.vertindex[2], & temp.uvindex[2]) == 6) {
		std::fill(temp.normindex, temp.normindex + 3, 0);
		/* f v1/vt1 v2/vt2 v3/vt3 */
	} else if (sscanf(line, "%i//%i %i//%i %i//%i",
				&temp.vertindex[0],&temp.normindex[0],
				&temp.vertindex[1], &temp.normindex[1],
				&temp.vertindex[2], & temp.normindex[2]) == 6) {
		std::fill(temp.uvindex, temp.uvindex + 3, 0);
		/* f v1//vn1 v2//vn2 v3//vn3 */
	} else if (sscanf(line, "%i/%i/%i %i/%i/%i %i/%i/%i",
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
	string line;
	FILE *file;
	char curr = 0;
	char next = 0;
	std::vector<struct Vector3D> temp_verts;
	std::vector<struct Vector3D> temp_uvs;
	std::vector<struct Vector3D> temp_norms;

	file =  fopen(name.c_str(), "r");
	if (file == NULL) {
		cout << "Unable to open file!\n" << endl;
		exit(EXIT_FAILURE);
	}

	/* Read in data */
	curr = fgetc(file);
	next = curr;
	next = fgetc(file);
	while (curr != EOF) {
		if (curr == 'v' && next == ' ') {
			getvertex(&curr, &next, file, &temp_verts);
		} else if (curr == 'v' && next == 'n') {
			getnormal(&curr, &next, file, &normals);
		} else if (curr == 'f') {
			getface(&curr, &next, file, &vertindices, &uvindices, &normindices);
		}
		curr = next;
		next = fgetc(file);
	}

	struct Vector3D point;
	unsigned int vertindex, normindex;
	for (size_t i = 0; i < vertindices.size(); i++) {
		vertindex = vertindices[i];
		point = temp_verts[vertindex];
		vertices.push_back(point);
	}

	cout << vertices.size() << " vertices\n" <<
	normals.size() << " normals\n" <<
	vertices.size()/3 << " Triangles (faces)" << endl;
}

