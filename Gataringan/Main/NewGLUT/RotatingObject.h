#pragma once

#include "Texture2D.h"
#include "MeshLoader.h"
#include "SceneObject.h"

class RotatingObject : public SceneObject
{
	private:
		Material* _material;
		Texture2D* _texture;

		float _boundary;
		Vector3 _position;
		GLfloat _zMovement;
		GLfloat _rotation;
		GLfloat _rotationValue;
	public:
		RotatingObject(Mesh* mesh, Texture2D* texture, Material* material, float x, float y, float z);
		~RotatingObject(void);

		void Update();
		void Draw();
		void UpdatePosition(float x, float y, float z);
		void Rotate(float xRV, float yRV, float zRV, float r);
		void Collision(int objectNo);
		float GetBoundary();
		Vector3 GetPosition();
};

