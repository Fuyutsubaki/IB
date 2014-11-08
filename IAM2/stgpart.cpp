#include"StgPart.h"

namespace stgpart
{
	void Effector::addBreakEffect(Vec2 p)
	{
		struct Box
			:FieldObject
		{
			Box(double x, double y, double ang)
			:FieldObject(x, y)
			{}
			Sharp getSharp()const override{ return Circle{ 0 }; }
			void updata(stgpart::TaskMediator &task)override
			{
				task.drawer->DrawCricre(x, y, 3);
			}
		};
		add(std::make_shared<Box>(p.x, p.y, 0));
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
		task.bulletMane->intersects(*task.playerMane,
			[&](Bullet&, PlayerShip&bt)
		{

		});

		//enemy vs player
		task.enemymane->intersects(*task.playerMane,
			[](Enemy&, PlayerShip&bt)
		{
			
		});

		//playerAtk vs enemy
		task.playerAtkmane->intersects(*task.enemymane,
			[](PlayerAttack&atk, Enemy&e)
		{
			e.addDamage(atk.getDamage());
		});


	}
}