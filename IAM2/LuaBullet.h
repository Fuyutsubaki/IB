#pragma once

#include"Luawrap.h"
#include"StgPart.h"

namespace stgpart
{
	class LuaBullet
		:public Bullet
	{
		int register_index;
	public:
		LuaBullet(double x, double y, int hp, int register_index)
			:Bullet(x, y, hp), register_index(register_index)
		{	}
		void pushSelf(luawrap::Lua&state)
		{
			lua_rawgeti(state.data(), LUA_REGISTRYINDEX, register_index);
		}
		
		void updata_script(TaskMediator& tasks)
		{
			auto state = tasks.lua;
			//stack�̏�Ԃ�ۑ�����
			int stack_state = lua_gettop(state->data());
			{
				pushSelf(*state);
				lua_getfield(state->data(), -1, "updata");

				pushSelf(*state);
				if (lua_pcall(state->data(), 1, 2, 0))
				{
					auto err = lua_tostring(state->data(), -1);
					std::cerr << err;
				}
				x = lua_tointeger(state->data(), -2);
				y = lua_tointeger(state->data(), -1);
			}
			lua_settop(state->data(), stack_state);
		}
		void updata(TaskMediator& tasks)override
		{
			updata_script(tasks);

			tasks.drawer->DrawCricre(x, y, 3);

			if (!fieldRect().intersects(Point(x, y)))
				alive = false;
		}
		
		Circle getSharp()const override { return Circle(x, y, 3); }
		void onHitFlag() 
		{
			alive = false; 
		}
	};

}