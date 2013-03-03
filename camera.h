/*
 * EngineGL
 * Copyright (C) 2013 Tudor Marcu. All rights reserved.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "structs.h"

#define NORM_TOL 0.000001f
#define DEG_TO_RAD 0.0174532925f
#define RAD_TO_DEG 57.295779513f
#define RIGHT   1
#define LEFT    2
#define UP      3
#define DOWN    4

class Camera
{
public:    
	/* Constructor and destructor */
	Camera(Vector3D, Vector3D, Vector3D, float, float);
	~Camera();

	/* Helpful math calculations */
	struct Vector3D Normalize(struct Vector3D);
	float GetMagnitude(struct Vector3D);

	/* Functions to control camera movement and look */
	void MoveCamera(int);
	void MouseMoved(int, int, int, int);
	void RotateCamera(Vector3D, float, float, float, float);

	float GetLookSpeed() { return speed;}
	void SetLookSpeed(float);

	float GetMoveSpeed() { return movespeed;}
	void SetMoveSpeed(float);

	void SetPrevPosition(float, float);

	struct Vector3D GetCameraPosition() { return position;}
	void SetCameraPosition(float, float, float);
	struct Vector3D GetCameraCenter() { return center;}
	void SetCameraCenter(float, float, float);
	struct Vector3D GetCameraUpVector() {return up;}
	void SetCameraUpVector(float, float, float);

private:
	int xMouseDiff;
	int yMouseDiff;
	float angle;
	float movespeed;
	float speed;
	float prev_x, prev_y;
	struct Vector3D position, center, up;
};
#endif
