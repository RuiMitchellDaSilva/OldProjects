#include "RotatingObject.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

RotatingObject::RotatingObject(Mesh* mesh, Texture2D* texture, Material* material, float x, float y, float z) : SceneObject(mesh)
{
	_mesh = mesh;
	_texture = texture;
	_rotation = (float)(rand() % 100);
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_material = material;
}

RotatingObject::~RotatingObject(void){}

void RotatingObject::Update()
{
	_rotation += 1.0f;
	_rotationValue += 0.8f;
}

void RotatingObject::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, _mesh->Vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, _mesh->TexCoords);
	glNormalPointer(GL_FLOAT, 0, _mesh->Normals);

	glBindTexture(GL_TEXTURE_2D, _texture->GetID());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->Ambient.x));
	glMaterialf(GL_FRONT, GL_SHININESS, _material->Shininess);

	glPushMatrix();
		Update();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, _rotationValue, _rotationValue, _rotationValue);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void RotatingObject::UpdatePosition(float x, float y, float z){}
void RotatingObject::Rotate(float xRV, float yRV, float zRV, float r){}
void RotatingObject::Collision(int objectNo){}

float RotatingObject::GetBoundary(){ return _boundary; }
Vector3 RotatingObject::GetPosition(){ return _position; }