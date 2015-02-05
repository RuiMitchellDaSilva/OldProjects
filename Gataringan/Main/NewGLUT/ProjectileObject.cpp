#include "ProjectileObject.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

ProjectileObject::ProjectileObject(Mesh* mesh, Texture2D* texture, float x, float y, float z, float mX, float mY, float mZ, float projSpeed) : SceneObject(mesh)
{

	_mesh = mesh;
	_texture = texture;
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_translate.x = mX;
	_translate.y = mY;
	_translate.z = mZ;
	_projSpeed = projSpeed;
	_boundary = _mesh->BoundryRad;

	_material = new Material();
	_material->Ambient.x = 0.19; _material->Ambient.y = 0.19; _material->Ambient.z = 0.19; _material->Ambient.w = 1.0;
	_material->Diffuse.x = 0.50; _material->Diffuse.y = 0.50; _material->Diffuse.z = 0.50; _material->Diffuse.w = 1.0;
	_material->Specular.x = 0.50; _material->Specular.y = 0.50; _material->Specular.z = 0.50; _material->Specular.w = 1.0;
	_material->Shininess = 0.40;
}

ProjectileObject::~ProjectileObject(void){}

void ProjectileObject::Update()
{
	_rotation += 0.1;

	if (_rotation >= 10)
	{
		_rotation = 0.5;
	}		

}

void ProjectileObject::Draw()
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
		glRotatef(_rotation, _translate.x, _translate.y, _translate.z);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ProjectileObject::UpdatePosition(float x, float y, float z)
{
	_position.x += (_translate.x * _projSpeed);
	_position.y += (_translate.y * _projSpeed);
	_position.z += (_translate.z * _projSpeed);

	Update();
}

void ProjectileObject::Rotate(float xRV, float yRV, float zRV, float r){}
void  ProjectileObject::Collision(int objectNo){}

float ProjectileObject::GetBoundary(){ return _boundary; }
Vector3 ProjectileObject::GetPosition(){ return _position; }