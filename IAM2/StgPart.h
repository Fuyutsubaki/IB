#pragma once
#include<list>
#include<memory>

#include"Luawrap.h"
#include"Input.h"
namespace stgpart
{
	class PlayerShipManeger;
	class BulletManeger;
	class PlayerAtackManeger;
	class BombManeger;
	class EnemyManeger;

	struct TaskMediator
	{
		TaskMediator(
		std::shared_ptr<PlayerShipManeger> playerMane,
		std::shared_ptr<BulletManeger>bulletMane,
		std::shared_ptr<PlayerAtackManeger>playerAtkmane,
		std::shared_ptr<BombManeger>bombmaneger,
		std::shared_ptr<EnemyManeger>enemymane,
		std::shared_ptr<luawrap::Lua> lua,
		std::shared_ptr<InputKey> input)
		:playerMane(playerMane),
		bulletMane(bulletMane),
		playerAtkmane(playerAtkmane),
		bombmaneger(bombmaneger),
		enemymane(enemymane),
		lua(lua),
		input(input)
		{}
		TaskMediator(){}
		std::shared_ptr<PlayerShipManeger> playerMane = nullptr;
		std::shared_ptr<BulletManeger>bulletMane = nullptr;
		std::shared_ptr<PlayerAtackManeger>playerAtkmane = nullptr;
		std::shared_ptr<BombManeger>bombmaneger = nullptr;
		std::shared_ptr<EnemyManeger>enemymane = nullptr;
		std::shared_ptr<luawrap::Lua> lua = nullptr;
		std::shared_ptr<InputKey> input = nullptr;
	};
	namespace impl
	{
		template<class ElementType>
		class BasicListManeger
			:public MediatorTask
		{
			template<class T>
			using Containor = std::list<T>;

			std::list<std::shared_ptr<ElementType>> list;
		public:
			void updata(TaskMediator&mediator)override
			{
				for (auto& x : list)
					x->updata(mediator);
				list.erase(
					std::remove_if(list.begin(), list.end(), [](std::shared_ptr<ElementType>const&x){return !x->isAlive(); })
					, list.end());
			}
			bool isAlive()const override
			{
				return true;
			}
			void add(std::shared_ptr<ElementType>const&p)
			{
				list.push_front(p);
			}
			std::list<std::shared_ptr<ElementType>>& getList()
			{
				return list;
			}

		};

		template<class ElementType>
		class BasicVectorManeger
			:public MediatorTask
		{
			template<class T>
			using Containor = std::vector<T>;

			std::list<std::shared_ptr<ElementType>> list;
		public:
			void updata(TaskMediator&mediator)override
			{
				for (auto& x : list)
					x->updata(mediator);
				list.erase(
					std::remove_if(list.begin(), list.end(), [](std::shared_ptr<ElementType>const&x){return !x->isAlive(); })
					, list.end());
			}
			bool isAlive()const override
			{
				return true;
			}
			void add(std::shared_ptr<ElementType>const&p)
			{
				list.push_back(p);
			}
			std::list<std::shared_ptr<ElementType>>& getList()
			{
				return list;
			}

		};
	}

	class MediatorTask
	{
	public:
		virtual void updata(TaskMediator&data) = 0;
		virtual bool isAlive()const = 0;
		virtual ~MediatorTask(){}
	};

	class PlayerShip
		:public MediatorTask
	{
	public:
		virtual Circle getSharp() = 0;
		virtual void onHitFlag()=0;
	};

	class PlayerShipManeger
		:public impl::BasicListManeger<PlayerShip>
	{	};

	class Enemy
		:public MediatorTask
	{
	public:
		virtual ~Enemy(){}
		virtual Circle getSharp() = 0;
		virtual void onHitFlag() = 0;
	};

	class EnemyManeger
		:public impl::BasicListManeger<Enemy>
	{	};

	class Bullet
		:public MediatorTask
	{
	public:
		virtual ~Bullet(){}
		virtual Circle getSharp() = 0;
		virtual void onHitFlag() = 0;
	};

	class BulletManeger
		:public impl::BasicListManeger<Bullet>
	{	};

	class CheckHit
		:public MediatorTask
	{
	public:
		CheckHit()
		{}
		void updata(TaskMediator&task)override
		{
			//Å“K‰»‚µ‚æ‚¤‚º
			for (auto x : task.bulletMane->getList())
			{
				for (auto y : task.playerMane->getList())
				{
					if (x->getSharp().intersects(y->getSharp()))
					{
						y->onHitFlag();
					}
				}
			}
		}
		bool isAlive()const override{ return true; }
	};

	class PlayerAttack
		:public MediatorTask
	{
	public:
		virtual Circle getSharp() = 0;
	};
	class PlayerAtackManeger
		:public impl::BasicVectorManeger<PlayerAttack>
	{	};

	class CheckHitAtkBt
		:public MediatorTask
	{
	public:
		void updata(TaskMediator&tasks)override
		{
			for (auto&atk : tasks.playerAtkmane->getList())
			{
				for (auto&bt : tasks.bulletMane->getList())
				{
					if (atk->getSharp().intersects(bt->getSharp()))
					{
						bt->onHitFlag();
					}
				}
			}
		}
		bool isAlive()const
		{
			return true;
		}
	};

	class Bomb
		:public MediatorTask
	{
	public:
		virtual Circle getSharp() = 0;
	};
	class BombManeger
		:public impl::BasicVectorManeger<Bomb>
	{	};

	class HitBEBt
		:MediatorTask
	{
	public:
		void updata(TaskMediator&tasks)override
		{
			for (auto&bomb : tasks.bombmaneger->getList())
			{
				for (auto&bt : tasks.bulletMane->getList())
				{
					if (bomb->getSharp().intersects(bt->getSharp()))
					{
						bt->onHitFlag();
					}
				}
			}
		}
		bool isAlive()const override
		{
			return true;
		}
	};

	class checkHitBomb
		:public MediatorTask
	{
	public:
		void updata(TaskMediator&tasks)override
		{
			for (auto&atk : tasks.bombmaneger->getList())
			{
				for (auto&bt : tasks.bulletMane->getList())
				{
					if (atk->getSharp().intersects(bt->getSharp()))
					{
						bt->onHitFlag();
					}
				}
			}
		}
		bool isAlive()const
		{
			return true;
		}
	};

	struct StgField
	{
		bool in(int x, int y)const
		{
			return true;
		}
	};
}