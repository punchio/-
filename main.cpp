#include <tuple>
#include <iostream>
#include <type_traits>

using namespace std;

enum EventType {
	E0,
	E1,
	E2,
	E3,
	E4,
};

#define EVENT_TUPLE(e, type) tuple<integral_constant<decltype(e), e>, type>

typedef tuple<  EVENT_TUPLE(E1, int),
	EVENT_TUPLE(E4, long),
	EVENT_TUPLE(E2, int*),
	EVENT_TUPLE(E3, char),
	void> TUPLE;

#define TUPLE_TYPE(t, index) typename std::tuple_element<index, t>::type
#define TUPLE_MAX (std::tuple_size<TUPLE>::value - 1)

template<EventType e, size_t index>
struct Finder {
	typedef TUPLE_TYPE(TUPLE_TYPE(TUPLE, index), 0) EnumType;
	typedef TUPLE_TYPE(TUPLE_TYPE(TUPLE, index), 1) ElemType;

	typedef std::is_same<std::integral_constant<decltype(e), e>, EnumType> IsSame;
	typedef ElemType TrueType;
	typedef typename Finder<e, index + 1>::Result FalseType;
	typedef typename std::conditional<IsSame::value, TrueType, FalseType>::type Result;
};

template<EventType e>
struct Finder<e, TUPLE_MAX> {
	typedef void Result;
};

#define FIND_TYPE(e) Finder<e, 0>::Result
int main() {
	std::cout << typeid(FIND_TYPE(E1)).name() << std::endl;
	return 0;
}
