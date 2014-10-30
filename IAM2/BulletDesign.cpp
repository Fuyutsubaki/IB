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

	Circle(x, y, 3).draw(Palette::Red);

}

namespace stgpart
{
	struct BTB :FieldObjectDesign
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
	struct Egg :FieldObjectDesign
	{
		Sharp getSharp(double x, double y, double)override{ return{ Circle{ x, y, 7 } }; }
		void draw(double x, double y, double)override
		{
			Circle(x, y, 7).draw(Palette::Black);
			Circle(x, y, 12).drawFrame(0.5, 0.5, Palette::Black);
		}
	};
	std::shared_ptr<FieldObjectDesign> FieldObjectDesignFactory::make_design(int n)
	{
		switch (n)
		{
			//None
		case 0:
		
			struct None :FieldObjectDesign
			{
				Sharp getSharp(double, double, double)override{ return{ Circle{ 0 } }; }
				void draw(double, double, double)override	{  }
			};

			return std::make_shared<None>();
		case 1:
			
			return std::make_shared<BTB>();
		case 2:
			
			return std::make_shared<Egg>();
		default:
			return error;
		}
	}


}

