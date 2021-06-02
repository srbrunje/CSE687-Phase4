#ifndef __EXAMPLE_TEST_H
#define __EXAMPLE_TEST_H

#include <csignal>

template<typename T>
class AddFunctor {
public:
	T _A, _B, _Ans;

	AddFunctor(T A, T B, T Ans) : _A{ A }, _B{ B }, _Ans{ Ans } {}
	bool operator()() {
		return _A + _B == _Ans;
	}
};

template<typename T>
class SubFunctor {
	T _A, _B, _Ans;

public:
	SubFunctor(T A, T B, T Ans) : _A{ A }, _B{ B }, _Ans{ Ans } {}
	bool operator()() {
		return _A - _B == _Ans;
	}
};

template<typename T>
class MulFunctor {
	T _A, _B, _Ans;

public:
	MulFunctor(T A, T B, T Ans) : _A{ A }, _B{ B }, _Ans{ Ans } {}
	bool operator()() {
		return _A * _B == _Ans;
	}
};

template<typename T>
class DivFunctor {
	T _A, _B, _Ans;

public:
	DivFunctor(T A, T B, T Ans) : _A{ A }, _B{ B }, _Ans{ Ans } {}
	bool operator()() {
		if (_B == 0) {
			throw std::exception("Divide by zero exception");
		}
		return _A / _B == _Ans;
	}
};

template<typename T>
class SegFunctor {
	T _A, _B, _Ans;

public:
	SegFunctor(T A, T B, T Ans) : _A{ A }, _B{ B }, _Ans{ Ans } {}
	bool operator()() {
		
		std::raise(SIGSEGV); //throw a seg fault

		return false;
	}
};


class LongRunFunctor{
	int _A, _B;

public:
	LongRunFunctor(int A, int B = 500) : _A{ A }, _B{ B }{}
	bool operator()() {
		if(_A < 1) _A = 1;
		if(_B < 200) _B = 200;

		for (int i = 0; i < _A; i++)
		{
			::Sleep(_B);
		}
		return true;
	}
};

#endif // !__EXAMPLE_TEST_H
