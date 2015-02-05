#pragma once

//Glut and window includes
#include <Windows.h> //Required for open GL on windows
#include <gl/GL.h> //opengl
#include <gl/GLU.h> //opengl utilities
#include "GL\freeglut.h" //freeglu library

//Loaders, structures, etc.
#include "Camera.h"
#include "LinkedList.h"
#include "SceneGraph.h"
#include "MeshLoader.h"
#include "ObjectLoader.h"
#include "stdlib.h" //Needed for drawing strings
#include "Structures.h"
#include "Texture2D.h"

//Object classes
#include "MovableObject.h"
#include "SceneObject.h"
#include "TranslatingObject.h"
#include "StaticObject.h"
#include "RotatingObject.h"
#include "ProjectileObject.h"
#include "HUDObject.h"

//Definitions

//Arrays
#define CONTROLOBJ 1
#define REFRESHRATE 16
#define TOTALNO 100
#define GRIDLENGTH 100
//Constants
#define PI 3.14159265358979323846
#define PIHALF 1.57079632679489661923

class GLUT
{
	private:
		//Scene objects
		SceneObject* _objects[TOTALNO]; //All objects in background	
		SceneObject* _ground;
		SceneObject* _wall[4];

		//HUD objects
		HUDObject* _gun;

		//Cameras
		Camera* _cameraMain;
		Camera* _cameraFPS;
		Camera* _cameraDebug;

		//Light
		Vector4* _lightPosition;
		Lighting* _lightData;

		//Materials
		Material* _defaultMaterial;
		Material* _metalMaterial;
		Material* _bluePlasticMaterial;
		Material* _redMaterial;

		//Meshes
		Mesh* _projMesh;
		Mesh* _cubeMesh;

		//Textures
		Texture2D* _metal;
		Texture2D* _coreTexture;
		Texture2D* _shieldTexture;

		//Scene graphs
		SceneGraph* _tank;
		SceneGraph* _tank2;	
		SceneGraph* _cubeShip;
		TreeNode* _shipRoot;

		//Linked lists
		LinkedList* _projectiles;
							
		//booleans
		bool _key[256];
		bool _shiftDown;
		bool _mLeftDown;
		bool _mRightDown;
		bool _debugMode;
		bool _scoreTimerOn;
		bool _resetShipState = true;

		//integers
		int _viewportWidth = 0;
		int _viewportHeight = 0;
		int _shotIntervalSpeed = 20; //The set speed of firing
		int _shotInterval = 0; //Limit number of projectiles launched at once
		int _shipStateCounter = 0; //Used in the transition of states
		int _shipStateMultplier = 1; //Set the speed of the CubeShip's state transition
		int _camMode = 0; //Set camera mode
		int _score = 0;
		int _highscore = 0;

		//floats
		float _angle = 0.0;
		float _scoreTimer = 0.0;
		float _translationSpeed = 0.5;

		//constants	
		const float _rotationSpeed = PI / 180 * 0.2;
	public:
		GLUT(int argc, char* argv[]);
		~GLUT(void);

		//Initialisation
		void InitGL(int argc, char* argv[]);
		void LoadingScreen(float percent);
		void InitVariables();
		void InitObjects();
		void InitCubeShip(float x, float y, float z);
		void InitCameras();
		void InitLight();
		void InitMenu();
		void ObjectMenu(int value);		

		//Input
		void Keyboard(unsigned char key, int x, int y);
		void KeyboardUp(unsigned char key, int x, int y);
		void MouseMotion(int x, int y);
		void Mouse(int button, int state, int x, int y);

		//Update
		void Update();
		void Timer(int value);
		void ProjectileUpdate();
		boolean SceneGraphCollision(SceneObject* p, SceneGraph* cubeShip);
		boolean CalculateCollision(float r1, float r2, Vector3 p1, Vector3 p2);
		void ScoringUpdate();
		void CubeShipUpdate();

		//Display/Draw
		void Display();
		void Idle();
		void Reshape(int w, int h);

		//HUD
		void HUD();
		void Crosshair();
		void HandleString();
		void DrawString(const char * text, int length, int x, int y);
};