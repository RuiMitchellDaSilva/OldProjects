#include "GLUT.h"
#include "GLUTCallbacks.h"
#include "SceneObject.h"

SceneObject::SceneObject(Mesh* mesh)
{
	_mesh = mesh;
}