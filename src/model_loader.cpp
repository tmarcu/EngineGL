/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#include "model_loader.h"

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::countVertices(FILE *model, char *filename)
{
	char c;

	if (!(model = fopen(filename, "r"))) {
		printf("Could not open model file.");
		exit(0);
	}

	nVertices_ = 0;
	while (fscanf(model, "%c", &c) != EOF) {
		if(c == 'v')
		nVertices_++;
	}

	fclose(model);
}