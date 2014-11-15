#pragma once
#include"StgPart.h"
#include"LuaBullet.h"

#include"FieldObjectDesign.h"
//class testBomb
//	:public stgpart::Bomb
//{
//	int count;
//public:
//	testBomb(double x, double y, int count)
//		:stgpart::Bomb(x, y,1000), count(count)
//	{}
//
//	void updata(stgpart::TaskMediator&task)override
//	{
//		task.drawer->DrawCricre(x, y, 40);
//		--count;
//		if (count<0)alive = false;
//	}
//	stgpart::Sharp getSharp()const override
//	{
//		return{ Circle{ x, y, 40 } };
//	}
//};

//class testPAtk
//	:public stgpart::PlayerAttack
//{
//public:
//	testPAtk(double x, double y)
//		:stgpart::PlayerAttack(x, y, 5)
//	{}
//	
//	void updata(stgpart::TaskMediator &task)override
//	{
//		x += 4;
//		task.drawer->DrawCricre(x, y, 3);
//	}
//	stgpart::Sharp getSharp()const override
//	{
//		return{ Circle{ x, y, 3 } };
//	}
//};
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
	//色々面倒になったのだよ
	int bomb_count = 180;
	int star_time;
	enum class State
	{
		normal
		,bomb
		,dead
	};
	State state = State::normal;

	
	void move(stgpart::TaskMediator &task)
	{
		auto const&input = task.input;
		auto f=[&](int spd)
		{
			auto tmp = Vec2{ x, y };
			auto v = input->arrow() * spd;
			x += v.x;
			y += v.y;
			if (!Point(x, y).intersects(fieldInnerRect()))
			{
				x = tmp.x;
				y = tmp.y;
			}
		};
		switch (state)
		{
		case State::normal:
		{
							  f(5);
		}return;
		case State::bomb:
		{
							f(9);
			
		}return;
			
		}
	}
	void shot(stgpart::TaskMediator &task)
	{
		if (state == State::normal)
		{
			if (task.input->A())
			{
				task.effector->addPlayerShotSE();
				task.playerAtkmane->add(std::make_shared<PlayerAtacck>(x, y));
			}
		}
		
	}
	
	void draw(stgpart::TaskMediator &task)
	{
		if (state != State::dead)
			task.drawer->drawPlayer(x, y);
	}

	void bomb(stgpart::TaskMediator &task)
	{
		switch (state)
		{
		case State::normal:
			if (task.input->B_click())
				state = State::bomb;
			break;

		case State::bomb:
			bool r = task.input->B_release() || (bomb_count == 0);
			if (r)
			{
				task.bombmaneger->add_basic_bomb(Vec2{ x, y }, 60, 1000, 40);
				state = State::dead;
			}
			--bomb_count;
			break;
		}
	}
public:
	Player(double x, double y)
		:stgpart::PlayerShip(x, y)
	{
			repop(Vec2{ x, y });
	}
	void updata(stgpart::TaskMediator &task)
	{
		move(task);
		shot(task);
		draw(task);
		bomb(task);
		if (task.input->C_click())
			task.motherShip->setPos(getPos());
		if (star_time > 0)--star_time;
	}
	stgpart::Sharp getSharp()const
	{
		return{ Circle{ x, y, 6 } };
	}
	void onHitFlag()override
	{
		state = State::dead;
	}
	bool isDeadState()const override{ return state == State::dead; }
	bool isStar()const{ return star_time; }
	void repop(Vec2 const&p)
	{
		state = State::normal;
		setPos(p);
		bomb_count = 180;
		star_time = 20;
	}
};

class GameInfoView
{
public:
	GameInfoView()
	{
		FontAsset::Register(L"font",20);
	}
	void updata(stgpart::TaskMediator&med)
	{
		FontAsset(L"font").draw(
			Format(L"player_life:", med.playerdata->getLife(),L"killscore",med.playerdata->get_kill_score(),L"Score",med.playerdata->getScore())
			, 0, 540);
	}
};

class BackGround
{
	Texture texture;
	int n = 0;
public:
	BackGround()
		:texture(L"data/BG.png")
	{}
	void updata()
	{
		
		
		texture.draw(Alpha(std::sin(n / 90 * Pi) * 128 + 128));

	}



};



class StgGame
	:public Task
{
	std::list<std::shared_ptr<stgpart::MediatorTask>> tasklist;
	stgpart::TaskMediator mediator;
	std::shared_ptr<stgpart::Drawer> drawer;
	std::shared_ptr<stgpart::CheckHit> checkhit;
	GameInfoView info_view;
	BackGround backGround;
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
				mediator.bulletMane->add(p);
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
				mediator.enemymane->add(p);
			}
			lua_pop(lua, 1);
		}
		
	}
	void repop()
	{
		auto pos = mediator.motherShip->getPos();
		//mediator.playerMane->add(std::make_shared<Player>(pos.x, pos.y));
	}
	enum State
	{
		game,
		dead,
		title
	};
	State state = State::title;
	Font font;
	std::shared_ptr<stgpart::PlayerShip> player;
public:
	StgGame(){}
	StgGame(StgGame const&) = delete;
	void init()
	{
		checkhit = std::make_shared<stgpart::CheckHit>();
		auto input = std::make_shared<InputKey>();

		
		auto btmane = std::make_shared<stgpart::BulletManeger>();
	
		
		player = std::make_shared<Player>(0, 0);
		auto patk = std::make_shared<stgpart::PlayerAtackManeger>();
		auto bombMane = std::make_shared<stgpart::BombManeger>();
		auto drawer = std::make_shared<stgpart::Drawer>();
		auto lua = std::make_shared<luawrap::Lua>();
		auto playerdata = std::make_shared<stgpart::PlayerData>();

		auto design = std::make_shared<stgpart::FieldObjectDesignFactory>();

		auto motherShip = std::make_shared<stgpart::MotherShip>();
		lua->load_file("test.lua");
		mediator = stgpart::TaskMediator( player,btmane, patk, bombMane, drawer, lua, input, playerdata, design, motherShip);
		tasklist.assign(std::initializer_list<std::shared_ptr<stgpart::MediatorTask>>
		{ btmane, patk,  bombMane, checkhit, mediator.enemymane, mediator.playerdata,mediator.motherShip});

		
		mediator.player->setPos(mediator.motherShip->getPos());
	}


	void game_mode()
	{
		auto p = mediator.design->getNone();
		auto const pos = mediator.player->getPos();
		
		Rect(0, 0, 960, 540).draw(Palette::Black);
		Rect(0, 540, 960, 600).draw(Palette::Aquamarine);
		backGround.updata();
		for (auto&task : tasklist)
		{
			task->updata(mediator);
		}
		mediator.effector->updata(mediator);
		if (Input::Key0.clicked)
			mediator.lua->call("Main");
		
		connectBt();
		info_view.updata(mediator);

		
		mediator.lua->call("frameInit", pos.x, pos.y);

	}

	void game_play()
	{
		mediator.player->updata(mediator);
		if (mediator.playerdata->getLife() <= 0)
		{
			state = State::dead;
		}
	}
	void game_over()
	{
		Rect{ 100, 100, 500, 200 }.draw(Palette::Black);
		font.draw(L"Game Over", 150, 100);
		font.draw(L"Zキーでタイトル、Cキーで呟く",150, 150);
		if (mediator.input->A_click())
		{
			state = State::title;
			mediator.effector->endBGM();
			init();
		}
		if (mediator.input->C_click())
		{
			Twitter::OpenTweetWindow(Format(L"はるはあけぼの、我は弾丸で", mediator.playerdata->getScore(), L"点をたたき出しました"));
		}
			
	}
	void titleMode()
	{
		Rect{ 100, 100, 500, 200 }.draw(Palette::Black);
		font.draw(L"Zキーでスタート", 150, 150);
		if (mediator.input->A_click())
		{
			init();
			mediator.effector->beginBGM();
			state = State::game;
			mediator.lua->call("Main");
		}
		
	}
	void updata()override
	{
		game_mode();
		switch (state)
		{
		case State::game:
			game_play();
			break;
		case State::dead:
			game_over();
			break;

		case State::title:
			titleMode();
			break;
		default:
			break;
		}
	}
	bool isAlive()const override
	{
		return true;
	}

	
private:
	
	
};

