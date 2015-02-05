#include "Texture2D.h"

#include <iostream>
#include <fstream>

using namespace std;

Texture2D::Texture2D(){}
Texture2D::~Texture2D(){}

bool Texture2D::Load(char* path, int width, int height)
{
	char* tempTextureData;
	int fileSize;
	ifstream inFile;

	_width = width;
	_height = height;

	inFile.open(path, ios::binary);

	if (!inFile.good())
	{
		cerr << "Unable to open texture file" << path << endl;
		return false;
	}

	inFile.seekg(0, ios::end); //Seeks for the end of file being read
	fileSize = (int)inFile.tellg(); //Get current position in file till the end, giving the total file size
	tempTextureData = new char[fileSize]; //create a new array to store data
	inFile.seekg(0, ios::beg); //Seek back to beginning of file
	inFile.read(tempTextureData, fileSize); //Read all data within file in one go
	inFile.close(); //Close the file

	cout << path << " texture file has successfully loaded" << endl;

	glGenTextures(1, &_ID); //Get next Texture ID
	glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	delete[] tempTextureData; //Clear up the data as we don't need this any more
	return true;
}
