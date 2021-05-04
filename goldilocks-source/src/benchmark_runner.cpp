#include "goldilocks/benchmark_runner.hpp"
#include "goldilocks/clock.hpp"

void BenchmarkRunner::run()
{
	// Initialize test
	if (!this->test->pre()) {
		this->test->prefail();
		return;
	}
	// Validate that test runs.
	if (!this->test->run_optimized()) {
		this->test->postmortem();
		return;
	}
	// Initialize comparative
	if (!this->comparative->pre()) {
		this->comparative->prefail();
		this->test->post();
		return;
	}

	if (!this->comparative->run_optimized()) {
		this->comparative->postmortem();
		this->test->post();
		return;
	}

	// Actual benchmarking
	for (uint16_t i = 0; i < this->iterations; ++i) {
		if (!this->test->janitor()) {
			this->test->postmortem();
			this->comparative->post();
			return;
		}

		if (!this->comparative->janitor()) {
			this->comparative->postmortem();
			this->test->post();
			return;
		}

		// clock() calls test->run_optimized() and comparative->run_optimized()
		this->results.add_measurement(clock(this->test),
									  clock(this->comparative));
	}

	this->test->post();
	this->comparative->post();
}
