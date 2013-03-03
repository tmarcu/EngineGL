/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#ifndef STRUCTS_H
#define STRUCTS_H

/* Screen data we want to incorporate */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define HALF_WIDTH SCREEN_WIDTH/2
#define HALF_HEIGHT SCREEN_HEIGHT/2
#define SCREEN_BPP     32

struct Vector3D {
	float x, y, z;
};
struct Polygon {
	Vector3D vertex[3];
};

#endif

