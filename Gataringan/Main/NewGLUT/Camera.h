#pragma once

//Glut and window includes
#include <Windows.h> 
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL\freeglut.h"

#include "Structures.h"

class Camera
{
	public:
		Camera(float x, float y, float z, float yw, float p);
		~Camera(void);

		void Update();
		void Move(float i); // i = increment
		void Strafe(float i);
		void Fly(float i);
		void RotateYaw(float angle);
		void RotatePitch(float angle);
		void SetPosition(float x, float y, float z);

		bool game = false;
		float yaw, pitch;//Yaw = Left and Right angle 
		//Pitch = Up and Down angle
		float strafeLookX, strafeLookZ; //for strafing foward, backwards, left and right, whilst keeping the camera angles consistent
		Vector3 pos; //cam position
		Vector3 lookAt; //what the camera is looking at
		Vector3 up; //A unary vector that points "up",
		//so the camera understands the "up" direction relevent to where the camera's looking	
}; 