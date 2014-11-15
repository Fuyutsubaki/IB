#include"StgPart.h"

namespace stgpart
{
	class MyBomb
		:public Bomb
	{
		int const lifetime;
		int count = 0;
		double r;
		Sound se;
		Texture tex;
	public:
		MyBomb(Vec2 const&pos, int dmg, int lifetime, double r)
			:Bomb(pos.x, pos.y, dmg), lifetime(lifetime), r(r), tex(TextureAsset(L"Bomb"))
		{
			se = SoundAsset(L"Bomb");
			if(!se.isPlaying)
				se.play();
		}
		void updata(TaskMediator&task)override
		{

			tex.scale(2 * r / tex.height).rotate(count / 90 * Pi).drawAt(Vec2(x, y), Alpha(255 - (count * 255. / lifetime)));
			if (count<lifetime)
				++count;
			else
			{
				se.play();
				alive = false;
			}

		}
		Sharp getSharp()const
		{
			return Circle{ x, y, r };
		}
	};
	class MyBomb2
		:public Bomb
	{
		int const lifetime;
		int count = 0;
		double r;
		Texture tex;
	public:
		MyBomb2(Vec2 const&pos, int dmg, int lifetime, double r)
			:Bomb(pos.x, pos.y, dmg), lifetime(lifetime), r(r), tex(TextureAsset(L"Bomb"))
		{
			
		}
		void updata(TaskMediator&task)override
		{

			tex.scale(2 * r / tex.height).rotate(count / 90 * Pi).drawAt(Vec2(x, y), Alpha(255 - (count * 255. / lifetime)));
			if (count<lifetime)
				++count;
			else
			{
				alive = false;
			}

		}
		Sharp getSharp()const
		{
			return Circle{ x, y, r };
		}
	};

	Effector::Effector()
	{
		TextureAsset::Register(L"Effect", L"data/effect.png");
		SoundAsset::Register(L"EnemyShot", L"data/sht1.mp3", false);
		SoundAsset::Register(L"PlayerShot", L"data/sht3.mp3", false);
		SoundAsset::Register(L"EnemyBreak", L"data/EnemyBreak.mp3", false);
		SoundAsset::Register(L"Bomb", L"data/Bomb.mp3", false);
		SoundAsset::Register(L"BGM", L"data/akebono.mp3", true);
		bgm = SoundAsset(L"BGM");

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
		SoundAsset(L"Bomb").play();
	}
	void Effector::addEnemyShotSE()
	{
		SoundAsset(L"EnemyShot").play();
	}
	void Effector::addPlayerShotSE()
	{
		SoundAsset(L"PlayerShot").play();
	}
	void Effector::addBreakSE()
	{
		SoundAsset(L"EnemyBreak").playMulti();
	}

	void Effector::beginBGM()
	{
		bgm.play();

	}
	void Effector::endBGM()
	{
		bgm.stop();
	}
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
			[&](PlayerShip&p, Bullet&b)
		{
			if (!p.isStar())
			{
				p.onHitFlag();
				task.effector->addBreakEffect(p.getPos());
				b.onHitFlag();
			}
		});

		//enemy vs player
		task.player->intersects(*task.enemymane,
			[&](PlayerShip&p, Enemy&e)
		{
			if (!p.isStar())
			{
				p.onHitFlag();
				task.effector->addBreakEffect(p.getPos());
				e.addDamage(500);
			}

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


	void BombManeger::add_basic_bomb(Vec2 const&pos, int r, int damage, int lifetime)
	{
		add(std::make_shared<MyBomb>(pos, damage, lifetime, r));
	}

	void BombManeger::add_sub_bomb(Vec2 const&pos, int r, int damage, int lifetime)
	{
		add(std::make_shared<MyBomb>(pos, damage, lifetime, r));
	}

	Drawer::Drawer()
	{
		TextureAsset::Register(L"player", L"data/myship.png");
		TextureAsset::Register(L"Bomb", L"data/Bomb.png");
	}
	void Drawer::drawPlayer(double x, double y)
	{
		TextureAsset(L"player").scale(0.5).drawAt(Vec2{ x, y });
	}

	MotherShip::MotherShip()
		:FieldObject(120, 360)
	{
		TextureAsset::Register(L"MotherShip", L"data/MotherShip.png");
	}
	void MotherShip::updata(TaskMediator&task)
	{
		//@TODO draw
		/*	{
			int x0 = x - 20;
			int y0 = y - 20;
			Rect{ x0, y0, 40, 40 }.drawFrame(1, 0, Palette::Red);
			for (int i = 1; i < 4; ++i)
			{
			Line{ Vec2{ x0, y0 + i * 5 }, Vec2{ x0 + 40 - i * 10, y0 + i * 5 } }.draw(1, Palette::Red);
			Line{ Vec2{ x0, y0 + 40 - i * 5 }, Vec2{ x0 + 40 - i * 10, y0 + 40 - i * 5 } }.draw(1, Palette::Red);
			}
			}*/
		++count;

		TextureAsset(L"MotherShip").scale(std::sin(count*Pi / 120) / 4 + 1).rotate(count*Pi / 90).drawAt(Vec2{ x, y });


		if (task.player->isDeadState() && task.playerdata->getLife())
		{
			task.playerdata->lostLife();
			task.player->repop(task.motherShip->getPos());
		}
	}
}