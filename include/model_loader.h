/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "structs.h"

class ModelLoader
{
public:
	std::vector<struct Vector3D> vertices;
	std::vector<struct Vector3D> normals;
	std::vector<struct face> faces;
	std::vector<unsigned int> vertindex;
	ModelLoader();

	~ModelLoader();

	void getvertex(char *c, char *n, FILE *f, std::vector<struct Vector3D> *vertex);
	void getnormal(char *c, char *n, FILE *f, std::vector<struct Vector3D> *normal);
	void getface(char *c, char *n, FILE *f, std::vector<struct face> *face, std::vector<unsigned int> *vertindex);
	void loadmodel(std::string name);
private:

};
#endif 
