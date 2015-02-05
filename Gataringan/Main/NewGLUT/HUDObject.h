#pragma once

//Glut and window includes
#include <Windows.h> //Required for open GL on windows
#include <gl/GL.h> //opengl
#include <gl/GLU.h> //opengl utilities
#include "GL\freeglut.h" //freeglu library

#include "Texture2D.h"
#include "MeshLoader.h"
#include "Structures.h"

class HUDObject
{
	private:
		Material* _material;
		Mesh* _mesh;
		Texture2D* _texture;

		float _angleX, _angleY;
		Vector3 _HUDPos;
		GLfloat _rotation;
	public:
		HUDObject(Mesh* mesh, Texture2D* texture, Material* material, float x, float y, float z, float angleX, float angleY);
		~HUDObject(void);

		void Update();
		void Draw();

		float yaw, pitch;
};