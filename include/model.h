/*
 * EngineGL
 * Copyright (C) 2015 Tudor Marcu. All rights reserved.
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <GL/glu.h>
#include "structs.h"

class Model
{
public:
	std::vector<struct Vector3D> vertices;
	std::vector<struct Vector3D> uvs;
	std::vector<struct Vector3D> normals;
	std::vector<struct face> faces;
	std::vector<unsigned int> vertindices;
	std::vector<unsigned int> uvindices;
	std::vector<unsigned int> normindices;
	unsigned int vao;
	unsigned int vbvert;
	unsigned int vbnorm;
	bool isfilled;
	Model();

	~Model();

	void getvertex(char *str, std::vector<struct Vector3D> *vertex);
	void getuv(char *str, std::vector<struct Vector3D> *uv);
	void getnormal(char *str, std::vector<struct Vector3D> *normal);
	void getface(char *str, std::vector<unsigned int> *vertindex, std::vector<unsigned int> *uvindex, std::vector<unsigned int> *normindex);
	void loadmodel(std::string name);
private:

};
#endif 
