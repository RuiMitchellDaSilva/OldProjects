#pragma once

#include "Texture2D.h"
#include "MeshLoader.h"
#include "SceneObject.h"
#include <sstream>

class MovableObject : public SceneObject
{
	private:
		Material* _material;
		Texture2D* _texture;

		float _boundary;
		Vector3 _position;
		GLfloat _zMovement;
		GLfloat _rotation;
		GLfloat _rotationValueX;
		GLfloat _rotationValueY;
		GLfloat _rotationValueZ;				
	public:
		MovableObject(Mesh* mesh, Texture2D* texture, Material* material, std::string name, float x, float y, float z);
		~MovableObject(void);

		void Update();
		void Draw();	
		void UpdatePosition(float x, float y, float z);
		void Rotate(float xRV, float yRV, float zRV, float r);
		void Collision(int objectNo);
		void SetRotation(float xRV, float yRV, float zRV, float r);
		float GetBoundary();
		Vector3 GetPosition();

		std::string _name;
};

