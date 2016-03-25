
#ifndef TEST_H
#define TEST_H

#include <cassert>

class Test {
	Test() {
		run_test();
	}

	virtual ~Test() {
	}

	virtual void run_test() = 0;

};


#define TEST(NAME) class NAME : public Test { }; NAME :: run_test()

#endif // TEST_H

