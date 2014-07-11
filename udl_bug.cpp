// /home/ed/bin/bin/g++ -std=c++11 -o udl_bug udl_bug.cpp

#include <iostream>

class Int {
public:
	constexpr Int(long long unsigned value)
		: m_value(value)
	{}
	const long long unsigned m_value;
};

constexpr Int operator "" _i(long long unsigned i) {
	return Int(i);
}

int main(int argc, char** argv) {
	std::cout << 1_i.m_value;
	return 0;
}

