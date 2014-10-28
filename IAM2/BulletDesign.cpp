#include<Siv3D.hpp>

class Egg
{
	void Draw(double x, double y)
	{
		Circle(x, y, 3).draw();
		Circle(x, y, 7).drawFrame();
	}
};