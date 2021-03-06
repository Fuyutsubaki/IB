#pragma once
#include"StgPart.h"


namespace stgpart
{
	class FieldObjectDesignFactory
	{
		struct Error :FieldObjectDesign
		{
			Sharp getSharp(double, double, double)override{ return{ Circle{ 0 } }; }
			void draw(double, double, double)override	
			{
				Circle{ 20 }.draw(Palette::Red);
			}
			void push_bomb(double, double, BombManeger&)override
			{}
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