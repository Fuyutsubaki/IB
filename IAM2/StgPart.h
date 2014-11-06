#pragma once
#include<list>
#include<memory>

#include"Luawrap.h"
#include"Input.h"
#include<boost\variant.hpp>
namespace stgpart
{
	struct Sharp
	{
		boost::variant<RectF, Circle> data;
		struct Intersects_v :boost::static_visitor<bool>
		{
			template<class Lhs, class Rhs>
			bool operator()(Lhs const&lhs, Rhs const&rhs)const
			{
				return lhs.intersects(rhs);
			}
		};
		struct EndPoint_v :boost::static_visitor<double>
		{
			inline double operator()(RectF const&a)const
			{
				return a.x + a.w;
			}
			inline double operator()(Circle const&a)const
			{
				return a.x + a.r;
			}
		};
	public:
		inline static Sharp makeRect(double x, double y, double w, double h){ return RectF{ x, y, w, h }; }
		inline static Sharp makeCircle(double x, double y, double r){ return Circle{ x, y, r }; }
		template<class T>
		Sharp(T const& a)
			:data(a){}

		inline bool intersects(Sharp const&rhs)const
		{
			return boost::apply_visitor(Intersects_v{}, data, rhs.data);
		}

		inline double endPoint()const
		{
			EndPoint_v v;
			return data.apply_visitor(v);
		}
	};


	class PlayerData;
	class PlayerShipManeger;
	class BulletManeger;
	class PlayerAtackManeger;
	class BombManeger;
	class Drawer;
	struct FieldObjectDesignFactory;
	class Effector;
	class MotherShip;
	

	struct TaskMediator
	{
		TaskMediator(
		std::shared_ptr<PlayerShipManeger>const& playerMane,
		std::shared_ptr<BulletManeger>const&bulletMane,
		std::shared_ptr<PlayerAtackManeger>const&playerAtkmane,
		std::shared_ptr<BombManeger>const&bombmaneger,
		std::shared_ptr<Drawer>const&drawer,
		std::shared_ptr<luawrap::Lua>const& lua,
		std::shared_ptr<InputKey>const& input,
		std::shared_ptr<PlayerData>const&playerdata,
		std::shared_ptr<FieldObjectDesignFactory>const&design,
		std::shared_ptr<MotherShip>const&motherShip
		)
		:playerMane(playerMane),
		bulletMane(bulletMane),
		playerAtkmane(playerAtkmane),
		bombmaneger(bombmaneger),
		drawer(drawer),
		lua(lua),
		input(input),
		playerdata(playerdata),
		design(design),
		motherShip(motherShip)
		, effector(std::make_shared<Effector>())
		{}
		TaskMediator(){}
		std::shared_ptr<PlayerShipManeger> playerMane = nullptr;
		std::shared_ptr<BulletManeger>bulletMane = nullptr;
		std::shared_ptr<PlayerAtackManeger>playerAtkmane = nullptr;
		std::shared_ptr<BombManeger>bombmaneger = nullptr;
		std::shared_ptr<Drawer>drawer;
		std::shared_ptr<luawrap::Lua> lua = nullptr;
		std::shared_ptr<InputKey> input = nullptr;
		std::shared_ptr<PlayerData>playerdata = nullptr;
		std::shared_ptr<FieldObjectDesignFactory>design;
		std::shared_ptr<MotherShip>motherShip;
		std::shared_ptr<Effector>effector;
	};
	namespace impl
	{
		template<class ElementType>
		struct ListPolicy
		{
			using element_type = ElementType;
			std::list<std::shared_ptr<ElementType>> list;
			template<class Pred>
			void sort(Pred f)
			{
				list.sort(f);
			}
		};
		template<class ElementType>
		struct VectorPolicy
		{
			using element_type = ElementType;
			std::vector<std::shared_ptr<ElementType>> list;
			template<class Pred>
			void sort(Pred f)
			{
				std::sort(list.begin(), list.end(), f);
			}
		};

		template<class Policy>
		class BasicFieldObjectManeger
			:protected Policy, public MediatorTask
		{
			using element_type = typename Policy::element_type;
			
		public:
			decltype(list)&getList(){ return list; }
			void updata(TaskMediator&mediator)override
			{
				for (auto& x : list)
					x->updata(mediator);
				list.erase(
					std::remove_if(list.begin(), list.end(), [](std::shared_ptr<element_type>const&x){return !x->isAlive(); })
					, list.end());

				sort([](std::shared_ptr<element_type>&lhs, std::shared_ptr<element_type>&rhs)
				{return lhs->getSharp().endPoint() < rhs->getSharp().endPoint();	});
				
			}
			bool isAlive()const override
			{
				return true;
			}
			void add(std::shared_ptr<element_type>const&p)
			{
				list.push_back(p);
			}
			template<class Other,class F>
			void intersects(Other&other, F f)
			{
				for (auto&x : list)
				{
					for (auto &y : other.getList())
					{
						if (x->getSharp().intersects(y->getSharp()))
							f(*x, *y);

					}
				}
			}
		};
		/*template<class ElementType>
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
				
				list.sort(
					[](std::shared_ptr<ElementType>&lhs, std::shared_ptr<ElementType>&rhs)
				{return lhs->getSharp().endPoint() < rhs->getSharp().endPoint();	}
				);
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

			Containor<std::shared_ptr<ElementType>> list;
		public:
			void updata(TaskMediator&mediator)override
			{
				for (auto& x : list)
					x->updata(mediator);
				list.erase(
					std::remove_if(list.begin(), list.end(), [](std::shared_ptr<ElementType>const&x){return !x->isAlive(); })
					, list.end());
				std::sort(list.begin(), list.end(),
					[](std::shared_ptr<ElementType>&lhs, std::shared_ptr<ElementType>&rhs)
				{return lhs->getSharp().endPoint() < rhs->getSharp().endPoint();	}
				);
			}
			bool isAlive()const override
			{
				return true;
			}
			void add(std::shared_ptr<ElementType>const&p)
			{
				list.push_back(p);
			}
			Containor<std::shared_ptr<ElementType>>& getList()
			{
				return list;
			}

		};*/
	}

	class MediatorTask
	{
	public:
		virtual void updata(TaskMediator&data) = 0;
		virtual bool isAlive()const = 0;
		virtual ~MediatorTask(){}
	};
	
	
	class FieldObjectDesign
	{
	public:
		virtual Sharp getSharp(double x, double y, double angle) = 0;
		virtual void draw(double x, double y, double angle) = 0;
		virtual~FieldObjectDesign(){}
	};
	class FieldObject
		:public MediatorTask
	{
	protected:
		inline static Rect const& fieldRect()
		{
			static const Rect rect = Rect{ 0 - 30, 0 - 30, 480 + 30, 728 + 30 };
			return rect;
		}
		double x;
		double y;
		double angle;
		bool alive;
	public:
		Vec2 getPos()const{ return{ x, y }; }
		void setPos(Vec2 const&xy)
		{
			x = xy.x;
			y = xy.y;
		}
		FieldObject(double x, double y)
			:x(x), y(y), alive(true)
		{}
		
		FieldObject(FieldObject const&) = delete;
		virtual Sharp getSharp()const = 0;
		bool isAlive()const override
		{
			return alive;
		}
		virtual ~FieldObject(){}

	};

	

	class Effector
		:public impl::BasicFieldObjectManeger<impl::VectorPolicy<FieldObject>>
	{

	public:
		
		void addBreakEffect(Vec2 p);
	};
	class PlayerShip
		:public FieldObject
	{
	public:
		PlayerShip(double x, double y)
			:FieldObject(x, y)
		{}
		virtual void onHitFlag()=0;
		virtual~PlayerShip(){}
	};

	class PlayerShipManeger
		:public  impl::BasicFieldObjectManeger<impl::ListPolicy<PlayerShip>>
	{	};

	class Bullet
		:public FieldObject
	{
	protected:
		int hp;
		std::shared_ptr<FieldObjectDesign> design;
	public:
		using FieldObject::FieldObject;
		Bullet(double x, double y, int hp, std::shared_ptr<FieldObjectDesign>const&p)
			:FieldObject(x, y), hp(hp), design(p)
		{}
		virtual ~Bullet(){}
		virtual void onHitFlag() = 0;
	};

	class BulletManeger
		:public  impl::BasicFieldObjectManeger<impl::ListPolicy<Bullet>>
	{	};

	class CheckHit
		:public MediatorTask
	{
	public:
		CheckHit()
		{}
		void updata(TaskMediator&tasks)override
		{
			tasks.playerMane->intersects(*tasks.bulletMane,
				[](PlayerShip&p, Bullet&)
			{
				p.onHitFlag();
			});
		}
		bool isAlive()const override{ return true; }
	};

	class PlayerAttack
		:public FieldObject
	{
	public:
		PlayerAttack(double x, double y)
			:FieldObject(x, y)
		{}
	};
	class PlayerAtackManeger
		:public impl::BasicFieldObjectManeger<impl::VectorPolicy<PlayerAttack>>
	{	};

	class CheckHitAtkBt
		:public MediatorTask
	{
	public:
		void updata(TaskMediator&tasks)override
		{
			tasks.playerAtkmane->intersects(*tasks.bulletMane,
				[&](PlayerAttack&, Bullet&bt)
			{
				tasks.effector->addBreakEffect(bt.getPos());
				bt.onHitFlag();
			});
		}
		bool isAlive()const
		{
			return true;
		}
	};

	class Bomb
		:public FieldObject
	{
	public:
		Bomb(double x, double y)
			:FieldObject(x, y)
		{}
	};
	class BombManeger
		:public impl::BasicFieldObjectManeger<impl::VectorPolicy<Bomb>>
	{	};

	class checkHitBomb
		:public MediatorTask
	{
	public:
		void updata(TaskMediator&tasks)override
		{
			tasks.bombmaneger->intersects(*tasks.bulletMane,
				[](Bomb&, Bullet&bt)
			{
				bt.onHitFlag();
			});
		}
		bool isAlive()const
		{
			return true;
		}
	};

	class Drawer
	{
	public:
		void DrawCricre(double x, double y, int r);

	};

	class MotherShip
		:public FieldObject
	{
	public :
		Sharp getSharp()const{ return Circle{ 0 }; }
		MotherShip()
			:FieldObject(180, 20)
		{}
		void updata(TaskMediator&task)override
		{}
	};


	struct PlayerData
	{
	public:
		PlayerData()
			:kill_list(60, 0)
		{	}
		int life;
		long long unsigned int score = 0;
		void updata()
		{
			kill_score -= kill_list.front();
			kill_list.pop_front();
			kill_score += now_kill;
			kill_list.push_back(now_kill);




			if (kill_score > 3)extend();
			now_kill = 0;
		}
		void addKill()		{ ++now_kill; }
	private:
		int now_kill = 0;
		std::list<int> kill_list;
		int kill_score = 0;
		void extend()
		{
			//‰¹‚Æ‚©‚È‚ç‚µ‚½‚¢
			life += now_kill;
		}
	};



	
	
}