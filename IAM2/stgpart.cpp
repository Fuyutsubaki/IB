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
}