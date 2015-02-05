#include "Camera.h"

#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <gl/GL.h> 
#include <gl/GLU.h>
#include "GL\freeglut.h" 

#define PI 3.14159265358979323846 // <-- pi
#define PIHALF 1.57079632679489661923  // <-- pi/2

Camera::Camera(float x, float y, float z, float yw, float p)
{
	yaw = yw;
	pitch = p;

	SetPosition(x, y, z); //initial position
}

Camera::~Camera(void){}

void Camera::Update()
{
	lookAt.x = cos(yaw) * cos(pitch);
	lookAt.y = sin(pitch);
	lookAt.z = sin(yaw) * cos(pitch);

	strafeLookX = cos(yaw - PIHALF);
	strafeLookZ = sin(yaw - PIHALF);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
			pos.x + lookAt.x, pos.y + lookAt.y, pos.z + lookAt.z,
			0.0, 1.0, 0.0);
}

void Camera::Move(float i)
{
	float lookX = cos(yaw) * cos(pitch);
	float lookY = sin( pitch);
	float lookZ = sin(yaw) * cos(pitch);

	pos.x =  pos.x + i * lookX;
	pos.z =  pos.z + i * lookZ;

	if (!game)
	{
		pos.y = pos.y + i * lookY;
	}
		
	Update();
}

void Camera::Strafe(float i)
{
	 pos.x =  pos.x + i *  strafeLookX;
	 pos.z =  pos.z + i *  strafeLookZ;

	Update();
}

void Camera::Fly(float i)
{
	 pos.y =  pos.y + i;

	Update();
}

void Camera::RotateYaw(float angle)
{
	 yaw += angle;

	Update();
}

void Camera::RotatePitch(float angle)//looking left or right
{
	const float limit = 89.0 * PI / 180.0;//89 degrees converted into radians
	//(89 degrees so that camera smoothly when pitch reaches near 1 radian(+/-))

	 pitch += angle;

	 if (pitch < -limit)
	 {
		 pitch = -limit;
	 }
		 
	 if (pitch > limit)
	 {
		 pitch = limit;
	 }		 

	Update();
}


void Camera::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	Update();
}