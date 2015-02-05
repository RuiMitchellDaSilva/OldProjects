#pragma once

class GLUT; //foward declare GLUT

namespace GLUTCallbacks
{
	void glutInitWindowSize(int width, int height);
	void glutInitWindowPosition(int x, int y);
	void Init(GLUT * gl);
	void Display();
	void Idle();
	void Reshape(int w, int h);
	void MouseMotion(int x, int y);
	void Mouse(int button, int state, int x, int y);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);	
	void Timer(int preferredRefresh);
	void objectMenu(int value);
};