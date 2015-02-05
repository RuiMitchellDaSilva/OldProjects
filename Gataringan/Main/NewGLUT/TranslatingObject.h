#pragma once

#include "MeshLoader.h"
#include "SceneObject.h"
#include "Texture2D.h"

class TranslatingObject : public SceneObject
{
	private:
		Texture2D* _texture;
		Material* _material;
	
		float _boundary;
		Vector3 _position;
		GLfloat _zMovement;
		GLfloat _rotation;	
		GLfloat _rotationValue;		
	public:
		TranslatingObject(Mesh* mesh, Texture2D* texture, Material* material, float x, float y, float z);
		~TranslatingObject(void);

		void Draw();
		void Update();
		void UpdatePosition(float x, float y, float z);
		void Rotate(float xRV, float yRV, float zRV, float r);
		void Collision(int objectNo);
		float GetBoundary();
		Vector3 GetPosition();
};

