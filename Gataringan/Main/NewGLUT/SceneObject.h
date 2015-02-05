#pragma once

#include "structures.h"

class SceneObject
{
	protected:
		Mesh* _mesh;
	public:
		SceneObject(Mesh* mesh);
		virtual ~SceneObject(void) {}

		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void UpdatePosition(float x, float y, float z) = 0;
		virtual void Rotate(float xRV, float yRV, float zRV, float r) = 0;
		virtual void Collision(int objectNo) = 0;
		virtual float GetBoundary() = 0;
		virtual Vector3 GetPosition() = 0;
};