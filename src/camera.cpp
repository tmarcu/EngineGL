/*
 * EngineGL
 * Copyright (C) 2015 Tudor Marcu. All rights reserved.
 */

#include "camera.h"

Camera::~Camera()
{
}

/* Constructor being used at the moment. 
 * TODO: Change to a default one so user doens't have to initialize
 * the vectors for it, since they should always start out the same. 
 */
Camera::Camera(Vector3D p, Vector3D c, Vector3D u, float s, float ms)
{
	position.x = p.x;
	position.y = p.y;
	position.z = p.z;

	center.x = c.x;
	center.y = c.y;
	center.z = c.z;

	up.x = u.x;
	up.y = u.y;
	up.z = u.z;

	speed = s;
	movespeed = ms;
}

/* Get and set functions for various parts of the camera */
void Camera::SetLookSpeed(float newspeed)
{
	speed = newspeed;
}

void Camera::SetMoveSpeed(float newspeed)
{
	movespeed = newspeed;    
}

void Camera::SetPrevPosition(float x, float y)
{
	prev_x = x; 
	prev_y = y;
}

void Camera::SetCameraPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::SetCameraCenter(float x, float y, float z)
{
	center.x = x;
	center.y = y;
	center.z = z;
}

void Camera::SetCameraUpVector(float x, float y, float z)
{
	up.x = x;
	up.y = y;
	up.z = z;
}

/* Simply get the magnitude of a vector |v| */
float Camera::GetMagnitude(struct Vector3D q)
{
	return sqrt(q.x*q.x + q.y*q.y + q.z*q.z);/* + q.s*q.s); <-quaternions */
}

/* Normalize a vector.
 * Need this to keep our tranformation vectors from skewing.
 */
struct Vector3D Camera::Normalize(struct Vector3D vector)
{
	float magnitude = GetMagnitude(vector);
	vector.x /= magnitude;
	vector.y /= magnitude;
	vector.z /= magnitude;
	return vector;
}

void Camera::MoveCamera(int direction)
{
	switch(direction) {
	/* MOVE FORWARD, add the look vector to the position */
	case UP:
		position.x += center.x*movespeed;
		position.y += center.y*movespeed;
		position.z += center.z*movespeed;
		break;
	/* MOVE BACKWARD, subtract the look vector from the position */
	case DOWN:
		position.x -= center.x*movespeed;
		position.y -= center.y*movespeed;
		position.z -= center.z*movespeed;
		break;
	/* MOVE LEFT,  add the z look vector to the x pos, and then subtract the x vector from the z pos */
	case LEFT:
		position.x +=  center.z*movespeed;
		position.z -=  center.x*movespeed;
		break;
	/* MOVE RIGHT, subtract the z look vector from the x pos, and then add the x vector to the z pos */
	case RIGHT:
		position.x -=  center.z*movespeed;
		position.z +=  center.x*movespeed;
		break;
	}

}

/* 
 * Rotate the camera based on matrix from ~axis and angle~
 * [rotation matrix]*[view matrix] = [new view matrix]
 */
void Camera::RotateCamera(struct Vector3D view, float angle, float x, float y, float z)
{
	float sinA = sin(angle);
	float cosA = cos(angle);
	struct Vector3D rotatedView;

	/* Apply the transformation */
	rotatedView.x = (cosA + x*x*(1 - cosA)) * view.x;
	rotatedView.x += (x*y*(1 - cosA) - z*sinA) * view.y;
	rotatedView.x += (x*z*(1 - cosA) + y*sinA) * view.z;

	rotatedView.y  = (y*x*(1 - cosA) + z*sinA) * view.x;
	rotatedView.y += (cosA + y*y*(1 - cosA)) * view.y;
	rotatedView.y += (y*z*(1 - cosA) - x*sinA) * view.z;

	rotatedView.z  = (z*x*(1 - cosA) - y*sinA) * view.x;
	rotatedView.z += (z*y*(1 - cosA) + x*sinA) * view.y;
	rotatedView.z += (cosA + z*z*(1 - cosA)) * view.z;

	/* Center is the vector we want to use for our camera look */
	center.x = rotatedView.x;
	center.y = rotatedView.y;
	center.z = rotatedView.z;
	center = Normalize(center); /* Clear up skew */
}

void Camera::MouseMoved(int x, int y, int mwidth, int mheight)
{
	float rotationAboutX;
	float rotationAboutY;
	struct Vector3D axis = {0, 1, 0}; /* Our axis for the first rotation */

	/* Handle distance traveled from center, should give direction as well */
	x -= mwidth;
	y -= mheight;

	/* Get the rotation around the axes; divide (mult. by < 0) to get a smaller
	 * number otherwise the rotations are too drastic. (Current - last location)
	 */
	rotationAboutX = -(x - prev_x)*speed*.1;
	rotationAboutY = -(y - prev_y)*speed*.1;


	/* Store the current values as the old ones for the next call */
	prev_x = x;
	prev_y = y;

	/* Calculate based on X distance, only passing the UP vector */
	RotateCamera(center, rotationAboutX, axis.x, axis.y, axis.z);

	/* Set the new axis for transforming */
	axis.x = -center.z;
	axis.y = 0.0f;
	axis.z = center.x;
	axis = Normalize(axis);

	RotateCamera(center, rotationAboutY, axis.x, axis.y, axis.z);
}