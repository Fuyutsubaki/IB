#pragma once

#include<lua.hpp>
#include<memory>
#include<iostream>
#include<string>
namespace luawrap
{
	struct Lua
	{
		struct deleter
		{
			void operator()(lua_State*p)
			{
				lua_close(p);
			}
		};
		std::unique_ptr<lua_State, deleter> state;
		lua_State*data()
		{
			return state.get();
		}
		explicit Lua(lua_State*p)
			:state(p)
		{}
		Lua()
			:state(luaL_newstate())
		{
			luaL_openlibs(data());
		}

		void set_global(const char*name, double d)
		{
			lua_pushnumber(data(), d);
			lua_setglobal(data(), name);
		}

		void push(double d){ lua_pushnumber(data(), d); }
		void push(int n){ lua_pushinteger(data(), n); }

		template<class Type>
		typename Type::type get_global(const char*name)
		{
			lua_getglobal(data(), name);
			return Type::get(*this, -1);
		}

		void load_file(const std::string&str)
		{
			if (luaL_loadfile(data(), str.data()) || lua_pcall(data(), 0, 0, 0))
				std::cout << str << ":syntax error";
		}

		template<class...T>
		void call(char * const name, T const &...args)
		{
			//stackÇÃèÛë‘Çï€ë∂Ç∑ÇÈ
			int stack_state = lua_gettop(data());
			{
				lua_getglobal(data(), name);
				(void) std::initializer_list<int>{[&]{push(args); return 0; }()...};
				if (lua_pcall(data(), sizeof...(T), 0, 0))
				{
					auto err = lua_tostring(data(), -1);
					std::cout << err;
				}
			}
			lua_settop(data(), stack_state);
		}

		template<class Type,class Func>
		void foreach_table(char * const name, Func f)
		{
			lua_getglobal(data(), name);
			lua_pushnil(data());
			while (lua_next(data(), -2))
			{
				f(Type::get(*this, -1));
				lua_pop(data(), 1);
			}
			lua_pop(data(), 1);
		}

		int makeRegisterIndex()
		{
			lua_newtable(data());
			return luaL_ref(data(), LUA_REGISTRYINDEX);
		}
		void unrefRegisterIndex(int idx)
		{
			luaL_unref(data(), LUA_REGISTRYINDEX, idx);
		}

		void registerCfunc(const char*name,int(*p)(lua_State*) )
		{
			lua_register(data(), name, p);
		}
	};
	struct Num
	{
		using type = lua_Number;
		static bool checkType(Lua&lua, int index)
		{
			return !!lua_isnumber(lua.data(), index);
		}
		static void push(Lua&lua, type d)
		{
			lua_pushnumber(lua.data(), d);
		}
		static type get(Lua&lua, int index)
		{
			return lua_tonumber(lua.data(), index);
		}
	};
}