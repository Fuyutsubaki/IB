#pragma once
#include"StgPart.h"


namespace stgpart
{
	class FieldObjectDesignFactory
	{
		struct Error :FieldObjectDesign
		{
			Sharp getSharp(double x, double, double ang)override{ return{ Circle{ 0 } }; }
			void draw(double x, double y, double angle)override	
			{
				Circle{ 20 }.draw(Palette::Red);
			}
		};
		std::shared_ptr<FieldObjectDesign> error;
	public:
		FieldObjectDesignFactory()
		{
			error = std::make_shared<Error>();
		}
		
		std::shared_ptr<FieldObjectDesign>getNone()
		{
			return error;
		}

		std::shared_ptr<FieldObjectDesign> make_design(int);
	};


}