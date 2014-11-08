#pragma once

#include"Luawrap.h"
#include"StgPart.h"
#include"FieldObjectDesign.h"

namespace stgpart
{
	class LuaBullet
		:public Bullet
	{
		int register_index;
		int design_handle = 0;
	public:
		LuaBullet(double x, double y, int register_index, std::shared_ptr<FieldObjectDesign>const&p, bool red)
			:Bullet(x, y, p,  red), register_index(register_index)
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
				if (lua_pcall(state->data(), 1, 4, 0))
				{
					std::cout << "LuaBullet:\t" << lua_tostring(state->data(), -1) << std::endl;
				}
				x = lua_tointeger(state->data(), -4);
				y = lua_tointeger(state->data(), -3);
				angle = lua_tonumber(state->data(), -2);
				auto des = lua_tointeger(state->data(), -1);
				if (design_handle != des)
				{
					design_handle = des;
					design = tasks.design->make_design(des);
				}
					
			}
			lua_settop(state->data(), stack_state);
		}

		void updata(TaskMediator& tasks)override
		{
			auto p = tasks.design->getNone();
			updata_script(tasks);

			design->draw(x, y, angle);
			if (!fieldRect().intersects(Point(x, y)))
			{
				alive = false;
			}
		}
		Sharp getSharp()const{ return design->getSharp(x, y, angle); }
		void onHitFlag() 
		{
			alive = false; 
		}
	};
	class LuaEnemy
		:public Bullet
	{
		int register_index;
		int design_handle = 0;
		int hp;
	public:
		LuaEnemy(double x, double y, int register_index, std::shared_ptr<FieldObjectDesign>const&p, bool red, bool hp)
			:Bullet(x, y, p, red), register_index(register_index), hp(hp)
		{
			isRedFlag = red;
		}
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
				if (lua_pcall(state->data(), 1, 4, 0))
				{
					std::cout << "LuaBullet:\t" << lua_tostring(state->data(), -1) << std::endl;
				}
				x = lua_tointeger(state->data(), -4);
				y = lua_tointeger(state->data(), -3);
				angle = lua_tonumber(state->data(), -2);
				auto des = lua_tointeger(state->data(), -1);
				if (design_handle != des)
				{
					design_handle = des;
					design = tasks.design->make_design(des);
				}

			}
			lua_settop(state->data(), stack_state);
		}

		void updata(TaskMediator& tasks)override
		{
			auto p = tasks.design->getNone();
			updata_script(tasks);

			design->draw(x, y, angle);
			if (!fieldRect().intersects(Point(x, y)))
			{
				alive = false;
			}
		}
		Sharp getSharp()const{ return design->getSharp(x, y, angle); }
		void onHitFlag()
		{
			alive = false;
		}
	};
}