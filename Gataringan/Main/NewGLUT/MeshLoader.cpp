#include "MeshLoader.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace MeshLoader
{
	void LoadVertices(ifstream& inFile, Mesh& mesh);
	void LoadTexCoords(ifstream& inFile, Mesh& mesh);
	void LoadNormals(ifstream& inFile, Mesh& mesh);
	void LoadIndices(ifstream& inFile, Mesh& mesh);

	void MeshLoader::LoadVertices(ifstream& inFile, Mesh& mesh)
	{
		//This first bit is to create a bounding sphere for the object
		float d1 = 0;
		float d2 = 0;

		inFile >> mesh.VertexCount;

		if (mesh.VertexCount > 0)
		{
			mesh.Vertices = new Vertex[mesh.VertexCount];

			for (int i = 0; i < mesh.VertexCount; i++)
			{
				inFile >> mesh.Vertices[i].x;
				inFile >> mesh.Vertices[i].y;
				inFile >> mesh.Vertices[i].z;

				//All these 'if' statements are used to create a bounding box around the object
				if (mesh.Vertices[i].x < d1)
				{
					d1 = mesh.Vertices[i].x;
				}					
				else if (mesh.Vertices[i].x > d2)
				{
					d2 = mesh.Vertices[i].x;
				}				

				if (mesh.Vertices[i].y < d1)
				{
					d1 = mesh.Vertices[i].y;
				}				
				else if (mesh.Vertices[i].y > d2)
				{
					d2 = mesh.Vertices[i].y;
				}
					
				if (mesh.Vertices[i].z < d1)
				{
					d1 = mesh.Vertices[i].z;
				}					
				else if (mesh.Vertices[i].z > d2)
				{
					d2 = mesh.Vertices[i].z;
				}
					
			}

			mesh.BoundryRad = (d2 - d1) / 2;
		}

	}

	void MeshLoader::LoadTexCoords(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.TexCoordCount;

		if (mesh.TexCoordCount > 0)
		{
			mesh.TexCoords = new TexCoord[mesh.TexCoordCount];

			for (int i = 0; i < mesh.TexCoordCount; i++)
			{
				inFile >> mesh.TexCoords[i].u;
				inFile >> mesh.TexCoords[i].v;
			}

		}

	}

	void MeshLoader::LoadNormals(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.NormalCount;

		if (mesh.NormalCount > 0)
		{
			mesh.Normals = new Vector3[mesh.NormalCount];

			for (int i = 0; i < mesh.NormalCount; i++)
			{
				inFile >> mesh.Normals[i].x;
				inFile >> mesh.Normals[i].y;
				inFile >> mesh.Normals[i].z;
			}

		}

	}

	void MeshLoader::LoadIndices(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.IndexCount;

		if (mesh.IndexCount > 0)
		{
			mesh.Indices = new GLushort[mesh.IndexCount];

			for (int i = 0; i < mesh.IndexCount; i++)
			{
				inFile >> mesh.Indices[i];
			}

		}

	}

	Mesh* MeshLoader::Load(char* path)
	{
		Mesh* mesh = new Mesh();

		ifstream inFile;

		inFile.open(path);

		if (!inFile.good())
		{
			cerr << "Can't open texture file " << path << endl;
			return nullptr;
		}

		LoadVertices(inFile, *mesh);
		LoadTexCoords(inFile, *mesh);
		LoadNormals(inFile, *mesh);	
		LoadIndices(inFile, *mesh);

		return mesh;
	}
}