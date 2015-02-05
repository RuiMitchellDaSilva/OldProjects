#define _CRT_SECURE_NO_DEPRECATE

#include "ObjectLoader.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace ObjectLoader
{
	void mapMesh(std::vector<unsigned short> & out_indices, std::vector<Vertex> & out_vertices, std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals, Mesh& mesh);
	bool is_near(float v1, float v2);
	bool getSimilarVertexIndex(Vertex &in_vertex, TexCoord &in_uv, Vector3 &in_normal, std::vector<Vertex> & out_vertices, std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals,
		unsigned short & result);
	void indexVBO_slow(std::vector<Vertex> & in_vertices, std::vector<TexCoord> & in_uvs, std::vector<Vector3> & in_normals, std::vector<unsigned short> & out_indices, std::vector<Vertex> & out_vertices,
		std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals);

	Mesh* ObjectLoader::Load(char* path)
	{
		Mesh* mesh = new Mesh();

		std::vector <Vertex> out_vertices;
		std::vector <TexCoord> out_uvs;
		std::vector <Vector3> out_normals;
		std::vector <GLushort> out_indices;
		std::vector < unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector <Vertex> temp_vertices;
		std::vector <TexCoord> temp_uvs;
		std::vector <Vector3> temp_normals;
		std::vector <GLushort> temp_indices;

		FILE * inFile = fopen(path, "r");

		if (inFile == NULL)
		{
			cout << "Can't open object file : " << path << endl;
			return nullptr;
		}

		while (1)
		{
			char lineH[128];
			int res = fscanf(inFile, "%s", lineH);
			if (res == EOF)
			{
				break;
			}				
			else
			{

				if ( strcmp (lineH, "v") == 0)
				{
					Vertex vertex;
					fscanf(inFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineH, "vt") == 0)
				{
					TexCoord uv;
					fscanf(inFile, "%f %f \n", &uv.u, &uv.v);
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineH, "vn") == 0)
				{
					Vector3 normal;
					fscanf(inFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineH, "f") == 0)
				{
					std::string vertex1, vertex2, vertex3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf(inFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					
					if (matches != 9)
					{
						printf("Cannot read file through this parser");
						return false;
					}

					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}


			}

		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			Vertex vertex = temp_vertices[vertexIndex - 1];
			out_vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < uvIndices.size(); i++)
		{
			unsigned int uvIndex = uvIndices[i];
			TexCoord uv = temp_uvs[uvIndex - 1];
			out_uvs.push_back(uv);
		}

		for (unsigned int i = 0; i < normalIndices.size(); i++)
		{
			unsigned int normalIndex = normalIndices[i];
			Vector3 normal = temp_normals[normalIndex - 1];
			out_normals.push_back(normal);
		}

							cout << "before indices" << endl;
							cout << "out_vertices.size()" << out_vertices.size() << endl;  //DEBUG
							cout << "out_uvs.size()" << out_uvs.size() << endl;
							cout << "out_normals.size()" << out_normals.size() << endl;
							cout << "out_indices.size()" << out_indices.size() << endl << endl;

		indexVBO_slow(out_vertices, out_uvs, out_normals,
						out_indices, out_vertices, out_uvs, out_normals);

		mapMesh(out_indices, out_vertices, out_uvs, out_normals, *mesh);

							cout << "after indices" << endl;
							cout << "out_vertices.size()" << out_vertices.size() << endl;  //DEBUG
							cout << "out_uvs.size()" << out_uvs.size() << endl;
							cout << "out_normals.size()" << out_normals.size() << endl;
							cout << "out_indices.size()" << out_indices.size() << endl << endl;

							cout << endl << "Successfully loaded : " << path << endl;

		return mesh;
	}

	void ObjectLoader::mapMesh(std::vector<unsigned short> & out_indices, std::vector<Vertex> & out_vertices, std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals, Mesh& mesh)
	{
		mesh.VertexCount = out_vertices.size();
		mesh.TexCoordCount = out_uvs.size();
		mesh.NormalCount = out_normals.size();
		mesh.IndexCount = out_indices.size();

		//Both of these floats are for the boundaries
		float d1 = 0; 
		float d2 = 0;

		if (mesh.VertexCount > 0)
		{
			mesh.Vertices = new Vertex[mesh.VertexCount];

			for (unsigned int i = 0; i < out_vertices.size(); i++)
			{
				mesh.Vertices[i] = out_vertices[i];

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


		if (mesh.TexCoordCount > 0)
		{
			mesh.TexCoords = new TexCoord[mesh.TexCoordCount];

			for (unsigned int i = 0; i < out_uvs.size(); i++)
			{
				mesh.TexCoords[i] = out_uvs[i];
			}

		}

		if (mesh.NormalCount > 0)
		{
			mesh.Normals = new Vector3[mesh.NormalCount];

			for (unsigned int i = 0; i < out_normals.size(); i++)
			{
				mesh.Normals[i] = out_normals[i];
			}

		}

		if (mesh.IndexCount > 0)
		{
			mesh.Indices = new GLushort[mesh.IndexCount];

			for (unsigned int i = 0; i < out_indices.size(); i++)
			{
				mesh.Indices[i] = out_indices[i];
			}

		}
	}

	// Returns true if v1 can be considered equal to v2
	bool ObjectLoader::is_near(float v1, float v2)
	{
		return fabs(v1 - v2) < 0.01f;
	}

	// Searches through all already-exported vertices
	// for a similar one.
	// Similar = same position + same UVs + same normal
	bool ObjectLoader::getSimilarVertexIndex(Vertex &in_vertex, TexCoord &in_uv, Vector3 &in_normal, std::vector<Vertex> & out_vertices, std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals,
								unsigned short & result)
	{
		// Lame linear search
		for (unsigned int i = 0; i<out_vertices.size(); i++)
		{

			if (
				is_near(in_vertex.x, out_vertices[i].x) &&
				is_near(in_vertex.y, out_vertices[i].y) &&
				is_near(in_vertex.z, out_vertices[i].z) &&
				is_near(in_uv.u, out_uvs[i].u) &&
				is_near(in_uv.v, out_uvs[i].v) &&
				is_near(in_normal.x, out_normals[i].x) &&
				is_near(in_normal.y, out_normals[i].y) &&
				is_near(in_normal.z, out_normals[i].z)
				)
			{
				result = i;
				return true;
			}

		}
		// No other vertex could be used instead
		// Looks like we'll have to add to the VBO
		return false;
	}

	void ObjectLoader::indexVBO_slow(std::vector<Vertex> & in_vertices, std::vector<TexCoord> & in_uvs, std::vector<Vector3> & in_normals, std::vector<unsigned short> & out_indices, std::vector<Vertex> & out_vertices,
							std::vector<TexCoord> & out_uvs, std::vector<Vector3> & out_normals)
	{
		// For each input vertex
		for (unsigned int i = 0; i<in_vertices.size(); i++)
		{

			// Try to find a similar vertex in out_XXXX
			unsigned short index;
			bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

			if (found)
			{ // A similar vertex is already in the VBO, use it instead
				out_indices.push_back(index); //out_indices is the index array you'll wanna use for mesh
			}
			else
			{ // If not, needs to be added in the output data
				out_vertices.push_back(in_vertices[i]);
				out_uvs.push_back(in_uvs[i]);
				out_normals.push_back(in_normals[i]);
				out_indices.push_back((unsigned short)out_vertices.size() - 1);
			}
		}
	}

}
