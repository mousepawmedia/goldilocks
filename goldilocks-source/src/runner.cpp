#include "goldilocks/runner.hpp"

void Runner::run()
{
	// run pre() from test. If it fails, call prefail()
	if (!this->test->pre()) {
		this->test->prefail();
		return;
	}

	for (uint16_t i = 0; i < this->iterations; ++i) {
		// run janitor() from test. If fails, call postmortem()
		if (!this->test->janitor()) {
			this->test->postmortem();
			return;
		}

		// run run() from test. If fails && test exits on failure, run
		// posrtmortem()
		if (!this->test->run()) {
			return;
		}  // TODO: If exit on fail.
	}

	this->test->post();
}
