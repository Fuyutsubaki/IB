#include"StgPart.h"

namespace stgpart
{
	class MyBomb
		:public Bomb
	{
		int lifetime;
		double r;
	public:
		MyBomb(Vec2 const&pos, int dmg, int lifetime, double r)
			:Bomb(pos.x, pos.y, dmg), lifetime(lifetime), r(r)
		{}
		void updata(TaskMediator&task)override
		{
			task.drawer->DrawCricre(x, y, r);
			if (lifetime > 0)
				--lifetime;
			else
				alive = false;	
		}
		Sharp getSharp()const
		{
			return Circle{ x, y, r };
		}
	};
	Effector::Effector()
	{
		TextureAsset::Register(L"Effect", L"data/effect.png");
	}
	void Effector::addBreakEffect(Vec2 p)
	{
		struct Box
			:FieldObject
		{
			int time;
			double spd;
			double uvx;
			double uvy;
			Box(double x, double y, double ang, double r)
				:FieldObject(x, y), time(30), uvx(std::cos(ang)), uvy(std::sin(ang)), spd(r / 30)
			{}

			Sharp getSharp()const override{ return Circle{ 0 }; }
			void updata(stgpart::TaskMediator &task)override
			{
				x += uvx*spd;
				y += uvy*spd;

				Triangle{ { x, y }, 16, 0 }.draw(Color(0xFFffF1, 255 * (time / 30.)));
				--time;
				if (time < 0)alive = false;
			}
		};
		for (int i = 0; i < 32; ++i)
		{
			add(std::make_shared<Box>(p.x, p.y, 2.9 / 5 * i, 70. * i / 32));
		}
		
	}
	void Effector::addSpriteEffect(Vec2 p)
	{
		struct Box
			:FieldObject
		{
			int time;
			Box(double x, double y, double ang)
				:FieldObject(x, y), time(255)
			{}

			Sharp getSharp()const override{ return Circle{ 0 }; }
			void updata(stgpart::TaskMediator &task)override
			{
				
				--time;
				if (time < 0)alive = false;
			}
		};
		add(std::make_shared<Box>(p.x, p.y, 0));
	}

	void Effector::addBombSE()
	{
		SoundAsset(L"BombSE").playMulti();
	}
	void Effector::addEnemyShotSE(){}
	void Effector::addPlayerShotSE(){}
	void Effector::addBreakSE(){}

	void CheckHit::updata(stgpart::TaskMediator&task)
	{
		//bomb vs enemy
		task.bombmaneger->intersects(*task.enemymane,
			[](Bomb&b, Enemy&e)
		{
			if (!e.isRed())
				e.addDamage(b.getDamage());
		});

		//bomb vs bullet
		task.bombmaneger->intersects(*task.bulletMane, 
			[&](Bomb&bomb, Bullet&bt)
		{
			if (!bt.isRed())
			{
				bt.onHitFlag();
				bt.push_bomb(*task.bombmaneger);
			}

		});

		//bullet vs player
		task.player->intersects(*task.bulletMane,
			[&](PlayerShip&p, Bullet&)
		{
			task.effector->addBreakEffect(p.getPos());
		});

		//enemy vs player
		task.player->intersects(*task.enemymane,
			[](PlayerShip&bt, Enemy&)
		{
			
		});

		//playerAtk vs enemy
		task.playerAtkmane->intersects(*task.enemymane,
			[&](PlayerAttack&atk, Enemy&e)
		{
			e.addDamage(atk.getDamage());
			task.effector->addSpriteEffect(atk.getPos());
			atk.onHitFlag();
		});


	}


	void BombManeger::add_basic_bomb(Vec2 const&pos,int r,int damage,int lifetime)
	{
		add(std::make_shared<MyBomb>(pos, damage, lifetime, r));
	}



	Drawer::Drawer()
	{
		TextureAsset::Register(L"player", L"data/myship.png");
	}
	void Drawer::drawPlayer(double x, double y)
	{
		TextureAsset(L"player").scale(0.5).drawAt(Vec2{ x, y });
	}


	void MotherShip::updata(TaskMediator&task)
	{
		//@TODO draw
		//task.player->
		if (task.player->isDeadState() && task.playerdata->getLife())
		{
			task.playerdata->lostLife();
			task.player->repop(task.motherShip->getPos());
		}		
	}
}