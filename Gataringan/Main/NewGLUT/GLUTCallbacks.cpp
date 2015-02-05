#include "GLUTCallbacks.h"
#include "GLUT.h"

namespace GLUTCallbacks
{
	namespace
	{
		GLUT * glut = nullptr;
	}

	void Init(GLUT * gl)
	{
		glut = gl;
	}

	void Display()
	{

		if (glut != nullptr)
		{
			glut->Display();
		}
			
	}

	void Idle()
	{
		glut->Idle();
	}

	void Reshape(int w, int h)
	{
		glut->Reshape(w, h);
	}

	void Mouse(int button, int state, int x, int y)
	{
		glut->Mouse(button, state, x, y);
	}

	void MouseMotion(int x, int y)
	{
		glut->MouseMotion(x, y);
	}

	void Keyboard(unsigned char key, int x, int y)
	{
		glut->Keyboard(key, x, y); 
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		glut->KeyboardUp(key, x, y);
	}

	void Timer(int preferredRefresh)
	{

		if (glut != nullptr)
		{
			int updateTime = glutGet(GLUT_ELAPSED_TIME);
			glut->Update();
			updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;
			glut->Timer(preferredRefresh);
			glutTimerFunc(16 - updateTime, GLUTCallbacks::Timer, 16);		
		}

	}

	void objectMenu(int choice)
	{

		if (glut != nullptr)
		{
			glut->ObjectMenu(choice);
		}		

	}

}