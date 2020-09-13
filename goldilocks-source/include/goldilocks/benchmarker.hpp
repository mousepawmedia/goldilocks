
#ifndef GOLDILOCKS_BENCHMARKER_HPP
#define GOLDILOCKS_BENCHMARKER_HPP

#include "goldilocks/test.hpp"

class Benchmarker
{
protected:
	Test* test;
	uint16_t iteration;

public:
	Benchmarker(Test* test, uint16_t iterations = 1)
	: test(test), iterations(iterations)
	{
	}

	void benchmark()
	{
		if (!this->test->pre()) {
			this->test->prefail() return;
		}

		if (!this->test->janitor()) {
			this->test->postmortem() return;
		}

		if (!this->test->run()) {
			this->test->postmortem() return;
		}

		if (!this->test->janitor()) {
			this->test->postmortem() return;
		}

		if (!this->test->run_optimized) {
			this->test->postmortem() return;
		}

		// Step 6: If there are still iterations of the test to be run,
		// return to step 4.
		for (uint16_t i = 0; i < this->iterations; ++i) {
			// run janitor() from test. If fails, call postmortem()
			if (!this->test->janitor()) {
				this->test->postmortem() return;
			}

		}

		this->test->post()
	}

	~Benchmarker() = default;
};

#endif  // GOLDILOCKS_BENCHMARKER_HPP