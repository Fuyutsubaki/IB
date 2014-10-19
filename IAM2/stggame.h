#pragma once
#include"StgPart.h"
#include"LuaBullet.h"


class testBomb
	:public stgpart::Bomb
{
	double x, y;
	int count;
public:
	testBomb(double x, double y, int count)
		:x(x), y(y), count(count)
	{}

	void updata(stgpart::TaskMediator&task)override
	{
		task.drawer->DrawCricre(x, y, 15);
		--count;
	}
	bool isAlive()const
	{
		return count > 0;
	}
	Circle getSharp()
	{
		return{ x, y, 15 };
	}

};

class testPAtk
	:public stgpart::PlayerAttack
{
	double x;
	double y;
public:
	testPAtk(double x, double y)
		:x(x), y(y)
	{}
	Circle getSharp()override
	{
		return Circle(x, y, 4);
	}
	void updata(stgpart::TaskMediator &task)override
	{
		x += 4;
		task.drawer->DrawCricre(x, y, 3);
	}
	bool isAlive()const override
	{
		return true;
	}
};

class Player
	:public stgpart::PlayerShip
{
	//FX–Ê“|‚É‚È‚Á‚½‚Ì‚¾‚æ
	int bomb_count = 180;
	enum class State
	{
		normal
		,panic
	};
	State state = State::normal;

	
	void move(stgpart::TaskMediator &task)
	{
		auto const&input = task.input;
		switch (state)
		{
		case State::normal:
			x += (input->right() - input->left())*3;
			y += (input->down() - input->up())*3;
			break;
		case State::panic:
			x += (input->right() - input->left())*5;
			y += (input->down() - input->up())*5;
			break;
		}
	}
	void shot(stgpart::TaskMediator &task)
	{
		if (state == State::normal)
		{
			if (task.input->shot())
				task.playerAtkmane->add(std::make_shared<testPAtk>(x, y));
		}
		
	}
	
	void draw(stgpart::TaskMediator &task)
	{
		task.drawer->DrawCricre(x, y, 6);
	}
public:
	Player()
		:stgpart::PlayerShip(30, 30)
	{}
	void updata(stgpart::TaskMediator &task)
	{
		move(task);
		shot(task);
		draw(task);
		switch (state)
		{
		case State::normal:
			if (Input::KeyX.clicked)
				state = State::panic;
			break;

		case State::panic:
			--bomb_count;
			if (bomb_count == 0)
			{
				task.bombmaneger->add(std::make_shared<testBomb>(x, y, 20));
				alive = false;
			}
			break;
		}
	}
	Circle getSharp()const override
	{
		return Circle(x, y, 6);
	}
	void onHitFlag()override
	{
		alive = false;
	}
};

class BackGround
{
	void draw()
	{
		Rect(0, 0, 640, 480).draw();
	}
};

class StgGame
	:public Task
{
	std::list<std::shared_ptr<stgpart::MediatorTask>> tasklist;
	stgpart::TaskMediator mediator;
	std::shared_ptr<stgpart::Drawer> drawer;
	static int regBt(lua_State*)
	{
		auto&med = get().mediator;
		auto idx = med.lua->makeRegisterIndex();
		auto p = std::make_shared<stgpart::LuaBullet>(0.0, 0., -1 ,idx);
		get().mediator.bulletMane->add(p);
		med.lua->push(idx);
		p->pushSelf(*med.lua);
		return 1;
	}
	StgGame(){}
public:
	StgGame(StgGame const&) = delete;
	void init()
	{
		auto input = std::make_shared<InputKey>();

		auto player = std::make_shared<stgpart::PlayerShipManeger>();
		auto btmane = std::make_shared<stgpart::BulletManeger>();
	
		auto ch = std::make_shared<stgpart::CheckHit>();
		auto chAB = std::make_shared<stgpart::CheckHitAtkBt>();
		auto chBomb = std::make_shared<stgpart::checkHitBomb>();
		auto patk = std::make_shared<stgpart::PlayerAtackManeger>();
		auto enemymane = std::make_shared<stgpart::EnemyManeger>();
		auto bombMane = std::make_shared<stgpart::BombManeger>();
		auto drawer = std::make_shared<stgpart::Drawer>();
		auto lua = std::make_shared<luawrap::Lua>();
		lua->load_file("test.lua");
		
		lua->registerCfunc("_regBt", regBt);
		mediator = stgpart::TaskMediator(player, btmane, patk, bombMane, enemymane, drawer, lua, input);
		tasklist.assign(std::initializer_list<std::shared_ptr<stgpart::MediatorTask>>
		{ btmane, ch, chAB, patk, player, bombMane, chBomb, enemymane});

		//player->add(std::make_shared<testPlayer>());
	}
	void updata()override
	{
		Rect(0, 0, 640, 480).draw();
		for (auto&task:tasklist)
		{
			task->updata(mediator);
		}
		if (Input::KeySpace.pressed)mediator.lua->call("Main");
		if (Input::KeyP.clicked)mediator.playerMane->add(std::make_shared<Player>());
	}
	bool isAlive()const override
	{
		return true;
	}

	static StgGame&get()
	{
		static StgGame a;
		return a;
	}
private:
	
	
};

