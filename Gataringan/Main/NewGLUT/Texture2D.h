#pragma once

#include "Structures.h"

class Texture2D
{
	private:
		int _width, _height;
		GLuint _ID; //This is the texture ID	
	public:
		Texture2D();
		~Texture2D();

		bool Load(char* path, int width, int height);
		GLuint GetID() const{ return _ID; }
		int GetWidth() const{ return _width; }
		int GetHeight() const { return _height;  }
};