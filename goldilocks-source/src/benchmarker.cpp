#include "goldilocks/benchmarker.hpp"

void Benchmarker::benchmark()
{
	if (!this->test->pre()) {
		this->test->prefail();
		return;
	}

	if (!this->test->janitor()) {
		this->test->postmortem();
		return;
	}

	if (!this->test->run()) {
		this->test->postmortem();
		return;
	}

	if (!this->test->janitor()) {
		this->test->postmortem();
		return;
	}

	if (!this->test->run_optimized()) {
		this->test->postmortem();
		return;
	}

	// Step 6: If there are still iterations of the test to be run,
	// return to step 4.
	for (uint16_t i = 0; i < this->iterations; ++i) {
		// run janitor() from test. If fails, call postmortem()
		if (!this->test->janitor()) {
			this->test->postmortem();
			return;
		}
	}

	this->test->post();
}
