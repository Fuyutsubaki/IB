#pragma once
#include"StgPart.h"
#include"LuaBullet.h"
class testBomb
	:public stgpart::Bomb
{
	float x, y;
	int count;
public:
	testBomb(float x, float y, int count)
		:x(x), y(y), count(count)
	{}

	void updata(stgpart::TaskMediator&)override
	{
		Circle(x, y, 15).draw(Palette::Aqua);
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
	float x;
	float y;
public:
	testPAtk(float x, float y)
		:x(x), y(y)
	{}
	Circle getSharp()override
	{
		return Circle(x, y, 4);
	}
	void updata(stgpart::TaskMediator &)override
	{
		x += 4;
		Circle(x, y, 4).draw(Palette::Blueviolet);
	}
	bool isAlive()const override
	{
		return true;
	}
};
class testPlayer
	:public stgpart::PlayerShip
{
	int x = 30;
	int y = 30;
	bool alive = true;
public:
	void updata(stgpart::TaskMediator &task)
	{
		auto &input = task.input;
		Circle(x, y, 6).draw(Color(255, 0, 0));
		x += input->right() - input->left();
		y += input->down() - input->up();
		if (task.input->shot())task.playerAtkmane->add(std::make_shared<testPAtk>(x, y));
		if (Input::KeyX.clicked)task.bombmaneger->add(std::make_shared<testBomb>(x, y, 20));
	}
	Circle getSharp()override
	{
		return Circle(x, y, 6);
	}
	void onHitFlag()override
	{
		alive = false;
	}
	bool isAlive()const override
	{
		return alive;
	}
};



class StgGame
	:public Task
{
	std::list<std::shared_ptr<stgpart::MediatorTask>> tasklist;
	stgpart::TaskMediator mediator;
	static int regBt(lua_State*L)
	{
		auto&med = get().mediator;
		auto idx = med.lua->makeRegisterIndex();
		auto p = std::make_shared<stgpart::LuaBullet>(0, 0, idx);
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
		auto lua = std::make_shared<luawrap::Lua>();
		lua->load_file("test.lua");
		
		lua->registerCfunc("_regBt", regBt);
		mediator = stgpart::TaskMediator(player, btmane, patk, bombMane, enemymane, lua, input);
		tasklist.assign(std::initializer_list<std::shared_ptr<stgpart::MediatorTask>>
		{ btmane, ch, chAB, patk, player, bombMane, chBomb, enemymane});

		//player->add(std::make_shared<testPlayer>());
	}
	void updata()override
	{
		for (auto&task:tasklist)
		{
			task->updata(mediator);
		}
		if (Input::KeySpace.pressed)mediator.lua->call("Main");
		if (Input::KeyP.clicked)mediator.playerMane->add(std::make_shared<testPlayer>());
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

