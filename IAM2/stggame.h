#pragma once
#include"StgPart.h"
#include"LuaBullet.h"

#include"FieldObjectDesign.h"
class testBomb
	:public stgpart::Bomb
{
	int count;
public:
	testBomb(double x, double y, int count)
		:stgpart::Bomb(x, y,1000), count(count)
	{}

	void updata(stgpart::TaskMediator&task)override
	{
		task.drawer->DrawCricre(x, y, 40);
		--count;
		if (count<0)alive = false;
	}
	stgpart::Sharp getSharp()const override
	{
		return{ Circle{ x, y, 40 } };
	}
};

class testPAtk
	:public stgpart::PlayerAttack
{
public:
	testPAtk(double x, double y)
		:stgpart::PlayerAttack(x, y, 5)
	{}
	
	void updata(stgpart::TaskMediator &task)override
	{
		x += 4;
		task.drawer->DrawCricre(x, y, 3);
	}
	stgpart::Sharp getSharp()const override
	{
		return{ Circle{ x, y, 3 } };
	}
};
class PlayerAtacck
	:public stgpart::PlayerAttack
{
public:
	PlayerAtacck(double x, double y)
	:stgpart::PlayerAttack(x, y, 5)
	{}

	void updata(stgpart::TaskMediator &task)override
	{
		x += 12;
		Rect{ Point{ x, y }, 20, 5 }.draw(Palette::White);
		if (!Point{ x, y }.intersects(fieldRect()))
			alive = false;
	}
	stgpart::Sharp getSharp()const override
	{
		return{ Rect{ Point{ x, y }, 10, 20 } };
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
		,bomb
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
		case State::bomb:
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
			{
				task.effector->addPlayerShotSE();
				task.playerAtkmane->add(std::make_shared<PlayerAtacck>(x, y));
			}
				
		}
		
	}
	
	void draw(stgpart::TaskMediator &task)
	{
		task.drawer->DrawCricre(x, y, 6);
	}

	void bomb(stgpart::TaskMediator &task)
	{
		switch (state)
		{
		case State::normal:
			if (Input::KeyX.clicked)
				state = State::bomb;
			break;

		case State::bomb:
			bool r = Input::KeyX.released || (bomb_count == 0);
			if (r)
			{
				task.bombmaneger->add(std::make_shared<testBomb>(x, y, 20));
				alive = false;
			}
			--bomb_count;
			break;
		}
	}
public:
	Player(double x, double y)
		:stgpart::PlayerShip(x, y)
	{}
	void updata(stgpart::TaskMediator &task)
	{
		move(task);
		shot(task);
		draw(task);
		bomb(task);
	}
	stgpart::Sharp getSharp()const
	{
		return{ Circle{ x, y, 6 } };
	}
	void onHitFlag()override
	{
		alive = false;
	}
};


class StgGame
	:public Task
{
	std::list<std::shared_ptr<stgpart::MediatorTask>> tasklist;
	stgpart::TaskMediator mediator;
	std::shared_ptr<stgpart::Drawer> drawer;
	std::shared_ptr<stgpart::CheckHit> checkhit;
	void connectBt()
	{
		{
			bool hasBullet = false;
			auto lua = mediator.lua->data();
			lua_getglobal(lua, "shotStack");
			lua_pushnil(lua);
			while (lua_next(lua, -2))
			{
				hasBullet = true;
				int idx = luaL_ref(lua, LUA_REGISTRYINDEX);
				auto p = std::make_shared<stgpart::LuaBullet>(0., 0., idx, mediator.design->getNone(), false);
				get().mediator.bulletMane->add(p);
			}
			lua_pop(lua, 1);
			if (hasBullet)
				mediator.effector->addEnemyShotSE();
		}
		{
			auto lua = mediator.lua->data();
			lua_getglobal(lua, "setStack");
			lua_pushnil(lua);
			while (lua_next(lua, -2))
			{
				int idx = luaL_ref(lua, LUA_REGISTRYINDEX);
				lua_getfield(lua, 0, "hp");
				int hp = lua_tointeger(lua, -1);
				lua_pop(lua, 1);
				auto p = std::make_shared<stgpart::LuaEnemy>(0., 0., idx, mediator.design->getNone(), false, hp);
				get().mediator.enemymane->add(p);
			}
			lua_pop(lua, 1);
		}
		
	}
	void repop()
	{
		auto pos = mediator.motherShip->getPos();
		mediator.playerMane->add(std::make_shared<Player>(pos.x, pos.y));
	}

	StgGame(){}
public:
	StgGame(StgGame const&) = delete;
	void init()
	{
		checkhit = std::make_shared<stgpart::CheckHit>();
		auto input = std::make_shared<InputKey>();

		auto player = std::make_shared<stgpart::PlayerShipManeger>();
		auto btmane = std::make_shared<stgpart::BulletManeger>();
	
		
		
		auto patk = std::make_shared<stgpart::PlayerAtackManeger>();
		auto bombMane = std::make_shared<stgpart::BombManeger>();
		auto drawer = std::make_shared<stgpart::Drawer>();
		auto lua = std::make_shared<luawrap::Lua>();
		auto playerdata = std::make_shared<stgpart::PlayerData>();

		auto design = std::make_shared<stgpart::FieldObjectDesignFactory>();

		auto motherShip = std::make_shared<stgpart::MotherShip>();
		lua->load_file("test.lua");
		mediator = stgpart::TaskMediator(player, btmane, patk, bombMane, drawer, lua, input, playerdata, design, motherShip);
		tasklist.assign(std::initializer_list<std::shared_ptr<stgpart::MediatorTask>>
		{ btmane, patk, player, bombMane, checkhit, mediator.enemymane, mediator.playerdata});
	}
	void updata()override
	{
		auto p = mediator.design->getNone();
		mediator.lua->call("frameInit");
		
		Rect(0, 0, 960, 540).draw(Palette::Black);
		for (auto&task : tasklist)
		{
			task->updata(mediator);
		}
		mediator.effector->updata(mediator);
		if (Input::KeySpace.clicked)
			mediator.lua->call("Main");
		if (Input::KeyP.clicked)repop();
		if (Input::KeyM.clicked)
			mediator.motherShip->setPos(mediator.playerMane->getList().begin()->get()->getPos());
		connectBt();
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

