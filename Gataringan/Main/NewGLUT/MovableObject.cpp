#include "MovableObject.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

MovableObject::MovableObject(Mesh* mesh, Texture2D* texture, Material* material, std::string name, float x, float y, float z) : SceneObject(mesh)
{
	_mesh = mesh;
	_texture = texture;
	_name = name;
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_boundary = _mesh->BoundryRad;

	_material = material;

	std::cout << _name << " loaded" << endl;
}

MovableObject::~MovableObject(void){}

void MovableObject::Update()
{
	_rotationValueX += 0.00001f;

	if (_rotationValueX >= 0.0001)
	{
		_rotationValueX -= 0.0001;
	}		

}

void MovableObject::Draw()
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
		glRotatef(_rotation, _rotationValueX, _rotationValueY, _rotationValueZ);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void MovableObject::UpdatePosition(float x, float y, float z)
{
	_position.x += x;
	_position.y += y;
	_position.z += z;

	Update();
}

void MovableObject::Rotate(float xRV, float yRV, float zRV, float r)
{
		_rotationValueX = xRV;
		_rotationValueY = yRV;
		_rotationValueZ = zRV;
		_rotation += r;

		Update();
}

void MovableObject::Collision(int objectNo)
{
	switch (objectNo)
	{
		case 1 :
			_position.x = 0;
			_position.y = 0;
			_position.z = 0;
			break;
		default:
			break;
	}
		
	Update();
}

void MovableObject::SetRotation(float xRV, float yRV, float zRV, float r)
{
	_rotationValueX = xRV;
	_rotationValueY = yRV;
	_rotationValueZ = zRV;
	_rotation = r;

	Update();
}

float MovableObject::GetBoundary(){ return _boundary; }
Vector3 MovableObject::GetPosition(){ return _position; }


