#include "Circle.h"
#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

//Ghost array
#define ENEMYCOUNT 10

//Munchie array
#define ITEMCOUNT 10

//Bullet array
#define FIREBALLCOUNT 50

//Highscore array
#define SCORECOUNT 10

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Player
{
	//All variables for the chassis of the tank
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;


	//Turret variables
	float turretRot;//Turret angle
	int count;
	int shotTime;
	Rect* turretRect;
	SoundEffect* gunFire;
	Texture2D* turretTxtr;
	Vector2* turretOri;//Turret point origin (pivot point)
	Vector2* turretPos;

	//Variables regarding the whole tank
	boolean dead;
	boolean speedUpAni;
	int itemsCollect;
	int highScore;
	int combo;//Combo multiplier
	int comboTime;
	SoundEffect* ouch;
	Vector2* position;
};

struct Item
{
	Vector2* position;
	Rect* rect;
	Texture2D* texture;
	SoundEffect* pop;
	int frameCount;
	int currentFrameTime;
	int frameTime;
};

struct MovingEnemy
{
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	SoundEffect* death;
	boolean dead;
	boolean deathAni;
	int frameCount;
	int pattern;
	int currentFrameTime;
	int direction;
};

struct Projectile
{	
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	boolean impact;
	float dirX, dirY;
	bool fire;
	bool gone;
	float speed;
};

// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	//Methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mState);
	void CheckStart(Input::KeyboardState* state, Input::MouseState* mState, int elapsedTime);
	void CheckPaused(Input::KeyboardState* state, Input::MouseState* mState, int elapsedTime);
	void CheckLeftClick(Input::MouseState* mState, Input::ButtonState leftclick);
	void CheckViewportCollision(int elapsedTime);
	boolean CheckCollisionSquare(int top1, int bottom1, int rightSide1, int leftSide1,
	int top2, int bottom2, int rightSide2, int leftSide2);
	void CheckCollision(Item* munchies, int elapsedTime);
	void SetBoundaries();
	void UpdatePlayer(int elapsedTime, Input::MouseState* mState);
	float AngleCalc(Input::MouseState* mState);
	void UpdateItems(Item* items, int elapsedTime);
	void UpdateX2(Item* x2, int elapsedTime);
	void UpdateEnemies(MovingEnemy* enemy, int elapsedTime);
	void CheckEnemyCollisions();
	void CheckFireEnemyImpact(int e);
	void CheckProjState(Projectile* fireball, int elapsedTime, Input::MouseState* mState);
	void levelChange();

	//Data to represent the Player Tank
	Player* _player;

	//Data to represent collectables
	Item* _supplies[ITEMCOUNT]; //collect these to finish level
	Item* _x2; //allows for a combo

	//Enemy
	MovingEnemy* _tank[ENEMYCOUNT];

	//Projectile
	Projectile* _fireball[FIREBALLCOUNT];

	//Scoring
	Vector2* _scores[SCORECOUNT];
	int _scoreNo[SCORECOUNT];
	boolean _highScore;
	Vector2* _highScorePos;
	Vector2* _comboPos;
	Vector2* _comboTimePos;
	boolean _countDown;
	Vector2* _itemsLeft;

	//Constant data for Game Variables
    const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cGhostFrameTime;

	//Start screen
	boolean _tutorial;
	Texture2D* _startScreen;
	Rect* _startRect;

	//Collision
	boolean _oneBump1; //This is so the sound effect occurs only once
	boolean _oneBump2;
	SoundEffect* _bump;

	//Levels
	float _enemySpeed;
	int _level;
	float _boundaryX;
	float _boundaryY;
	Texture2D* _background;
	Texture2D* _hUD;
	Rect* _boundaries[5];
	Rect* _backgroundRect;

	//Pause screen
	int _pauseFlash;
	Rect* _pauseRect;
	Texture2D* _pause;
	Vector2* _pausePos;

	//Win screen
	Texture2D* _win;
	Rect* _winRect;

	//Game over screen
	Texture2D* _gameOver;
	Rect* _gameOverRect;

	//Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPos;
	bool _paused;
	bool _pKeyDown;
	bool _start;
	bool _startStop;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};