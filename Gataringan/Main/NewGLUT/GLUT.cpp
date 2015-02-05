#include "GLUT.h"
#include "GLUTCallbacks.h"

#include <iostream>
#include <sstream>
#include <cmath>

/*
ESC - quit

Mouse
Motion- Camera
Left Mouse Button - Shoot
Right Mouse Button - Menu



Game Mode Only

Movement
W - Foward
S - Backwards



Debug Mode Only

Movement
W - Foward
S - Backwards
A - Left
D - Right
Q - Up
E - Down

Move Cube Ship
I - Foward
K - Backwards
J - Left
L - Right
U - Up
O - Down

*/

GLUT::GLUT(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	InitVariables();
	InitGL(argc, argv);	
	InitObjects();
	InitMenu(); //Initialise menu for debug mode
	glutMainLoop();
}

GLUT::~GLUT(void){}


//Initialisation
void GLUT::InitGL(int argc, char* argv[]) //Initialise all glut functions and variables
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(700, 100);
	glutCreateWindow("GATARINGAN"); //Create window with name

	glutIgnoreKeyRepeat(1); // Auto repeat keystrokes shouln't be reported by the keyboard and special callbacks, 
	//unless zero is entered (then auto repeat keystrokes will be reported)

	glutDisplayFunc(GLUTCallbacks::Display);
	glutIdleFunc(GLUTCallbacks::Display);//double buffering
	glutReshapeFunc(GLUTCallbacks::Reshape);
	glutMouseFunc(GLUTCallbacks::Mouse);
	glutMotionFunc(GLUTCallbacks::MouseMotion); //These 2 glut functions are needed to return the mouse coordinates
	glutPassiveMotionFunc(GLUTCallbacks::MouseMotion);

	glutKeyboardFunc(GLUTCallbacks::Keyboard); //In conjunction with glutIgnoreKeyRepeat(), sets up keyboard callback for the application
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutIdleFunc(GLUTCallbacks::Idle);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, 0);

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Set the viewport to be the entire window
	glViewport(0, 0, 800, 800);
	// Set the default perspective.
	gluPerspective(45, 1, 0.1, 1000);
	// Get Back to the Modelview

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void GLUT::LoadingScreen(float percent)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0.2, 0.5, 0.6, 1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT, 0.0, 0.1, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glDisable(GL_CULL_FACE);
			glClear(GL_DEPTH_BUFFER_BIT);

			//Loading text
			std::string y;
			std::stringstream out1;
			out1 << "LOADING : ";
			out1 << percent;
			out1 << "%";
			y = out1.str();

			const char * text = y.data();
			int length = y.size();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
				glLoadIdentity();
				glOrtho(0.0, GLUT_SCREEN_WIDTH, 0.0, GLUT_SCREEN_HEIGHT, -1.0, 1.0);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
					glLoadIdentity();
					glPushAttrib(GL_DEPTH_TEST);
					glDisable(GL_DEPTH_TEST);

					glRasterPos2i(70, 120);

					for (int i = 0; i < length; i++)
						glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);


					glPopAttrib();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

			//outline for loading bar
			glBegin(GL_LINES);
			glVertex3f(60, 100, -0.1f);
			glVertex3f(140, 100, -0.1f);
			glVertex3f(140, 100, -0.1f);
			glVertex3f(140, 90, -0.1f);
			glVertex3f(140, 90, -0.1f);
			glVertex3f(60, 90, -0.1f);
			glVertex3f(60, 90, -0.1f);
			glVertex3f(60, 100, -0.1f);
			glEnd();

			//loading bar progress
			glBegin(GL_QUADS);
			glVertex3f(60, 100, -0.1f);
			glVertex3f(60 + (0.8 * percent), 100, -0.1f);
			glVertex3f(60 + (0.8 * percent), 90, -0.1f);
			glVertex3f(60, 90, -0.1f);
			glEnd();

			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFlush();
}

void GLUT::InitVariables()
{
	_shiftDown = false;
	_camMode = 0;
	_viewportWidth = 0;
	_viewportHeight = 0;
	_mLeftDown = false;
	_mRightDown = false;
}

void GLUT::InitObjects()
{
	LoadingScreen(0); //loading screen
	LoadingScreen(0);


	InitCameras();
	InitLight();
	

	//Meshes
	_cubeMesh = MeshLoader::Load("cube.txt");
	_projMesh = ObjectLoader::Load("small projectile.obj");
	Mesh* cuboidMesh = MeshLoader::Load("cuboidZ.txt");
	Mesh* planeMesh = MeshLoader::Load("plane.txt");
	Mesh* cylinderMesh = ObjectLoader::Load("cylinder.obj");
	Mesh* minigunMesh = ObjectLoader::Load("mini2v1.obj");											LoadingScreen(25);
	Mesh* largeGroundMesh = ObjectLoader::Load("largeGround.obj");									LoadingScreen(35);
	Mesh* groundMesh = ObjectLoader::Load("smallGround.obj");										LoadingScreen(45);
	Mesh* wallMesh = ObjectLoader::Load("wall.obj");

	
	//Textures
	_metal = new Texture2D(); _metal->Load("metal.raw", 512, 512);
	_shieldTexture = new Texture2D(); _shieldTexture->Load("shieldTexture.raw", 512, 512);
	_coreTexture = new Texture2D(); _coreTexture->Load("coreTexture.raw", 512, 512);
	Texture2D* stars = new Texture2D(); stars->Load("stars.raw", 512, 512);
	Texture2D* Penguin = new Texture2D(); Penguin->Load("Penguins.raw", 512, 512);
	Texture2D* dirt = new Texture2D(); dirt->Load("dirt.raw", 512, 512);
	Texture2D* background = new Texture2D(); background->Load("background.raw", 512, 512);
	Texture2D* wallTexture = new Texture2D(); wallTexture->Load("wallTexture.raw", 512, 512);		 LoadingScreen(46);
	
	//Materials
	_defaultMaterial = new Material();
	_defaultMaterial->Ambient.x = 0.0; _defaultMaterial->Ambient.y = 0.05; _defaultMaterial->Ambient.z = 0.05; _defaultMaterial->Ambient.w = 1.0;
	_defaultMaterial->Diffuse.x = 0.0; _defaultMaterial->Diffuse.y = 0.05; _defaultMaterial->Diffuse.z = 0.05; _defaultMaterial->Diffuse.w = 1.0;
	_defaultMaterial->Specular.x = 0.0; _defaultMaterial->Specular.y = 0.0; _defaultMaterial->Specular.z = 0.0; _defaultMaterial->Specular.w = 1.0;
	_defaultMaterial->Shininess = 100.0f;

	_metalMaterial = new Material();
	_metalMaterial->Ambient.x = 0.19; _metalMaterial->Ambient.y = 0.19; _metalMaterial->Ambient.z = 0.19; _metalMaterial->Ambient.w = 1.0;
	_metalMaterial->Diffuse.x = 0.50; _metalMaterial->Diffuse.y = 0.50; _metalMaterial->Diffuse.z = 0.50; _metalMaterial->Diffuse.w = 1.0;
	_metalMaterial->Specular.x = 0.50; _metalMaterial->Specular.y = 0.50; _metalMaterial->Specular.z = 0.50; _metalMaterial->Specular.w = 1.0;
	_metalMaterial->Shininess = 0.40;

	_bluePlasticMaterial = new Material();
	_bluePlasticMaterial->Ambient.x = 0.0; _bluePlasticMaterial->Ambient.y = 0.1; _bluePlasticMaterial->Ambient.z = 0.06; _bluePlasticMaterial->Ambient.w = 1.0;
	_bluePlasticMaterial->Diffuse.x = 0.0; _bluePlasticMaterial->Diffuse.y = 0.51; _bluePlasticMaterial->Diffuse.z = 0.51; _bluePlasticMaterial->Diffuse.w = 1.0;
	_bluePlasticMaterial->Specular.x = 0.5; _bluePlasticMaterial->Specular.y = 0.5; _bluePlasticMaterial->Specular.z = 0.5; _bluePlasticMaterial->Specular.w = 1.0;
	_bluePlasticMaterial->Shininess = 0.25;

	_redMaterial = new Material();
	_redMaterial->Ambient.x = 0.17; _redMaterial->Ambient.y = 0.01; _redMaterial->Ambient.z = 0.01; _redMaterial->Ambient.w = 1.0;
	_redMaterial->Diffuse.x = 0.61; _redMaterial->Diffuse.y = 0.04; _redMaterial->Diffuse.z = 0.04; _redMaterial->Diffuse.w = 1.0;
	_redMaterial->Specular.x = 0.73; _redMaterial->Specular.y = 0.63; _redMaterial->Specular.z = 0.63; _redMaterial->Specular.w = 1.0;
	_redMaterial->Shininess = 0.6;

	//All objects
	_objects[0] = new RotatingObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, 10, 7.5, 10);
	_objects[1] = new RotatingObject(_cubeMesh, _metal, _metalMaterial, 10, 7.5, -10);
	_objects[2] = new RotatingObject(_cubeMesh, _coreTexture, _redMaterial, -10, 7.5, -10);
	_objects[3] = new RotatingObject(_cubeMesh, dirt, _defaultMaterial, -10, 7.5, 10);								LoadingScreen(47);

	_objects[4] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, 100, 7, 100);
	_objects[5] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, 100, 7, -100);
	_objects[6] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, -100, 7, -100);
	_objects[7] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, -100, 7, 100);

	_objects[8] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, 10, 0, 10);
	_objects[9] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, 10, 0, -10);
	_objects[10] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, -10, 0, -10);
	_objects[11] = new StaticObject(cylinderMesh, wallTexture, _defaultMaterial, -10, 0, 10);

	for (int i = 12; i < TOTALNO; i++)
	{
		_objects[i] = new TranslatingObject(_cubeMesh, stars, _defaultMaterial, (((rand() % 5000) / 10.0f) - 20.0f), (((rand() % 5000) / 10.0f) - 10.0f), -(rand() % 1000));
	}

	LoadingScreen(48);


	_wall[0] = new MovableObject(wallMesh, wallTexture, _defaultMaterial, "wall " + 1, 100, 0, 0);
	_wall[1] = new MovableObject(wallMesh, wallTexture, _defaultMaterial, "wall " + 2, 0, 0, -100);
	_wall[1]->Rotate(0, 1, 0, 90);
	_wall[2] = new MovableObject(wallMesh, wallTexture, _defaultMaterial, "wall " + 3, -100, 0, 0);
	_wall[3] = new MovableObject(wallMesh, wallTexture, _defaultMaterial, "wall " + 4, 0, 0, 100);
	_wall[3]->Rotate(0, 1, 0, 90);


	_ground = new MovableObject(largeGroundMesh, dirt, _defaultMaterial, "ground", 0, -0.1, 0);


	_gun = new HUDObject(minigunMesh, _metal, _metalMaterial, 1.75, 0, 0.5, 180, 0);



	//Scene Graphs
	_tank = new SceneGraph();
	Mesh* b = ObjectLoader::Load("tank/tank_body.obj");
	_tank->InsertRootNode(_tank->NewNode(new MovableObject(b, _metal, _metalMaterial, "Tank chassis", 0, 2, 0)));	LoadingScreen(60);
	Mesh* t = ObjectLoader::Load("tank/tank_turret.obj");
	_tank->GetFront()->left = _tank->NewNode(new MovableObject(t, _metal, _metalMaterial, "Tank turret", 0, 4, 0)); LoadingScreen(70);
	Mesh* gL = ObjectLoader::Load("tank/tank_turret_gun_l.obj");
	_tank->GetFront()->left->left = _tank->NewNode(new MovableObject(gL, _metal, _metalMaterial, "Turret left gun", 2.8, 4, 0)); LoadingScreen(80);
	Mesh* gR = ObjectLoader::Load("tank/tank_turret_gun_r.obj");
	_tank->GetFront()->left->right = _tank->NewNode(new MovableObject(gR, _metal, _metalMaterial, "Turret right gun", -2.8, 4, 0)); LoadingScreen(90);



	_tank2 = new SceneGraph();
	Mesh* b2 = ObjectLoader::Load("tank/tank_body.obj");
	_tank2->InsertRootNode(_tank2->NewNode(new MovableObject(b2, _metal, _metalMaterial, "Tank chassis", 0, 2, 0)));
	Mesh* t2 = ObjectLoader::Load("tank/tank_turret.obj");
	_tank2->GetFront()->left = _tank2->NewNode(new MovableObject(t2, _metal, _metalMaterial, "Tank turret", 0, 2, 0));



	InitCubeShip(40, 10, 40);



	//Linked Lists
	_projectiles = new LinkedList();
	SceneObject* p = new ProjectileObject(_projMesh, _metal,
		100, -190, 100,
		0, 0, 0, 0);
	_projectiles->InsertNodeAtFront(_projectiles->NewNode(p));



	LoadingScreen(100);	
}

void GLUT::InitCubeShip(float x, float y, float z)
{
	_cubeShip = new SceneGraph();

	_cubeShip->InsertRootNode(_cubeShip->NewNode(new MovableObject(_cubeMesh, _coreTexture, _redMaterial, "Ship core", 0 + x, 0 + y, 0 + z)));//core


	_cubeShip->GetFront()->right = _cubeShip->NewNode(); //Top-Bottom Shield LINK


	_cubeShip->GetFront()->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _metal, _metalMaterial, "Ship bottom shield", 0 + x, -2 + y, 0 + z));
	_cubeShip->GetFront()->right->left->left = _cubeShip->NewNode(); //Shield 1 + 2 LINK
	_cubeShip->GetFront()->right->left->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "bottom shield 1", 1.0 + x, -3.5 + y, 1.0 + z));
	_cubeShip->GetFront()->right->left->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "bottom shield 2", 1.0 + x, -3.5 + y, -1.0 + z));
	_cubeShip->GetFront()->right->left->right = _cubeShip->NewNode(); //Shield 3 + 4 LINK
	_cubeShip->GetFront()->right->left->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "bottom shield 3", -1.0 + x, -3.5 + y, 1.0 + z));
	_cubeShip->GetFront()->right->left->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "bottom shield 4", -1.0 + x, -3.5 + y, -1.0 + z));

	_cubeShip->GetFront()->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _metal, _metalMaterial, "Ship top shield", 0 + x, 2 + y, 0 + z));
	_cubeShip->GetFront()->right->right->left = _cubeShip->NewNode(); //Shield 1 + 2 LINK
	_cubeShip->GetFront()->right->right->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "top shield 1", 1.0 + x, 3.5 + y, 1.0 + z));
	_cubeShip->GetFront()->right->right->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "top shield 2", 1.0 + x, 3.5 + y, -1.0 + z));
	_cubeShip->GetFront()->right->right->right = _cubeShip->NewNode(); //Shield 3 + 4 LINK
	_cubeShip->GetFront()->right->right->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "top shield 3", -1.0 + x, 3.5 + y, 1.0 + z));
	_cubeShip->GetFront()->right->right->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "top shield 4", -1.0 + x, 3.5 + y, -1.0 + z));


	_cubeShip->GetFront()->left = _cubeShip->NewNode(); //Side Shield LINK


	_cubeShip->GetFront()->left->left = _cubeShip->NewNode(); //Front-Back Shield LINK

	_cubeShip->GetFront()->left->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _metal, _metalMaterial, "Ship front shield", -2 + x, 0 + y, 0 + z));//structure + shield
	_cubeShip->GetFront()->left->left->left->left = _cubeShip->NewNode(); //Shield 1 + 2 LINK
	_cubeShip->GetFront()->left->left->left->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "front shield 1", -3.5 + x, 1.0 + y, 1.0 + z));
	_cubeShip->GetFront()->left->left->left->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "front shield 2", -3.5 + x, -1.0 + y, 1.0 + z));
	_cubeShip->GetFront()->left->left->left->right = _cubeShip->NewNode(); //Shield 3 + 4 LINK
	_cubeShip->GetFront()->left->left->left->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "front shield 3", -3.5 + x, 1.0 + y, -1.0 + z));
	_cubeShip->GetFront()->left->left->left->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "front shield 4", -3.5 + x, -1.0 + y, -1.0 + z));

	_cubeShip->GetFront()->left->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _metal, _metalMaterial, "Ship back shield", 2 + x, 0 + y, 0 + z));
	_cubeShip->GetFront()->left->left->right->left = _cubeShip->NewNode(); //Shield 1 + 2 LINK
	_cubeShip->GetFront()->left->left->right->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "back shield 1", 3.5 + x, 1.0 + y, 1.0 + z));
	_cubeShip->GetFront()->left->left->right->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "back shield 2", 3.5 + x, -1.0 + y, 1.0 + z));
	_cubeShip->GetFront()->left->left->right->right = _cubeShip->NewNode(); //Shield 3 + 4 LINK
	_cubeShip->GetFront()->left->left->right->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "back shield 3", 3.5 + x, 1.0 + y, -1.0 + z));
	_cubeShip->GetFront()->left->left->right->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "back shield 4", 3.5 + x, -1.0 + y, -1.0 + z));


	_cubeShip->GetFront()->left->right = _cubeShip->NewNode(); //Left-Right Shield LINK		

	_cubeShip->GetFront()->left->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _metal, _metalMaterial, "Ship left-side shield", 0 + x, 0 + y, -2 + z));
	_cubeShip->GetFront()->left->right->left->left = _cubeShip->NewNode(); //Shield 1 + 2 LINK
	_cubeShip->GetFront()->left->right->left->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "left-side shield 1", 1.0 + x, 1.0 + y, -3.5 + z));
	_cubeShip->GetFront()->left->right->left->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "left-side shield 2", 1.0 + x, -1.0 + y, -3.5 + z));
	_cubeShip->GetFront()->left->right->left->right = _cubeShip->NewNode(); //Shield 3 + 4 LINK
	_cubeShip->GetFront()->left->right->left->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "left-side shield 3", -1.0 + x, 1.0 + y, -3.5 + z));
	_cubeShip->GetFront()->left->right->left->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "left-side shield 4", -1.0 + x, -1.0 + y, -3.5 + z));

	_cubeShip->GetFront()->left->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _metal, _metalMaterial, "Ship right-side shield", 0 + x, 0 + y, 2 + z));
	_cubeShip->GetFront()->left->right->right->left = _cubeShip->NewNode(); //Shield 1 + 2 LINK
	_cubeShip->GetFront()->left->right->right->left->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "right-side shield 1", 1.0 + x, 1.0 + y, 3.5 + z));
	_cubeShip->GetFront()->left->right->right->left->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "right-side shield 2", 1.0 + x, -1.0 + y, 3.5 + z));
	_cubeShip->GetFront()->left->right->right->right = _cubeShip->NewNode(); //Shield 3 + 4 LINK
	_cubeShip->GetFront()->left->right->right->right->left = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "right-side shield 3", -1.0 + x, 1.0 + y, 3.5 + z));
	_cubeShip->GetFront()->left->right->right->right->right = _cubeShip->NewNode(new MovableObject(_cubeMesh, _shieldTexture, _bluePlasticMaterial, "right-side shield 4", -1.0 + x, -1.0 + y, 3.5 + z));

	_shipRoot = _cubeShip->GetFront();
}

void GLUT::InitCameras()
{
	_cameraFPS = new Camera(0.0, 6.0, 0.0, 0.0, 0.0);
	_cameraFPS->game = true;
	printf("Camera: %f %f %f Direction vector: %f %f %f\n", _cameraFPS->pos.x, _cameraFPS->pos.y, _cameraFPS->pos.z,
		_cameraFPS->lookAt.x, _cameraFPS->lookAt.y, _cameraFPS->lookAt.z);

	_cameraDebug = new Camera(0.0, 0.0, 0.0, 0.0, 0.0);

	_cameraMain = _cameraFPS;
}

void GLUT::InitLight()
{
	_lightPosition = new Vector4();
	_lightPosition->x = 0.0;
	_lightPosition->y = 10.0;
	_lightPosition->z = 1.0;
	_lightPosition->w = 0.0;

	_lightData = new Lighting();
	_lightData->Ambient.x = 1.0;
	_lightData->Ambient.y = 1.0;
	_lightData->Ambient.z = 1.0;
	_lightData->Ambient.w = 1.0;
	_lightData->Diffuse.x = 0.8;
	_lightData->Diffuse.y = 0.8;
	_lightData->Diffuse.z = 0.8;
	_lightData->Diffuse.w = 1.0;
	_lightData->Specular.x = 0.0;
	_lightData->Specular.y = 0.0;
	_lightData->Specular.z = 0.0;
	_lightData->Specular.w = 0.0;
}

void GLUT::InitMenu() //SORT OUT DIFFEENT MODES AND RESTRICTIONS
{
	int menu, submenu1, submenu2, submenu3;

	submenu1 = glutCreateMenu(GLUTCallbacks::objectMenu);
	glutAddMenuEntry("FPS", 0);
	glutAddMenuEntry("Debug mode", -1);

	submenu2 = glutCreateMenu(GLUTCallbacks::objectMenu);
	glutAddMenuEntry("100%", 1);
	glutAddMenuEntry("75%", 2);
	glutAddMenuEntry("50%", 3);
	glutAddMenuEntry("25%", 4);

	submenu3 = glutCreateMenu(GLUTCallbacks::objectMenu);
	glutAddMenuEntry("Positional", 5);
	glutAddMenuEntry("Directional", 6);
	glutAddMenuEntry("North", 7);
	glutAddMenuEntry("South", 8);
	glutAddMenuEntry("East", 9);
	glutAddMenuEntry("West", 10);

	menu = glutCreateMenu(GLUTCallbacks::objectMenu);
	glutAddSubMenu("Cameras", submenu1);
	glutAddSubMenu("Firing speed", submenu2);
	glutAddSubMenu("Lights", submenu3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GLUT::ObjectMenu(int value)
{

	switch (value)
	{
	case 0: //Set for FPS
		_camMode = 0;
		_cameraMain = _cameraFPS;
		_debugMode = false;
		break;

	case 1:
		_shotIntervalSpeed = 10;
		break;

	case 2:
		_shotIntervalSpeed = 20;
		break;

	case 3:
		_shotIntervalSpeed = 30;
		break;

	case 4:
		_shotIntervalSpeed = 40;
		break;

	case 5:
		_lightPosition->w = 1.0;
		break;

	case 6:
		_lightPosition->w = 0.0;
		break;

	case 7:
		_lightPosition->x = -100.0;
		_lightPosition->y = 100.0;
		_lightPosition->z = 0.0;
		break;

	case 8:
		_lightPosition->x = 100.0;
		_lightPosition->y = 100.0;
		_lightPosition->z = 0.0;
		break;

	case 9:
		_lightPosition->x = 0.0;
		_lightPosition->y = 100.0;
		_lightPosition->z = -100.0;
		break;

	case 10:
		_lightPosition->x = 0.0;
		_lightPosition->y = 100.0;
		_lightPosition->z = 100.0;
		break;

	default: //Set for Debug
		_camMode = 0;
		_cameraMain = _cameraDebug;
		_debugMode = true;
	}
}


//Input
void GLUT::Keyboard(unsigned char key, int x, int y)
{

	if (key == 27)// Esc key is assigned "27"
	{
		exit(0); //exit the program
	}
		
	if (_camMode <= 0)
	{
		glutSetCursor(GLUT_CURSOR_NONE);  //Hides the mouse pointer when in FPS mode
		glutWarpPointer(_viewportWidth / 2, _viewportHeight / 2);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}


	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	{
		_shiftDown = true;
	}
	else
	{
		_shiftDown = false;
	}

	_key[key] = true;

}

void GLUT::KeyboardUp(unsigned char key, int x, int y)
{
	_key[key] = false; //This is to allow the key to be held down without any percievable lag
}

void GLUT::MouseMotion(int x, int y)
{
	static bool warped = false; //stops glutWarpPointer from triggering callback event to mouse

	if (warped)
	{
		warped = false;
		return;
	}

	if (_camMode <= 0)
	{
		int dX = x - _viewportWidth / 2;
		int dY = y - _viewportHeight / 2;

		if (dX)
		{
			_cameraMain->RotateYaw(_rotationSpeed * dX);
		}

		if (dY)
		{
			_cameraMain->RotatePitch(_rotationSpeed * -dY);
		}

		glutWarpPointer(_viewportWidth / 2, _viewportHeight / 2);

		warped = true;
	}

}

void GLUT::Mouse(int button, int state, int x, int y)
{

	if (state == GLUT_DOWN) //if mouse butoon is clicked
	{

		if (button == GLUT_LEFT_BUTTON)
		{
			_mLeftDown = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			_mRightDown = true;
		}

	}
	else if (state == GLUT_UP)
	{

		if (button == GLUT_LEFT_BUTTON) //if mouse button is released
		{
			_mLeftDown = false;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			_mRightDown = false;
		}

	}

}


//Update
void GLUT::Update()
{
	//HUD Objects
	_gun->Update();

	//Scene objects
	for (int i = 0; i < TOTALNO; i++)
	{
		_objects[i]->Update();
	}

	for (int i = 0; i < TOTALNO; i++)
	{
		_objects[i]->UpdatePosition(-0.7f, -1.0f, 2.0f);
	}

	for (int i = 0; i < 4; i++)
		_wall[i]->Update();


	//Planes
	_ground->Update();


	//Projectiles
	ProjectileUpdate();


	//Scoring
	ScoringUpdate();


	//Model Tank
	TreeNode* tankRoot = _tank->GetFront();
	_tank->UpdateTree(tankRoot);


	//Player Tank Chassis
	TreeNode* tankRoot2 = _tank2->GetFront();
	_tank2->UpdateTree(tankRoot2);
	_angle = (atan(_cameraMain->lookAt.x) / PI) * 180;
	_tank2->RotateTree(tankRoot2, 0, 1, 0, _angle);


	//Cube Ship State
	CubeShipUpdate();
		

	_cameraMain->Update(); //update the main camera


	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));
}

void GLUT::Timer(int preferredRefresh) //Handles keyboard input
{

	if (_debugMode)
	{
	
		//No-Clip movement
		if (_key['w'] || _key['W'])
		{
			_cameraMain->Move(_translationSpeed);
		}
		else if (_key['s'] || _key['S'])
		{
			_cameraMain->Move(-_translationSpeed);
		}
		else if (_key['a'] || _key['A'])
		{
			_cameraMain->Strafe(_translationSpeed);
		}
		else if (_key['d'] || _key['D'])
		{
			_cameraMain->Strafe(-_translationSpeed);
		}
		else if (_key['e'] || _key['E'])
		{
			_cameraMain->Fly(-_translationSpeed);
		}
		else if (_key['q'] || _key['Q'])
		{
			_cameraMain->Fly(_translationSpeed);
		}

		//Move the Cube Ship
		if (_key['i'] || _key['I'])
		{
			_cubeShip->UpdatePositionOfTree(_shipRoot, 0, 0, -0.1);
		}
		else if (_key['k'] || _key['K'])
		{
			_cubeShip->UpdatePositionOfTree(_shipRoot, 0, 0, 0.1);
		}
		else if (_key['j'] || _key['J'])
		{
			_cubeShip->UpdatePositionOfTree(_shipRoot, -0.1, 0, 0);
		}
		else if (_key['l'] || _key['L'])
		{
			_cubeShip->UpdatePositionOfTree(_shipRoot, 0.1, 0, 0);
		}
		else if (_key['u'] || _key['U'])
		{
			_cubeShip->UpdatePositionOfTree(_shipRoot, 0, 0.1, 0);
		}
		else if (_key['o'] || _key['O'])
		{
			_cubeShip->UpdatePositionOfTree(_shipRoot, 0, -0.1, 0);
		}


	}
	else if (!_debugMode)
	{
		TreeNode* tankRoot2 = _tank2->GetFront();

		if (_key['w'] || _key['W'])
		{
			_cameraMain->Move(_translationSpeed);
			_tank2->UpdatePositionOfTree(tankRoot2, _cameraMain->lookAt.x / 2, 0, _cameraMain->lookAt.z / 2);
		}
		else if (_key['s'] || _key['S'])
		{
			_cameraMain->Move(-_translationSpeed);
			_tank2->UpdatePositionOfTree(tankRoot2, -_cameraMain->lookAt.x / 2, 0, -_cameraMain->lookAt.z / 2);
		}
	}


}

void  GLUT::ProjectileUpdate()
{
	
	if (_mLeftDown && _shotInterval == 0)
	{
		Vector3 t;
		t.x = _cameraMain->lookAt.x;
		t.y = _cameraMain->lookAt.y;
		t.z = _cameraMain->lookAt.z;

		SceneObject* p = new ProjectileObject(_projMesh, _metal,
			_cameraMain->pos.x + t.x, _cameraMain->pos.y + t.y, _cameraMain->pos.z + t.z,
			t.x / 5, t.y / 5, t.z / 5, 10);

		_projectiles->InsertNodeAtFront(_projectiles->NewNode(p));
	}
	
	_shotInterval++;

	if (_shotInterval >= _shotIntervalSpeed)
	{
		_shotInterval = 0;
	}


	Node* projHead = _projectiles->GetFront();

	_projectiles->UpdateAll(projHead);


	int i = 1;

	if (_projectiles->GetSize() > 10)
	{
		_projectiles->DeleteList();
	}	

	while (i <= _projectiles->GetSize()) //check each projectile
	{

		if (_projectiles->FindNode(i, i, projHead) != nullptr && (_projectiles->FindNode(i, i, projHead)->data) != nullptr)
		{

			if (CalculateCollision(_projectiles->FindNode(i, i, projHead)->data->GetBoundary(), 6.0f,
				_projectiles->FindNode(i, i, projHead)->data->GetPosition(), _cubeShip->GetFront()->data->GetPosition()))
			{

				if (SceneGraphCollision(_projectiles->FindNode(i, i, projHead)->data, _cubeShip))
				{
					(_projectiles->FindNode(i, i, projHead)->data) = nullptr;
					_projectiles->DeleteNode(_projectiles->FindNode(i - 1, i - 1, projHead));
					printf("%5d", _projectiles->CountNodes(projHead, 0));

					_scoreTimerOn = true;
				}

			}

		}

		i++;
	}

}

boolean GLUT::SceneGraphCollision(SceneObject* p, SceneGraph* cubeShip)
{
	//Ship Core
	if (CalculateCollision(cubeShip->GetFront()->data->GetBoundary(), p->GetBoundary(), cubeShip->GetFront()->data->GetPosition(),
		p->GetPosition()))
	{
		//Resets the ship
		cubeShip->DeleteTree(cubeShip->GetFront());
		InitCubeShip(40, 10, 40);
		_shipStateCounter = 0;

		if (_resetShipState)
		{
			_shipStateMultplier = 1;
			_resetShipState = false;
		}
		else
		{
			_shipStateMultplier++;
			_score += 1000;
		}

		return true;
	}

	//Ship Bottom Shield
	if (cubeShip->GetFront()->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->left->data->GetBoundary(), p->GetBoundary(),
		cubeShip->GetFront()->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->right);
		_score += 200;
		return true;
	}

	//Bottom Shield 1
	if (cubeShip->GetFront()->right->left != nullptr && cubeShip->GetFront()->right->left->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->left->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->left->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->left->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->right->left->left);
		_score += 50;
		return true;
	}

	//Bottom Shield 2
	if (cubeShip->GetFront()->right->left != nullptr && cubeShip->GetFront()->right->left->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->left->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->left->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->left->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->right->left->left);
		_score += 50;
		return true;
	}

	//Bottom Shield 3
	if (cubeShip->GetFront()->right->left != nullptr && cubeShip->GetFront()->right->left->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->left->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->left->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->left->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->right->left->right);
		_score += 50;
		return true;
	}

	//Bottom Shield 4
	if (cubeShip->GetFront()->right->left != nullptr && cubeShip->GetFront()->right->left->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->left->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->left->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->left->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->right->left->right);
		_score += 50;
		return true;
	}

	//Ship Top Shield
	if (cubeShip->GetFront()->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->right);
		_score += 200;
		return true;
	}

	//Top Shield 1
	if (cubeShip->GetFront()->right->right != nullptr && cubeShip->GetFront()->right->right->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->right->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->right->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->right->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->right->right->left);
		_score += 50;
		return true;
	}

	//Top Shield 2
	if (cubeShip->GetFront()->right->right != nullptr && cubeShip->GetFront()->right->right->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->right->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->right->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->right->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->right->right->left);
		_score += 50;
		return true;
	}

	//Top Shield 3
	if (cubeShip->GetFront()->right->right != nullptr && cubeShip->GetFront()->right->right->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->right->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->right->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->right->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->right->right->right);
		_score += 50;
		return true;
	}

	//Top Shield 4
	if (cubeShip->GetFront()->right->right != nullptr && cubeShip->GetFront()->right->right->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->right->right->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->right->right->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->right->right->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->right->right->right);
		_score += 50;
		return true;
	}

	//Ship Front Shield
	if (cubeShip->GetFront()->left->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->left);
		_score += 200;
		return true;
	}

	//Front Shield 1
	if (cubeShip->GetFront()->left->left->left != nullptr && cubeShip->GetFront()->left->left->left->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->left->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->left->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->left->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->left->left->left);
		_score += 50;
		return true;
	}

	//Front Shield 2
	if (cubeShip->GetFront()->left->left->left != nullptr && cubeShip->GetFront()->left->left->left->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->left->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->left->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->left->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->left->left->left);
		_score += 50;
		return true;
	}

	//Front Shield 3
	if (cubeShip->GetFront()->left->left->left != nullptr && cubeShip->GetFront()->left->left->left->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->left->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->left->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->left->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->left->left->right);
		_score += 50;
		return true;
	}

	//Front Shield 4
	if (cubeShip->GetFront()->left->left->left != nullptr && cubeShip->GetFront()->left->left->left->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->left->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->left->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->left->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->left->left->right);
		_score += 50;
		return true;
	}

	//Ship Back Shield
	if (cubeShip->GetFront()->left->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->left);
		_score += 200;
		return true;
	}

	//Back Shield 1
	if (cubeShip->GetFront()->left->left->right != nullptr && cubeShip->GetFront()->left->left->right->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->right->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->right->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->right->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->left->right->left);
		_score += 50;
		return true;
	}

	//Back Shield 2
	if (cubeShip->GetFront()->left->left->right != nullptr && cubeShip->GetFront()->left->left->right->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->right->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->right->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->right->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->left->right->left);
		_score += 50;
		return true;
	}

	//Back Shield 3
	if (cubeShip->GetFront()->left->left->right != nullptr && cubeShip->GetFront()->left->left->right->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->right->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->right->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->right->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->left->right->right);
		_score += 50;
		return true;
	}

	//Back Shield 4
	if (cubeShip->GetFront()->left->left->right != nullptr && cubeShip->GetFront()->left->left->right->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->left->right->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->left->right->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->left->right->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->left->right->right);
		_score += 50;
		return true;
	}

	//Ship Left Shield
	if (cubeShip->GetFront()->left->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->right);
		_score += 200;
		return true;
	}

	//Left Shield 1
	if (cubeShip->GetFront()->left->right->left != nullptr && cubeShip->GetFront()->left->right->left->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->left->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->left->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->left->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->right->left->left);
		_score += 50;
		return true;
	}

	//Left Shield 2
	if (cubeShip->GetFront()->left->right->left != nullptr && cubeShip->GetFront()->left->right->left->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->left->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->left->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->left->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->right->left->left);
		_score += 50;
		return true;
	}

	//Left Shield 3
	if (cubeShip->GetFront()->left->right->left != nullptr && cubeShip->GetFront()->left->right->left->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->left->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->left->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->left->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->right->left->right);
		_score += 50;
		return true;
	}

	//Left Shield 4
	if (cubeShip->GetFront()->left->right->left != nullptr && cubeShip->GetFront()->left->right->left->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->left->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->left->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->left->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->right->left->right);
		_score += 50;
		return true;
	}

	//Ship Right Shield
	if (cubeShip->GetFront()->left->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->right);
		_score += 200;
		return true;
	}


	//Right Shield 1
	if (cubeShip->GetFront()->left->right->right != nullptr && cubeShip->GetFront()->left->right->right->left->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->right->left->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->right->left->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->right->left->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->right->right->left);
		_score += 50;
		return true;
	}

	//Right Shield 2
	if (cubeShip->GetFront()->left->right->right != nullptr && cubeShip->GetFront()->left->right->right->left->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->right->left->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->right->left->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->right->left->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->right->right->left);
		_score += 50;
		return true;
	}


	//Right Shield 3
	if (cubeShip->GetFront()->left->right->right != nullptr && cubeShip->GetFront()->left->right->right->right->left != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->right->right->left->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->right->right->left->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->right->right->left);
		cubeShip->DeleteLeftNode(cubeShip->GetFront()->left->right->right->right);
		_score += 50;
		return true;
	}

	//Right Shield 4
	if (cubeShip->GetFront()->left->right->right != nullptr && cubeShip->GetFront()->left->right->right->right->right != nullptr &&
		CalculateCollision(cubeShip->GetFront()->left->right->right->right->right->data->GetBoundary(),
		p->GetBoundary(), cubeShip->GetFront()->left->right->right->right->right->data->GetPosition(), p->GetPosition()))
	{
		cubeShip->DeleteBranch(cubeShip->GetFront()->left->right->right->right->right);
		cubeShip->DeleteRightNode(cubeShip->GetFront()->left->right->right->right);
		_score += 50;
		return true;
	}

	return false;
}

boolean GLUT::CalculateCollision(float r1, float r2, Vector3 p1, Vector3 p2) //Create a "box" around each object and check for overlap
{

	if (((p1.x + r1) < (p2.x - r2)) || ((p1.x - r1) > (p2.x + r2)))
	{
		return false;
	}	

	if (((p1.y + r1) < (p2.y - r2)) || ((p1.y - r1) > (p2.y + r2)))
	{
		return false;
	}	

	if (((p1.z + r1) < (p2.z - r2)) || ((p1.z - r1) > (p2.z + r2)))
	{
		return false;
	}
		
	return true;
}

void GLUT::ScoringUpdate()
{

	if (_score > _highscore)
	{
		_highscore = _score;
	}

	if (_scoreTimer > 0)
	{
		_scoreTimer--;
	}
	else
	{
		_resetShipState = true;
		_score = 0;
	}

	if (_scoreTimerOn)
	{
		_scoreTimer = 660;
		_scoreTimerOn = false;
	}

}

void GLUT::CubeShipUpdate()
{

	if (!_debugMode)
	{
		_shipStateCounter += _shipStateMultplier;

		if (_shipStateCounter > 2000)
		{
			_shipStateCounter = 0;
		}

		if (_cubeShip != nullptr)
		{
			_cubeShip->UpdateTree(_shipRoot);

			if (_shipStateCounter < 500)
			{
				_cubeShip->UpdatePositionOfTree(_shipRoot, -0.2 * _shipStateMultplier, 0, 0);
			}
			else if (_shipStateCounter > 500 && _shipStateCounter < 1000)
			{
				_cubeShip->UpdatePositionOfTree(_shipRoot, 0, 0, -0.2 * _shipStateMultplier);
			}
			else if (_shipStateCounter > 1000 && _shipStateCounter < 1500)
			{
				_cubeShip->UpdatePositionOfTree(_shipRoot, 0.2 * _shipStateMultplier, 0, 0);
			}
			else if (_shipStateCounter > 1500)
			{
				_cubeShip->UpdatePositionOfTree(_shipRoot, 0, 0, 0.2 * _shipStateMultplier);
			}

		}
	}

}


//Display
void GLUT::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //This clears the scene so we start a fresh each time

	//background colour
	glClearColor(0.2, 0.5, 0.8, 1); //light blue


	//loaded objects

	for (int i = 0; i < 4; i++)
	{
		_wall[i]->Draw();
	}
		

	_ground->Draw();


	Node* projHead = _projectiles->GetFront();
	_projectiles->DrawAll(projHead);


	if (_cubeShip != nullptr)
	{
		_cubeShip->DrawTree(_shipRoot);
	}
		

	if (_debugMode)
	{

		for (int i = 0; i < TOTALNO; i++)
		{
			_objects[i]->Draw();
		}

		TreeNode* tankRoot = _tank->GetFront();
		_tank->DrawTree(tankRoot);
	}
	else
	{
		TreeNode* tankRoot2 = _tank2->GetFront();
		_tank2->DrawTree(tankRoot2);	
	}
	
	HUD();//Drawn last to always appera on-screen

	glFlush(); //Flushes the scene we just drew to the graphics card
	glutSwapBuffers();
}

void GLUT::Idle() //The program doesn't freeze (visually) when a different window is selected
{
	Display();
}

void GLUT::Reshape(int w, int h) //For when the screen reshapes
{
	_viewportWidth = w;
	_viewportHeight = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window spec
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 500); //set the perspective
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}


//HUD
void GLUT::HUD()
{
	_gun->Draw();
	Crosshair();
	HandleString();
}

void GLUT::Crosshair()
{
	//http://www.daniweb.com/software-development/cpp/threads/277785/text-in-opengl

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, GLUT_SCREEN_WIDTH, 0.0, GLUT_SCREEN_HEIGHT, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glPushAttrib(GL_DEPTH_TEST);
			glDisable(GL_DEPTH_TEST);

			glBegin(GL_LINES);
				glColor3f(1, 1, 1);
				glVertex3f(90, 100, -0.1f);
				glVertex3f(110, 100, -0.1f);
			glEnd();

			glBegin(GL_LINES);
				glColor3f(1, 1, 1);
				glVertex3f(100, 90, -0.1f);
				glVertex3f(100, 110, -0.1f);
			glEnd();

			glPopAttrib();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLUT::HandleString()
{
	std::string score;
	std::stringstream scoreOut;
	scoreOut << "Score : ";
	scoreOut << _score;
	score = scoreOut.str();
	DrawString(score.data(), score.size(), 140, 190);

	std::string highscore;
	std::stringstream highscoreOut;
	highscoreOut << "Highscore : ";
	highscoreOut << _highscore;
	highscore = highscoreOut.str();
	DrawString(highscore.data(), highscore.size(), 140, 195);

	std::string time;
	std::stringstream timeOut;
	timeOut << "Time : ";
	timeOut << (int) (_scoreTimer / 60);
	time = timeOut.str();
	DrawString(time.data(), time.size(), 140, 170);

	if (_debugMode)
	{
		std::string angle;
		std::stringstream angleOut;
		angleOut << "Angle : ";
		angleOut << _angle;
		angle = angleOut.str();
		DrawString(angle.data(), angle.size(), 140, 180);

		std::string y;
		std::stringstream out1;
		out1 << "Yaw : ";
		out1 << _cameraMain->yaw;
		y = out1.str();
		DrawString(y.data(), y.size(), 5, 150);

		std::string p;
		std::stringstream out2;
		out2 << "Pitch : ";
		out2 << _cameraMain->pitch;
		p = out2.str();
		DrawString(p.data(), p.size(), 5, 145);

		std::string c1;
		std::stringstream outc1;
		outc1 << "X : ";
		outc1 << _cameraMain->pos.x;
		c1 = outc1.str();
		DrawString(c1.data(), c1.size(), 5, 135);

		std::string c2;
		std::stringstream outc2;
		outc2 << "Y : ";
		outc2 << _cameraMain->pos.y;
		c2 = outc2.str();
		DrawString(c2.data(), c2.size(), 5, 130);

		std::string c3;
		std::stringstream outc3;
		outc3 << "Z : ";
		outc3 << _cameraMain->pos.z;
		c3 = outc3.str();
		DrawString(c3.data(), c3.size(), 5, 125);

		std::string s1;
		std::stringstream outs1;
		outs1 << "strafeLookX : ";
		outs1 << _cameraMain->strafeLookX;
		s1 = outs1.str();
		DrawString(s1.data(), s1.size(), 5, 115);

		std::string s2;
		std::stringstream outs2;
		outs2 << "strafeLookZ : ";
		outs2 << _cameraMain->strafeLookZ;
		s2 = outs2.str();
		DrawString(s2.data(), s2.size(), 5, 110);



		std::string gp1;
		std::stringstream outgp1;
		outgp1 << "g X : ";
		outgp1 << _cameraMain->lookAt.x;
		gp1 = outgp1.str();
		DrawString(gp1.data(), gp1.size(), 5, 80);

		std::string gp2;
		std::stringstream outgp2;
		outgp2 << "g Y : ";
		outgp2 << _cameraMain->lookAt.y;
		gp2 = outgp2.str();
		DrawString(gp2.data(), gp2.size(), 5, 75);

		std::string gp3;
		std::stringstream outgp3;
		outgp3 << "g Z : ";
		outgp3 << _cameraMain->lookAt.z;
		gp3 = outgp3.str();
		DrawString(gp3.data(), gp3.size(), 5, 70);



		std::string gyaw;
		std::stringstream outgyaw;
		outgyaw << "Yaw angle: ";
		outgyaw << tan(_cameraMain->yaw);
		gyaw = outgyaw.str();
		DrawString(gyaw.data(), gyaw.size(), 5, 60);

		std::string gpitch;
		std::stringstream outgpitch;
		outgpitch << "Pitch angle: ";
		outgpitch << tan(_cameraMain->pitch);
		gpitch = outgpitch.str();
		DrawString(gpitch.data(), gpitch.size(), 5, 55);
	}
	
}

void GLUT::DrawString(const char * text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, GLUT_SCREEN_WIDTH, 0.0, GLUT_SCREEN_HEIGHT, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glPushAttrib(GL_DEPTH_TEST);
			glDisable(GL_DEPTH_TEST);

			glRasterPos2i(x, y);
			glColor3f(1, 1, 1);

			for (int i = 0; i < length; i++)
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);

			glPopAttrib();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


