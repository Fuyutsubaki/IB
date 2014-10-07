#pragma once

#include"Luawrap.h"
#include"StgPart.h"

namespace stgpart
{
	class LuaBullet
		:public Bullet
	{
		int register_index;
		double x;
		double y;
		bool alive;
		
	public:
		LuaBullet(double x, double y, int register_index)
			:x(x), y(y), alive(true), register_index(register_index)
		{	}
		void pushSelf(luawrap::Lua&state)
		{
			lua_rawgeti(state.data(), LUA_REGISTRYINDEX, register_index);
		}
		
		void updata_script(TaskMediator& tasks)
		{
			auto state = tasks.lua;
			//stack‚Ìó‘Ô‚ð•Û‘¶‚·‚é
			int stack_state = lua_gettop(state->data());
			{
				pushSelf(*state);
				lua_getfield(state->data(), -1, "updata");

				pushSelf(*state);
				if (lua_pcall(state->data(), 1, 2, 0))
				{
					auto err = lua_tostring(state->data(), -1);
				}
				x = lua_tointeger(state->data(), -2);
				y = lua_tointeger(state->data(), -1);
			}
			lua_settop(state->data(), stack_state);
		}
		void updata(TaskMediator& tasks)override
		{
			updata_script(tasks);

			Circle(x, y, 3).draw();
		}
		bool isAlive()const override
		{
			return alive;
		}
		Circle getSharp() { return Circle(x, y, 3); }
		void onHitFlag() 
		{
			alive = false; 
		}
	};
}