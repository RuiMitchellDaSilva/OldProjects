#pragma once

#include "MeshLoader.h"
#include "SceneObject.h"
#include "Texture2D.h"

class ProjectileObject : public SceneObject
{
	private:
		Material* _material;
		Texture2D* _texture;

		float _projSpeed;
		float _boundary;
		Vector3 _position;
		Vector3 _translate;
		GLfloat _zMovement;
		GLfloat _rotation;
		GLfloat _rotationValue;
			
	public:
		ProjectileObject(Mesh* mesh, Texture2D* texture, float x, float y, float z, float mX, float mY, float mZ, float projSpeed);
		~ProjectileObject(void);

		void Update();
		void Draw();		
		void UpdatePosition(float x, float y, float z);
		void Rotate(float xRV, float yRV, float zRV, float r);
		void Collision(int objectNo);
		float GetBoundary();
		Vector3 GetPosition();
};

