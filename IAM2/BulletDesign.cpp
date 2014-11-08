#include<Siv3D.hpp>
#include"StgPart.h"
#include"FieldObjectDesign.h"

void stgpart::Drawer::DrawCricre(double x, double y, int r)
{
	//Circle(x, y, 7).draw(Palette::Black);
	//Circle(x, y, 12).drawFrame(0.5, 0.5, Palette::Black);

	/*Circle(x, y + 4, 3).draw(Palette::Black);
	Circle(x + 4, y, 3).draw(Palette::Black);
	Circle(x - 4, y, 3).draw(Palette::Black);
	Circle(x, y - 4, 3).draw(Palette::Black);*/

	/*Circle(x, y, 12).draw(Palette::Black);
	static const double root3 = std::sqrt(3);
	Triangle(x, y, 12*root3).draw(Palette::White);*/

	Circle(x, y, r).draw(Palette::Red);

}




namespace stgpart
{
	class bomb_local
		:public Bomb
	{
		bomb_local()
		:Bomb(0, 0, 1000)
		{
			
		}
		void updata(TaskMediator&)
		{}

	};
	class mini_bulletDesign
		:public FieldObjectDesign
	{
	public:
		void push_bomb(BombManeger&mane)override
		{
			
		}
	};
	class neetbomb
		:public Bomb
	{
		neetbomb()
		:Bomb(0, 0,0)
		{
			alive = false;
		}
		void updata(TaskMediator&)
		{}

	};
	class EnemyDesign
		:public FieldObjectDesign
	{
	public:
		void push_bomb(BombManeger&mane)override
		{

		}
	};

	struct BTB :mini_bulletDesign
	{
		int count = 0;
		Sharp getSharp(double x, double y, double)override
		{
			return Circle(x, y, 12);
		}
		void draw(double x, double y, double angle)override
		{
			++count;
			Circle(x, y, 12).draw(Palette::Black);
			static const double root3 = std::sqrt(3);
			Triangle(x, y, 12 * root3, (count *s3d::Math::Pi / 60 + angle)).draw(Palette::White);
		}
		
	};
	struct Egg :mini_bulletDesign
	{
		Sharp getSharp(double x, double y, double)override{ return{ Circle{ x, y, 7 } }; }
		void draw(double x, double y, double)override
		{
			Circle(x, y, 7).draw(Palette::Black);
			Circle(x, y, 12).drawFrame(0.5, 0.5, Palette::Black);
		}
	};

	struct Mini :mini_bulletDesign
	{
		Sharp getSharp(double x, double y, double)override{ return{ Circle{ x, y, 3 } }; }
		void draw(double x, double y, double)override
		{
			Circle(x, y, 3).draw(Palette::Black);
		}
	};

	struct BoxEmy :EnemyDesign
	{
		Sharp getSharp(double x, double y, double)override{ return{ RectF{ x + 3, y + 3, 24, 24 } }; }
		void draw(double x, double y, double)override
		{
			using V = Vec2;
			V c{ x, y };
			static V const r{ 15, 15 };
			static V const cir{ 0, 25 };
			static V const in{ 12, 12 };
			RectF{ c - r, 2 * r }.draw(Palette::Black);
			
			Line{ c - cir, c + cir }.draw(1, Palette::Black);
			Circle{ c - cir, 3 }.drawFrame(1, 0, Palette::Black);
			Circle{ c + cir, 3 }.drawFrame(1, 0, Palette::Black);
			RectF{ c - in, 2 * in }.drawFrame(3, 0, Palette::White);
		}
	};
	std::shared_ptr<FieldObjectDesign> FieldObjectDesignFactory::make_design(int n)
	{
		switch (n)
		{
			//None
		case 0:
		
			struct None :mini_bulletDesign
			{
				Sharp getSharp(double, double, double)override{ return{ Circle{ 0 } }; }
				void draw(double, double, double)override	{  }
				
			};

			return std::make_shared<None>();
		case 1:
			
			return std::make_shared<BTB>();
		case 2:
			
			return std::make_shared<Egg>();
		case 3:
			return std::make_shared<Mini>();


		case 101:
			return std::make_shared<BoxEmy>();
		default:
			return error;
		}
	}


}

