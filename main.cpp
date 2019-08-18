#include <tuple>
#include <vector>
#include <iostream>
#include <type_traits>
#include <functional>

using namespace std;


enum EventType {
	E0,
	E1,
	E2,
	E3,
	E4,
};

template<EventType e, typename... Args>
class Signal
{
public:
	typedef int (*Func)(Args...);
	void bind(Func func)
	{
		cout << "Enum:" << e << " bind." << endl;
		m_slots.push_back(func);
	}

	/* 重载操作符 -> signal触发机制 */
	void operator()(Args... args)
	{
		cout << "Enum:" << e << " call." << endl;
		for (auto s: m_slots)
		{
			s(args...);
		}
	}

	static Signal<e, Args...>& instance() {
		static Signal<e, Args...> sig;
		return sig;
	}
private:
	std::vector<Func>m_slots;
};

int call1(int p1) {
	std::cout << "call1:" << p1 << endl;
	return 0;
}

int call2(int p1, int p2) {
	std::cout << "call2:" << p2 << endl;
	return 0;
}

int call3(int p1, int p2, int p3) {
	std::cout << "call3:" << p3 << endl;
	return 0;
}

#define EVENT_TUPLE(e, ...) tuple<integral_constant<EventType, e>, Signal<e, __VA_ARGS__>>

typedef tuple<  EVENT_TUPLE(E1, int),
				EVENT_TUPLE(E4, int),
				EVENT_TUPLE(E2, int, int),
				EVENT_TUPLE(E3, int, int, int),
void> TUPLE;

#define TUPLE_TYPE(t, index) typename std::tuple_element<index, t>::type
#define TUPLE_MAX (std::tuple_size<TUPLE>::value - 1)

template<EventType e, size_t index>
struct Finder {
	typedef TUPLE_TYPE(TUPLE_TYPE(TUPLE, index), 0) EnumType;
	typedef TUPLE_TYPE(TUPLE_TYPE(TUPLE, index), 1) ElemType;

	typedef std::is_same<std::integral_constant<EventType, e>, EnumType> IsSame;
	typedef ElemType TrueType;
	typedef typename Finder<e, index+1>::Result FalseType;
	typedef typename std::conditional<IsSame::value, TrueType, FalseType>::type Result;
};

template<EventType e>
struct Finder<e, TUPLE_MAX> {
	typedef void Result;
};

#define EVENT_BIND(e, func) {Finder<e, 0>::Result::instance().bind(func);}
#define EVENT_CALL(e, ...) {Finder<e, 0>::Result::instance()(__VA_ARGS__);}

int main() {
	EVENT_BIND(E1, call1);
	//EVENT_CALL(E1, 1, 2);
	//EVENT_BIND(E1, call2);
	EVENT_CALL(E1, 1);
	EVENT_BIND(E2, call2);
	EVENT_CALL(E2, 1, 2);
	EVENT_BIND(E3, call3);
	EVENT_CALL(E3, 1, 2, 3);
	EVENT_BIND(E4, call1);
	EVENT_CALL(E1, 1);
	EVENT_CALL(E4, 4);
	return 0;
}