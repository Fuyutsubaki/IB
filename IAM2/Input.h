#pragma once
#include"Task.h"
#include<Siv3D.hpp>
class InputKey
	:public Task
{
	int asobi;
	int a;
	int b;
	int c;
public:
	InputKey()
	{
		INIReader reader(L"KeyConfig.ini");
		
		a = reader.get<int>(L"KeyZ", 0);
		b = reader.get<int>(L"KeyX", 1);
		c = reader.get<int>(L"KeyC", 2);
		asobi = reader.get<int>(L"DeadZone", 1000);

	}
	bool up()const	{ return Input::KeyUp.pressed || (Joypad(0).yPos < Joypad(0).yMax / 2 - asobi); }
	bool down()const{ return Input::KeyDown.pressed || (Joypad(0).yPos > Joypad(0).yMax / 2 + asobi); }
	bool right()const{ return Input::KeyRight.pressed || (Joypad(0).xPos > Joypad(0).xMax / 2 + asobi); }
	bool left()const{ return Input::KeyLeft.pressed || (Joypad(0).xPos < Joypad(0).xPos / 2 - asobi); }

	Vec2 arrow()const
	{
		return Vec2{ right() - left(), down() - up() };
	}
	bool A()const
	{
		return Input::KeyZ.pressed | Joypad(0).button(a).pressed;
	}
	bool A_click()const
	{
		return Input::KeyZ.clicked | Joypad(0).button(a).clicked;
	}
	bool B_click()const
	{
		return Input::KeyX.clicked | Joypad(0).button(b).clicked;
	}
	bool B_release()const
	{
		return Input::KeyX.released | Joypad(0).button(b).released;
	}

	bool C_click()const
	{
		return Input::KeyC.clicked | Joypad(0).button(c).clicked;
	}
	void updata()override
	{
		
	}
	bool isAlive()const override{ return true; }
};