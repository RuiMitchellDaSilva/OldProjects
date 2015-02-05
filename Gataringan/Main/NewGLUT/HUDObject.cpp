#include "HUDObject.h"

HUDObject::HUDObject(Mesh* mesh, Texture2D* texture, Material* material, float x, float y, float z, float angleX, float angleY)
{
	_rotation = 0.1;
	yaw = 0;
	pitch = 0;
	_angleX = angleX;
	_angleY = angleY;
	_mesh = mesh;
	_texture = texture;
	_HUDPos = { x, y, z };

	_material = material;
}

void HUDObject::Update()
{
	_rotation += 5;

	if (_rotation >= 100)
	{
		_rotation = 0.1;
	}
		
}

void HUDObject::Draw()
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
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(_HUDPos.x, _HUDPos.y, _HUDPos.z);
		glRotatef(_angleY, 1, 0, 0);
		glRotatef(_angleX, 0, 1, 0);
		glRotatef(_rotation, 0, 0, -1);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
	glPopMatrix();

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(_HUDPos.x - 3.65, _HUDPos.y, _HUDPos.z);
		glRotatef(_angleY, 1, 0, 0);
		glRotatef(_angleX, 0, 1, 0);
		glRotatef(_rotation, 0, 0, -1);
		glCullFace(GL_BACK);
		glDrawElements(GL_TRIANGLES, _mesh->IndexCount, GL_UNSIGNED_SHORT, _mesh->Indices);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
