#pragma once

#include <Windows.h> //Required for open GL on windows
#include <gl/GL.h> //opengl
#include <gl/GLU.h> //opengl utilities
#include "GL\freeglut.h" //freeglu library

/*All the non-internal structures that
	had to be hardcoded*/

struct Vector3 
{
	float x, y, z;
};	

struct Vertex
{
	GLfloat x, y, z;
};

struct TexCoord
{
	GLfloat u, v;
};

struct Vector4
{
	GLfloat x, y, z, w; // the "w" variable i for Homogenous Coordinates
};

struct Lighting
{
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
};

struct Material
{
	Vector4 Ambient;//Colour when NOT illuminated
	Vector4 Diffuse;//Colour when illuminated
	Vector4 Specular;//Sets the colour for highlights
	GLfloat Shininess;//The shininess coeffiecient
};

struct Mesh
{
	Vertex* Vertices;
	Vector3* Normals;
	TexCoord* TexCoords;
	GLushort* Indices;
	float BoundryRad;//Used to define a bounding box around each object
	int VertexCount, NormalCount, TexCoordCount, IndexCount;
};