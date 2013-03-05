/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

class ModelLoader
{
public:



	ModelLoader();

	~ModelLoader();

	void countVertices(FILE *model, char *filename);
	int get_vertices() { return nVertices_;};
private:
	int nVertices_;
	float *polys_;
};
#endif 
