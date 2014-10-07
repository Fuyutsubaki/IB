#pragma once
#include<Siv3D.hpp>
#include<algorithm>
#include<boost\range\algorithm\sort.hpp>
#include<utility>

	class RoughCollisionDetection_t
	{
		template<class L, class R>
		static bool AA(const L&lhs, const R&rhs)
		{
			return (lhs.getmin() < rhs.getmin() && rhs.getmin() < lhs.getmax()) || (lhs.getmin() < rhs.getmax() && rhs.getmax() < lhs.getmax());
		}
		template<class L, class R>
		static bool isLeft(const L&lhs, const R&rhs)
		{
			return lhs.getmax() < rhs.getmin();
		}
		template<class LargeList, class SmallList, class Func>
		void check(LargeList&large, SmallList&small, const Func&func)const
		{
			auto hint = large.begin();
			using large_onj_type = decltype(*hint);
			for (auto &s : small)
			{
				hint = std::find_if(hint, large.end(), [&](const large_onj_type&obj){return !isLeft(s, obj); });
				for (auto it = hint; it != large.end(); ++it)
				{
					if (AA(*it, s))
						func(*it, s);
					else break;
				}
			}
		}
		struct Comp
		{
			template<class T>
			bool operator()(const T&lhs,const T&rhs)const
			{
				return lhs.getmin() < rhs.getmin();
 			}
		};
	public:
		template<class List0, class List1, class Func>
		void operator()(List0&a, List1&b, const Func&f)const
		{
			boost::range::sort(a, Comp{});
			boost::range::sort(b, Comp{});

			if (a.size() < b.size())
			{
				namespace ph = std::placeholders;
				using atype = decltype(*a.begin());
				using btype = decltype(*b.begin());
				check(b, a, [&](btype&b, atype&a){return f(a, b); });
			}
			else
				check(a, b, f);
		}
	};
	static const RoughCollisionDetection_t checkCollisionDetection;
}