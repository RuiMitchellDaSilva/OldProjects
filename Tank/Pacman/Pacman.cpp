#include "Pacman.h"

#include <sstream>
#include <time.h>
#include <iostream>
#include <math.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f),
_cPacmanFrameTime(125), _cGhostFrameTime(750)
{
		srand(time(NULL));

		_player = new Player();
		_player->frame = 0;
		_player->direction = 0;
		_player->currentFrameTime = 0;	
		_player->speedMultiplier = 1.0f;
		_player->highScore = 0;
		_player->combo = 1;
		_player->comboTime = 0;
		_player->itemsCollect = ITEMCOUNT;
		_player->dead = false;
		_player->speedUpAni = false;
		_player->turretRot = 45.0;
		_player->count = 0; //This is for firing the projectiles
		_player->shotTime = 0;//Limits firing speed

		for(int i = 0; i < ITEMCOUNT; i++)
		{
			_supplies[i] = new Item();
			_supplies[i]->currentFrameTime = 0; 
			_supplies[i]->frameCount = rand() % 1;
			_supplies[i]->frameTime = 0;
		}

		_x2 = new Item();
		_x2->currentFrameTime = 0;
		_x2->frameCount = 0;
		_x2->frameTime = 0;

		for (int i = 0; i < ENEMYCOUNT; i++)
		{
			_tank[i] = new MovingEnemy();
			_tank[i]->dead = false;
			_tank[i]->direction = 0;
			_tank[i]->deathAni = false;
			_tank[i]->currentFrameTime = 0;
			_tank[i]->frameCount = 0;
		}

		for (int i = 0; i < FIREBALLCOUNT; i++)
		{
			_fireball[i] = new Projectile();
			_fireball[i]->impact = false;
			_fireball[i]->gone = false;
			_fireball[i]->fire = false;
			_fireball[i]->speed = 1 * (100 / FIREBALLCOUNT);
		}

		for (int i = 0; i < SCORECOUNT; i++)
			_scoreNo[i] = 0;
		

		_level = 1;
		_enemySpeed = 0.01f;
		SetBoundaries();

		_paused = false;
		_pKeyDown = false;
		_start = true;
		_startStop = false;
		_highScore = true;
		_tutorial = true;//The tutorial screen will only appear upon loading the first level at the beginning

		//Initialize sound
		Audio::Initialise();

		//Initialise important Game aspects
		Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
		Input::Initialise();

		// Start the Game Loop - This calls Update and Draw in game loop
		Graphics::StartGameLoop();
	

}


Pacman::~Pacman()
{
	delete _player->texture;
	delete _player->turretTxtr;
	delete _player->sourceRect;
	delete _player->position;
	delete _player->turretPos;
	delete _player;

	for (int i = 0; i < ITEMCOUNT; i++)
	{
		delete _supplies[i]->texture;
		delete _supplies[i]->rect;
	}

	delete [] _supplies;

	delete _x2->texture;
	delete _x2->rect;
	delete _x2;

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		delete _tank[i]->texture;
		delete _tank[i]->sourceRect;
		delete _tank[i]->position;
	}

	delete [] _tank;

	for (int i = 0; i < FIREBALLCOUNT; i++)
	{
		delete _fireball[i]->texture;
		delete _fireball[i]->sourceRect;
		delete _fireball[i]->position;
	}

	delete[] _fireball;
}

void Pacman::LoadContent()
{


	// Load Player Tank
	_player->texture = new Texture2D();
	_player->texture->Load("Textures/Player.png", false);
	_player->position = new Vector2(512.0f, 382.0f);
	_player->turretPos = new Vector2(_player->position->X + 15, _player->position->Y + 20);
	_player->turretOri = new Vector2(6.0, 6.0);
	_player->sourceRect = new Rect(0.0f, 0.0f, 40, 40);
	_player->turretRect = new Rect(373.0f, 207.0f, 23, 10);
	_player->ouch = new SoundEffect();
	_player->ouch->Load("Sounds/Dead.wav");
	_player->gunFire = new SoundEffect();
	_player->gunFire->Load("Sounds/Shot.wav");

	// Load Item
	Texture2D* itemTex = new Texture2D();
	itemTex->Load("Textures/AmmoBox.tga", true);
	SoundEffect* noise = new SoundEffect();
	noise->Load("Sounds/ItemGrab.wav");

	for (int i = 0; i < ITEMCOUNT; i++)
	{
		_supplies[i]->texture = itemTex;
		_supplies[i]->pop = noise;
		_supplies[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());

		while (_supplies[i]->position->X < (_boundaryX + 50) || //spawning within boundaries
					_supplies[i]->position->X > (Graphics::GetViewportWidth() - (_boundaryX + 50)) ||
					_supplies[i]->position->Y < (_boundaryY + 50) ||
					_supplies[i]->position->Y >(Graphics::GetViewportHeight() - (_boundaryY + 50)))
		{
			_supplies[i]->position->X = rand() % Graphics::GetViewportWidth();
			_supplies[i]->position->Y = rand() % Graphics::GetViewportHeight();
		}

		_supplies[i]->rect = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 32, 32);
	}

	//Load Combo Item
	_x2->texture = new Texture2D();
	_x2->pop = new SoundEffect();
	_x2->texture->Load("Textures/X2.png", false);
	_x2->pop->Load("Sounds/X2.wav");
	_x2->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());

	while (_x2->position->X < (_boundaryX + 50) || //spawning within boundaries
				_x2->position->X >(Graphics::GetViewportWidth() - (_boundaryX + 50)) ||
				_x2->position->Y < (_boundaryY + 50) ||
				_x2->position->Y >(Graphics::GetViewportHeight() - (_boundaryY + 50)))
	{
		_x2->position->X = rand() % Graphics::GetViewportWidth();
		_x2->position->Y = rand() % Graphics::GetViewportHeight();
	}

	_x2->rect = new Rect(0.0f, 0.0f, 33, 30);

	// Load Enemy
	Texture2D* blue = new Texture2D();
	blue->Load("Textures/EnemyTank.tga", false);

	for (int i = 0; i < ENEMYCOUNT; i++)
	{
		_tank[i]->pattern = i + 1;//This is used to help implement the different patterns for each ghost
		_tank[i]->texture = blue;
		_tank[i]->death = new SoundEffect();
		_tank[i]->death->Load("Sounds/Boom.wav");
		_tank[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());

		if (_level == 2)
		{
			while (_tank[i]->position->X > _boundaryX &&
				_tank[i]->position->X < (Graphics::GetViewportWidth() - _boundaryX) ||
				_tank[i]->position->Y > _boundaryY &&
				_tank[i]->position->Y < (Graphics::GetViewportHeight() - _boundaryY))
			{
				_tank[i]->position->X = rand() % Graphics::GetViewportWidth();
				_tank[i]->position->Y = rand() % Graphics::GetViewportHeight();
			}
		}
		else
		{
			while (_tank[i]->position->X < (_boundaryX + 30) ||
				_tank[i]->position->X > (Graphics::GetViewportWidth() - (_boundaryX + 30)) ||
				_tank[i]->position->Y < (_boundaryY + 30) ||
				_tank[i]->position->Y > (Graphics::GetViewportHeight() - (_boundaryY + 30)) ||
				_tank[i]->position->X > (_player->position->X - 250) &&
				_tank[i]->position->X < (_player->position->X + 250) && 
				_tank[i]->position->Y > (_player->position->Y - 250) && 
				_tank[i]->position->Y < (_player->position->Y + 250))
			{
				_tank[i]->position->X = rand() % Graphics::GetViewportWidth();
				_tank[i]->position->Y = rand() % Graphics::GetViewportHeight();
			}
		}


		_tank[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}

	// Load Bullet
	Texture2D* proj = new Texture2D;
	proj->Load("Textures/Proj.png", false);

	for (int i = 0; i < FIREBALLCOUNT; i++)
	{
		_fireball[i]->texture = proj;
		_fireball[i]->position = new Vector2(-10000, -10000);
		_fireball[i]->sourceRect = new Rect(0.0f, 0.0f, 10, 10);
	}

	//set highscore at other corner
	_highScorePos = new Vector2(865.0f, 31.0f);
	_comboPos = new Vector2(870.0f, 80.0f);
	_comboTimePos = new Vector2(930.0f, 64.0f);
	_countDown = false;
	_itemsLeft = new Vector2(12.0f, 25.0f);

	for (int i = 0; i < SCORECOUNT; i++)
		_scores[i] = new Vector2(600, 181 + (38.3 * i));
	

	//Start screen parameters
	_startScreen = new Texture2D;

	if (_level == 1)
	{

		if (_tutorial)
			_startScreen->Load("Textures/StartScreen.png", false);
		else
		{
			_startScreen->Load("Textures/Level1Start.png", false);
			_enemySpeed += 0.01;
		}

	}
	else if (_level == 2)
	{
		_startScreen->Load("Textures/Level2Start.png", false);
		_tutorial = false;
	}
		else
			_startScreen->Load("Textures/Level3Start.png", false);

	_startRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());

	//Collision
	_bump = new SoundEffect();
	_bump->Load("Sounds/Bump.wav");
	_oneBump1 = false;
	_oneBump2 = true;

	//Level background parameters
	Texture2D* b = new Texture2D;
	_hUD = new Texture2D;

	if (_level == 2)
		b->Load("Textures/Level2.png", false);	
	else if (_level == 3)
			b->Load("Textures/Level3.png", false);
		else
			b->Load("Textures/Level1.png", false);

	_background = b;
	_hUD->Load("Textures/HUD.png", false);
	_backgroundRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());

	//Pause parameters
	_pause = new Texture2D;
	_pause->Load("Textures/Pause.png", false);
	_pauseRect = new Rect(0.0f, 0.0f, 60, 80);
	_pausePos = new Vector2(800.0f, 20.0f);
	_pauseFlash = 0;

	//Game over screen parameters
	_gameOver = new Texture2D;
	_gameOver->Load("Textures/GameOver.png", false);
	_gameOverRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());

	//Win screen parameters
	_win = new Texture2D;
	_win->Load("Textures/Victory.png", false);
	_winRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());

	// Set Menu Paramters
	_menuBackground = new Texture2D;
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), 
		Graphics::GetViewportHeight()); 
	_menuStringPos = new Vector2(Graphics::GetViewportWidth() / 2.0f, 
		Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{

	// Gets the current state of the keyboard
	Input::KeyboardState* state = Input::Keyboard::GetState();

	//// Gets the current state of the mouse
	Input::MouseState* mState = Input::Mouse::GetState();

	CheckStart(state, mState, elapsedTime);	

	
}

void Pacman::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing

	//Draw background
	SpriteBatch::Draw(_background, _backgroundRect, nullptr);

	//Enemy tanks
	for (int i = 0; i < ENEMYCOUNT; i++)
	{
			SpriteBatch::Draw(_tank[i]->texture, _tank[i]->position, _tank[i]->sourceRect);
	}

	//Draw chassis of tank
	if (!_player->dead)
	{
		SpriteBatch::Draw(_player->texture, _player->position, _player->sourceRect);
	}

	//Draw fireball 
	for (int i = 0; i < FIREBALLCOUNT; i++)
	{

		if (!_fireball[i]->impact)
			SpriteBatch::Draw(_fireball[i]->texture, _fireball[i]->position, _fireball[i]->sourceRect);// Draws Munchie	

	}

	//Draw tank turret
	if (!_player->dead)
	{
		SpriteBatch::Draw(_player->texture, _player->turretPos, _player->turretRect,
						_player->turretOri, 1.0, _player->turretRot, Color::White, SpriteEffect::NONE);// Draws Pacman
	}

	for (int i = 0; i < ITEMCOUNT; i++)
	{
		SpriteBatch::Draw(_supplies[i]->texture, _supplies[i]->position, _supplies[i]->rect);// Draws Munchie	
	}

	SpriteBatch::Draw(_x2->texture, _x2->position, _x2->rect);

	//Draw background
	SpriteBatch::Draw(_hUD, _backgroundRect, nullptr);

	//Draw highscore String
	std::stringstream stream2;
	stream2 << "Score: " << _player->highScore;
	SpriteBatch::DrawString(stream2.str().c_str(), _highScorePos, Color::Red);

	//Draw combo time String
	if (_countDown)
	{
		std::stringstream combo;
		combo << "x" << _player->combo;
		SpriteBatch::DrawString(combo.str().c_str(), _comboPos, Color::Blue);
		std::stringstream comboTime;
		comboTime << "Time : " << (_player->comboTime - 1000) / 3000;
		SpriteBatch::DrawString(comboTime.str().c_str(), _comboTimePos, Color::Cyan);
	}

	//Draw items left string
	std::stringstream munchieLeft;
	munchieLeft << "Supplies Left: " << _player->itemsCollect;
	SpriteBatch::DrawString(munchieLeft.str().c_str(), _itemsLeft, Color::Yellow);

	if (_paused)
	{
		_pauseFlash += elapsedTime;

		if (_pauseFlash >= 0 && _pauseFlash < 500)
			SpriteBatch::Draw(_pause, _pausePos, _pauseRect);

		if (_pauseFlash >= 1000)
			_pauseFlash = 0;

	}

	//Draw win screen
	if (_player->itemsCollect <= 0)
		SpriteBatch::Draw(_win, _winRect, nullptr);

	//Draw game over screen
	if (_player->dead)
	{
		SpriteBatch::Draw(_gameOver, _gameOverRect, nullptr);

		for (int i = 0; i < SCORECOUNT; i++)
		{
			std::stringstream scores;
			scores << "" << _scoreNo[i];
			SpriteBatch::DrawString(scores.str().c_str(), _scores[i], Color::White);
		}

	}			

	//Draw start screen
	if (!_startStop)
		SpriteBatch::Draw(_startScreen, _startRect, nullptr);
	
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mState)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _player->speedMultiplier;

	if (_player->dead && _highScore)
	{
		bool check = false;
		int index = 0;
		int temp = _scoreNo[index];
		int temp2 = _player->highScore;

		while (!check)
		{

			if (_player->highScore > _scoreNo[index])
			{

				for (int i = index; i < SCORECOUNT; i++)
				{
					temp = _scoreNo[i];
					_scoreNo[i] = temp2;
					temp2 = temp;
				}

				check = true;
			}
			else
				index++;

		}

		_highScore = false;
	}

	if (_player->dead && state->IsKeyDown(Input::Keys::Y))
	{
		_level = 1;
		_player->highScore = 0;
		_enemySpeed = 0.01f;
		_highScore = true;
		_tutorial = true;
		levelChange();
	}
		

	if (!_player->dead && _player->itemsCollect <= 0 && state->IsKeyDown(Input::Keys::T))
	{
		_level++;

		if (_level > 3)
			_level = 1;

		levelChange();
	}

	if (!_player->dead)
	{

		if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
		{
			_player->speedMultiplier = 5.0f;
			_player->speedUpAni = true;
		}				
		else
		{
			_player->speedMultiplier = 1.0f;
			_player->speedUpAni = false;
		}
			

		// Checks if D key is pressed
		if (state->IsKeyDown(Input::Keys::D))
		{
			_player->position->X += pacmanSpeed;
			_player->direction = 0;
			_player->turretPos->X = (_player->position->X + 15);
			_player->turretPos->Y = (_player->position->Y + 20);
		} //Moves Pacman across X axis
		else if (state->IsKeyDown(Input::Keys::A)) 			// Checks if A key is pressed
			{
				_player->position->X -= pacmanSpeed;
				_player->direction = 2;
				_player->turretPos->X = (_player->position->X + 25);
				_player->turretPos->Y = (_player->position->Y + 20);
			}									                 //Moves Pacman across X axis in the opposite direction	
			else if (state->IsKeyDown(Input::Keys::W))// Checks if W key is pressed
				{
					_player->position->Y -= pacmanSpeed;
					_player->direction = 3;
					_player->turretPos->X = (_player->position->X + 20);
					_player->turretPos->Y = (_player->position->Y + 25);//Moves Pacman across Y axis
				}
				else if (state->IsKeyDown(Input::Keys::S))	// Checks if S key is pressed		
					{
						_player->position->Y += pacmanSpeed; //Moves Pacman across Y axis in the opposite direction
						_player->direction = 1;
						_player->turretPos->X = (_player->position->X + 20);
						_player->turretPos->Y = (_player->position->Y + 15);
					}
	}

	if (mState->LeftButton == Input::ButtonState::PRESSED)
		{

			if (_player->shotTime >= 10)
				_player->shotTime = 0;

			if (_player->count > FIREBALLCOUNT)
				_player->count = 0;

			if (_player->shotTime == 0)
			{
				_fireball[_player->count]->fire = true;
				_player->count++;
			}

			 _player->shotTime++;
		}

}

void Pacman::CheckStart(Input::KeyboardState* state, Input::MouseState* mState, int elapsedTime)//Start and Pause
{
		if(!_startStop)//Start screen
	{
		if (state->IsKeyDown(Input::Keys::SPACE))//If space is pressed, the game starts
		{
			_start = !_start;
			_startStop = !_startStop;
		}
	}

	if (!_start)
	{
		if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)//Pressing P will pause the game
		{
			_pKeyDown = true;
			_paused = !_paused;
		}

		if (state->IsKeyUp(Input::Keys::P))
			_pKeyDown = false;

		CheckPaused(state, mState, elapsedTime);
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::MouseState* mState, int elapsedTime)//All methods that stop when the
{																								 //game is on the start screen/paused

			if (!_paused)
		{	
			Input(elapsedTime, state, mState);

			UpdatePlayer(elapsedTime, mState); 
			
			for (int i = 0; i < ENEMYCOUNT; i++)
			{
				UpdateEnemies(_tank[i], elapsedTime);
			}

			for (int i = 0; i < ITEMCOUNT; i++)
			{
				CheckCollision(_supplies[i], elapsedTime);
			}

			for (int i = 0; i < ITEMCOUNT; i++)
			{
				UpdateItems(_supplies[i], elapsedTime);
			}

			UpdateX2(_x2, elapsedTime);

			if (!_player->dead)
			{

				for (int i = 0; i < FIREBALLCOUNT; i++)
				{
					CheckProjState(_fireball[i], elapsedTime, mState);
				}

			}
			
			CheckViewportCollision(elapsedTime);
		}
}

void Pacman::CheckViewportCollision(int elapsedTime)//Check if player has moved beyond the game viewport
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _player->speedMultiplier;
	// Checks if Player is trying to disappear

	if (_player->position->X + _player->sourceRect->Width > Graphics::GetViewportWidth() - _boundaryX) 
		//1024 is game width or boundary
	{
		// Player hit right wall - reset his position
		_player->position->X -= pacmanSpeed;
		_player->turretPos->X -= pacmanSpeed;
		if (_oneBump2)
			_oneBump1 = true;
	}

	if (_player->position->X < 0 + _boundaryX)
	{
		// Player hit left wall - reset his position
		_player->position->X += pacmanSpeed;
		_player->turretPos->X += pacmanSpeed;
		if (_oneBump2)
			_oneBump1 = true;
	}

	if (_player->position->Y < 0 + _boundaryY)
	{
		// Player hit top wall - reset his position
		_player->position->Y += pacmanSpeed;
		_player->turretPos->Y += pacmanSpeed;
		if (_oneBump2)
			_oneBump1 = true;
	}

	if (_player->position->Y + _player->sourceRect->Height > Graphics::GetViewportHeight() - _boundaryY)
	{
		// Player hit bottom wall - reset his position
		_player->position->Y -= pacmanSpeed;
		_player->turretPos->Y -= pacmanSpeed;
		if (_oneBump2)
			_oneBump1 = true;
	}

	if (_oneBump1)
	{
		Audio::Play(_bump);
		_oneBump1 = false;
		_oneBump2 = false;
	}

	if (_player->position->X + _player->sourceRect->Width < Graphics::GetViewportWidth() - _boundaryX - 10 &&
		_player->position->X > 10 + _boundaryX &&
		_player->position->Y > 10 + _boundaryY &&
		_player->position->Y + _player->sourceRect->Height < Graphics::GetViewportHeight() - _boundaryY - 10)
		_oneBump2 = true; //This is to avoid repeated bumps when colliding with the edge

}


void Pacman::CheckCollision(Item* munchies, int elapsedTime)//Checks collision with ingame entities (enemies, items, etc.)
{
	//This is for combos when colliding with items or shooting enemies	
	if (_countDown)
	{
		_player->comboTime -= 5;
		if (_player->comboTime <= 2000)
		{
			_player->combo = 1;
			_countDown = false;
		}
	}

	//Checks if pacman has collided with a munchie
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		if (CheckCollisionSquare(_player->position->Y, _player->position->Y + _player->sourceRect->Height, _player->position->X + _player->sourceRect->Width, _player->position->X,
			_supplies[i]->position->Y, _supplies[i]->position->Y + _supplies[i]->rect->Height, _supplies[i]->position->X + _supplies[i]->rect->Width, _supplies[i]->position->X) &&
			!_player->dead)
		{
			Audio::Play(_supplies[i]->pop);
			_supplies[i]->position->Y = -10000;
			_supplies[i]->position->X = -10000;
			_player->itemsCollect--;
			_player->highScore += (100 * _player->combo);

			if (_countDown)
				_player->combo++;
		}
	}

	//Combo Item
	if (CheckCollisionSquare(_player->position->Y, _player->position->Y + _player->sourceRect->Height, _player->position->X + _player->sourceRect->Width, _player->position->X,
		_x2->position->Y, _x2->position->Y + _x2->rect->Height, _x2->position->X + _x2->rect->Width, _x2->position->X) &&
		!_player->dead)
	{
		Audio::Play(_x2->pop);
		_x2->position->Y = -10000;
		_x2->position->X = -10000;
		_player->combo++;
		_player->comboTime = 33000;
		_countDown = true;
	}
	
	for (int i = 0; i < ENEMYCOUNT; i++)
	{


		//Checks for Enemy Collision with Tank
		if (!_player->dead &&
			!_tank[i]->dead &&
			CheckCollisionSquare(_player->position->Y, _player->position->Y + _player->sourceRect->Height,
			_player->position->X + _player->sourceRect->Width, _player->position->X, _tank[i]->position->Y,
			_tank[i]->position->Y + _tank[i]->sourceRect->Height, _tank[i]->position->X + _tank[i]->sourceRect->Width,
			_tank[i]->position->X))
		{
			_player->dead = true;
			Audio::Play(_player->ouch);
		}	

		//Checks for Fireball collision with enemy
		CheckFireEnemyImpact(i);

	}

}

void Pacman::CheckFireEnemyImpact(int e)//Enemy number variable = e
{

	for (int i = 0; i < FIREBALLCOUNT; i++)
	{

		if (!_tank[e]->dead && CheckCollisionSquare(_fireball[i]->position->Y, _fireball[i]->position->Y + _fireball[i]->sourceRect->Height,
			_fireball[i]->position->X + _fireball[i]->sourceRect->Width, _fireball[i]->position->X, _tank[e]->position->Y,
			_tank[e]->position->Y + _tank[e]->sourceRect->Height, _tank[e]->position->X + _tank[e]->sourceRect->Width,
			_tank[e]->position->X))
		{
			_tank[e]->dead = true;
			_player->highScore += (200 * _player->combo);

			if (_countDown)
				_player->combo++;

			Audio::Play(_tank[e]->death);
			_fireball[i]->impact = true;
		}

	}

}

boolean Pacman::CheckCollisionSquare(int top1, int bottom1, int rightSide1, int leftSide1,
	int top2, int bottom2, int rightSide2, int leftSide2)
{
	if (top1 > bottom2)
		return false;
	if (rightSide1 < leftSide2)
		return false;
	if (leftSide1 > rightSide2)
		return false;
	if (bottom1 < top2)
		return false;

	return true;
}

void Pacman::SetBoundaries() //Setting the boundaries for each level
{
	if (_level >= 3)
	{
		_boundaryX = 305;
		_boundaryY = 0;
	}
	else  if (_level == 2)
	{
		_boundaryX = 90;
		_boundaryY = 70;
	}
	else
	{
		_boundaryX = 0;
		_boundaryY = 0;
	}
}

void Pacman::UpdatePlayer(int elapsedTime, Input::MouseState* mState)
{
	_player->currentFrameTime += elapsedTime;
	_player->sourceRect->Y = _player->sourceRect->Height * _player->direction;
	_player->sourceRect->X = _player->sourceRect->Width * _player->frame;

	if (_player->currentFrameTime > _cPacmanFrameTime)
	{
		_player->frame++;

		if (_player->speedUpAni)
		{
			_player->turretRect->X = 373.0f;
			_player->turretRect->Y = 193.0f;

			if (_player->frame >= 8)
			{
				_player->frame = 5;
			}
		}
		else
		{
			_player->turretRect->X = 373.0f;
			_player->turretRect->Y = 207.0f;

			if (_player->frame >= 4)
			{
				_player->frame = 0;
			}

		}

		_player->currentFrameTime = 0;
	}

	_player->turretRot = AngleCalc(mState);

}

float Pacman::AngleCalc(Input::MouseState* mState)
{
	float x = (mState->X - _player->turretPos->X);
	float y = (mState->Y - _player->turretPos->Y);

	if (x == 0)//to avoid a division by zero in angle calculation
	{
		x++;
	}

	if (y == 0)
	{
		y += 2;
	}

	float a = atan(y / x);
	float angle = -1 * MathHelper::ToDegrees(a);

	if(mState->X < _player->turretPos->X)
		angle += 180;

	return angle;
}

void Pacman::UpdateItems(Item* items, int elapsedTime)
{

	for (int i = 0; i < ITEMCOUNT; i++)
	{
		_supplies[i]->currentFrameTime += elapsedTime;
		_supplies[i]->rect->Y = 0; 
		_supplies[i]->rect->X = _supplies[i]->rect->Width * _supplies[i]->frameCount;

		if (_supplies[i]->currentFrameTime > 500)
		{
			_supplies[i]->frameCount++;

			if ( _supplies[i]->frameCount >= 9) 
					_supplies[i]->frameCount = 0;

			_supplies[i]->currentFrameTime = 0;
		}

		if (_supplies[i]->position->X >= 900 && _supplies[i]->position->X < 2000 ||
			_supplies[i]->position->X <= 30 && _supplies[i]->position->X > -1000 ||
			_supplies[i]->position->Y >= 800 && _supplies[i]->position->Y < 2000 ||
			_supplies[i]->position->Y <= 25 && _supplies[i]->position->Y > -1000)
		{
			_supplies[i]->position->X = rand() % Graphics::GetViewportWidth();
			_supplies[i]->position->Y = rand() % Graphics::GetViewportHeight();
		}

	}
}

void Pacman::UpdateX2(Item* x2, int elapsedTime)
{

	_x2->currentFrameTime += elapsedTime * 4;
	_x2->rect->Y = 0;
	_x2->rect->X = _x2->rect->Width * _x2->frameCount;

	if (_x2->currentFrameTime > 500)
	{
		_x2->frameCount++;

		if (_x2->frameCount >= 8)
			_x2->frameCount = 0;

		_x2->currentFrameTime = 0;
	}

	if (_x2->position->X >= 900 && _x2->position->X < 2000 ||
		_x2->position->X <= 30 && _x2->position->X > -1000 ||
		_x2->position->Y >= 800 && _x2->position->Y < 2000 ||
		_x2->position->Y <= 25 && _x2->position->Y > -1000)
	{
		_x2->position->X = rand() % Graphics::GetViewportWidth();
		_x2->position->Y = rand() % Graphics::GetViewportHeight();
	}


}

void Pacman::UpdateEnemies(MovingEnemy* tanks, int elapsedTime)
{

	for (int i = 0; i < ENEMYCOUNT; i++)
	{

		 //Animation of tanks
		_tank[i]->currentFrameTime += elapsedTime;

		if (_tank[i]->dead)
		{	
			if (!_tank[i]->deathAni)
			{
				_tank[i]->currentFrameTime = 0;
				_tank[i]->frameCount = 0;
				_tank[i]->sourceRect->Width = 30;
				_tank[i]->sourceRect->Height = 30;
				_tank[i]->deathAni = true;
			}

			_tank[i]->sourceRect->Y = 80;
			_tank[i]->sourceRect->X = _tank[i]->sourceRect->Width * _tank[i]->frameCount;
			_tank[i]->frameCount = _tank[i]->currentFrameTime / 500;

			if (_tank[i]->frameCount > 8)
			{
				_tank[i]->position->X = 10000;
				_tank[i]->position->Y = 10000;
			}

		}
		else
		{
			_tank[i]->sourceRect->Y = _tank[i]->sourceRect->Height * _tank[i]->direction;
			_tank[i]->sourceRect->X = _tank[i]->sourceRect->Width * _tank[i]->frameCount;


			if (_tank[i]->currentFrameTime > _cGhostFrameTime)
			{
				_tank[i]->frameCount++;

				if (_tank[i]->frameCount >= 3)
					_tank[i]->frameCount = 0;

				_tank[i]->currentFrameTime = 0;
			}

			if (_tank[i]->direction == 0)//Moves Right
			{
				_tank[i]->position->X += (_enemySpeed * elapsedTime);
			}
			else if (_tank[i]->direction == 2)//Moves Left
			{
				_tank[i]->position->X -= (_enemySpeed * elapsedTime);
			}
			else if (_tank[i]->direction == 1)//Moves Down
			{
				_tank[i]->position->Y += (_enemySpeed * elapsedTime);
			}
			else if (_tank[i]->direction == 3)//Moves Up
			{
				_tank[i]->position->Y -= (_enemySpeed * elapsedTime);
			}

			if (!_player->dead) //different patterns
			{

				if (i % 2 == 0)
				{

					if (_player->position->X < _tank[i]->position->X &&
						(_player->position->X + 10) <= _tank[i]->position->X)
					{
						_tank[i]->position->X -= 0.001 * _enemySpeed;
						_tank[i]->direction = 2;
					}
					else if (_player->position->X > _tank[i]->position->X &&
						(_player->position->X - 10) >= _tank[i]->position->X)
					{
						_tank[i]->position->X += 0.001 * _enemySpeed;
						_tank[i]->direction = 0;
					}
					else if (_player->position->Y < _tank[i]->position->Y &&
						(_player->position->Y + 10) <= _tank[i]->position->Y)
					{
						_tank[i]->position->Y -= 0.001 * _enemySpeed;
						_tank[i]->direction = 3;
					}
					else if (_player->position->Y > _tank[i]->position->Y &&
						(_player->position->Y - 10) >= _tank[i]->position->Y)
					{
						_tank[i]->position->Y += 0.001 * _enemySpeed;
						_tank[i]->direction = 1;
					}

				}

				if (i % 2 != 0)
				{

					if (_player->position->Y > _tank[i]->position->Y &&
						(_player->position->Y - 10) >= _tank[i]->position->Y)
					{
						_tank[i]->position->Y += 0.001 * _enemySpeed;
						_tank[i]->direction = 1;
					}
					else if (_player->position->Y < _tank[i]->position->Y &&
						(_player->position->Y + 10) <= _tank[i]->position->Y)
					{
						_tank[i]->position->Y -= 0.001 * _enemySpeed;
						_tank[i]->direction = 3;
					}
					else if (_player->position->X > _tank[i]->position->X &&
						(_player->position->X - 10) >= _tank[i]->position->X)
					{
						_tank[i]->position->X += 0.001 * _enemySpeed;
						_tank[i]->direction = 0;
					}
					else if (_player->position->X < _tank[i]->position->X &&
						(_player->position->X + 10) <= _tank[i]->position->X)
					{
						_tank[i]->position->X -= 0.001 * _enemySpeed;
						_tank[i]->direction = 2;
					}

				}

			}

		}

	}
}

void Pacman::CheckProjState(Projectile* fireball, int elapsedTime, Input::MouseState* mState)
{

	for (int i = 0; i < FIREBALLCOUNT; i++)
	{

		if (_fireball[i]->fire == true) //All these calculations are done to output the direction of the projectile
			                            //to where the mouse pointer is once the left mouse button was clicked
		{
			Audio::Play(_player->gunFire);
			_fireball[i]->impact = false;
			float radian = MathHelper::ToRadians(AngleCalc(mState)) + (0.5 * MathHelper::Pi);
			_fireball[i]->dirX = sin(radian) / 5;
			_fireball[i]->dirY = cos(radian) / 5;
			_fireball[i]->position->X = (_player->turretPos->X - _player->turretOri->X);
			_fireball[i]->position->Y = (_player->turretPos->Y - _player->turretOri->Y);
			_fireball[i]->fire = false;
			//Calculated angle is converted to radians and then added onto 1/2 a radian
			//A right-angled triangle is created by the mouse position and the turret position
			//in order to get the x and y coordinate, the set the hypotenuse to be 1
			//(therefore no need to perform a multiplication with the equation soh / cah / toa)
		}

		_fireball[i]->position->X += _fireball[i]->dirX * _fireball[i]->speed;
		_fireball[i]->position->Y += _fireball[i]->dirY * _fireball[i]->speed;

		if (_fireball[i]->impact)
		{
			_fireball[i]->position->X = -10000;
			_fireball[i]->position->Y = -10000;
		}
	}

}

void Pacman::levelChange()
{
		_player->combo = 1;
		_player->comboTime = 0;
		_player->itemsCollect = 50;
		_player->dead = false;
		_player->itemsCollect = ITEMCOUNT;

		for (int i = 0; i < ENEMYCOUNT; i++)
		{
			_tank[i]->dead = false;
			_tank[i]->deathAni = false;
		}

		SetBoundaries();
		LoadContent();
		_start = true;
		_startStop = false;
}
