#pragma once
#include"Task.h"
#include<Siv3D.hpp>
class InputKey
	:public Task
{
public:
	bool up(){ return Input::KeyUp.pressed; }
	bool down(){ return Input::KeyDown.pressed; }
	bool right(){ return Input::KeyRight.pressed; }
	bool left(){ return Input::KeyLeft.pressed; }
	bool shot(){ return Input::KeyZ.pressed; }
	void updata()override
	{	}
	bool isAlive()const override{ return true; }
};