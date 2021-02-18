#include "goldilocks/manager.hpp"

// MACRO IF we are using a GCC-style compiler.
// NOTE: We're assuming Intel/AMD. What about PowerPC and ARM?
#if defined __GNUC__ || __MINGW32__ || __MINGW64__
// If we're on a 64-bit system...
#if defined __LP64__

void TestManager::calibrate()
{
	/* According to Intel's documentation, we must "warms up"
	 * the instruction cache that we'll be using for measurement,
	 * to improve accuracy.*/

	uint32_t low, high;

	/* Run the following commands three times to warm up the
	instructions cache.*/
	for (uint16_t i = 0; i < 3; i++) {
		asm volatile("cpuid;"
					 "rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

		asm volatile("rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");
	}
}

inline uint64_t TestManager::clock(Test* test)
{
	uint32_t low, high;
	uint64_t cyc1, cyc2;

	// Serialize initially (we won't do it later, though.)
	asm volatile("cpuid;" ::: "%rax", "%rbx", "%rcx", "%rdx");
	// Get the initial timestamp (all 64 bits).
	asm volatile("rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%rax", "%rdx");
	// Put the high and low halves of the timestamp together and store.
	cyc1 = ((uint64_t)high << 32) | low;

	/* If we have a test, run it. (If there is no test, we will just
	 * wind up measuring the measurement instructions by themselves.*/
	if (test != NULL)
		test->run();

	/* Get the second timestamp. DO NOT SERIALIZE! The CPUID instruction
	 * is too unpredictable in terms of execution size. We can just
	 * assume that an out-of-order execution will yield an outlier
	 * measurement.*/
	asm volatile(/*"cpuid;"*/
				 "rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%rax", "%rdx");

	// Put the high and low halves of the timestamp together and store.
	cyc2 = ((uint64_t)high << 32) | low;

	// Return the difference between the AFTER and BEFORE timestamps.
	return (cyc2 - cyc1);
}

// If we're on a 32-bit system...
#else
/* NOTE: This seems to be assuming we're not on a 16-bit system. Do we
 * need to factor that in?*/

/* See the comments for the 64-bit versions of these functions. The only
 * difference between the 32-bit and 64-bit versions is in which
 * registers we are clobbering. On x86 (32-bit), we clobber %eax-%edx,
 * while on x64, we clobber %rax-%rdx.*/
void TestManager::calibrate()
{
	uint32_t low, high;

	for (uint16_t i = 0; i < 3; i++) {
		asm volatile("cpuid;"
					 "rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%eax", "%ebx", "%ecx", "%edx");

		asm volatile("rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%eax", "%ebx", "%ecx", "%edx");
	}
}

inline uint64_t TestManager::clock(Test* test)
{
	uint32_t low, high;
	uint64_t cyc1, cyc2;

	asm volatile("cpuid;" ::: "%eax", "%ebx", "%ecx", "%edx");
	asm volatile("rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%eax", "%edx");
	cyc1 = ((uint64_t)high << 32) | low;
	if (test != NULL)
		test->run_optimized();
	asm volatile(/*"cpuid;"*/
				 "rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%eax", "%edx");

	cyc2 = ((uint64_t)high << 32) | low;

	return (cyc2 - cyc1);
}

#endif

/* MACRO ELSE if we're not on a GCC, just don't offer the rdtsc functions;
 * offer dummies instead. We will NOT be supporting MSVC under ANY
 * circumstances!*/
#else
void TestManager::calibrate() { return; }

inline uint64_t TestManager::clock(Test* test) { return; }

#endif

void TestManager::list_tests(bool showTitles)
{
	/* Loop through all the indexes in the map `tests`...
	 * SOURCE: http://stackoverflow.com/a/110255/472647
	 */
	for (std::map<testname_t, testptr_t>::iterator it = tests.begin();
		 it != tests.end();
		 ++it) {
		// Print out the index to IOSqueak channel.
		channel << it->first;
		// If we're supposed to also display the title...
		if (showTitles) {
			// Show the title.
			channel << ": " << it->second->get_title();
		}
		// Newline.
		channel << IOCtrl::n;
	}
	channel << IOCtrl::endl;
}

void TestManager::list_suites(bool showTitles)
{
	/* Loop through all the indexes in the map `suites`...
	 * SOURCE: http://stackoverflow.com/a/110255/472647
	 */
	for (std::map<testsuitename_t, testsuiteptr_t>::iterator it =
			 suites.begin();
		 it != suites.end();
		 ++it) {
		// Print out the index to IOSqueak.
		channel << it->first;
		// If we're supposed to also display the title...
		if (showTitles) {
			// Show the title.
			channel << ": " << it->second->get_title();
		}
		// Newline.
		channel << IOCtrl::n;
	}
	channel << IOCtrl::endl;
}

void TestManager::show_docs(testname_t test_name)
{
	// Ensure the test DOES NOT already exist before continuing.
	if (!validate(test_name, true, GolidlocksItemType::test)) {
		return;
	}
	channel << IOFormatTextAttr::bold << tests[test_name]->get_title() << "["
			<< test_name << "]: " << IOCtrl::end;
	channel << tests[test_name]->get_docs() << IOCtrl::endl;
}

bool TestManager::i_confirm(bool default_option)
{
	// Get what the user typed.
	std::string buffer;
	getline(std::cin, buffer);

	// If yes was selected...
	if (buffer == "y" || buffer == "Y") {
		return true;
	}
	// Otherwise, if no was selected...
	else if (buffer == "n" || buffer == "N") {
		return false;
	}
	// For anything else, use the default...
	return default_option;
}

void TestManager::i_load_suite(testsuitename_t suite)
{
	// If no suite was specified (load all)...
	if (suite == "") {
		channel << "Load ALL test suites? (y/N) " << IOCtrl::end;

		if (i_confirm(false)) {
			load_suite();
		}
		return;
	}

	// Otherwise, if a suite was specified...

	// Ensure the test exists before continuing.
	if (!validate(suite, true, GolidlocksItemType::suite)) {
		return;
	}

	if (!suites[suite]->is_loaded()) {
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red
				<< "ERROR: The suite " << suite
				<< " is not yet loaded. Aborting." << IOCtrl::endl;
		return;
	}

	channel << "Load test suite " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << suites[suite]->get_title()
			<< IOCtrl::end;
	channel << "[" << suite << "]? (y/N) " << IOCtrl::end;

	if (i_confirm(false)) {
		load_suite(suite);
	}
}

void TestManager::i_run(testname_t item, unsigned int repeat)
{
	// Don't make this yell on fail...let the next call do it!
	if (validate(item, false, GolidlocksItemType::test)) {
		i_run_test(item, repeat);
	} else if (validate(item, true, GolidlocksItemType::suite)) {
		i_run_suite(item);
	}
}

void TestManager::i_run_test(testname_t test, unsigned int repeat)
{
	// Ensure the test exists before continuing.
	if (!validate(test, true, GolidlocksItemType::test)) {
		return;
	}

	channel << "Run test " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << tests[test]->get_title()
			<< IOCtrl::end;
	channel << "[" << test << "]" << IOCtrl::end;
	if (repeat > 1) {
		channel << " " << stringify(repeat, IOFormatBase::dec) << " times"
				<< IOCtrl::end;
	}
	channel << "? (y/N) " << IOCtrl::end;

	if (i_confirm(false)) {
		run_test(test, repeat);
	}
}

void TestManager::i_run_suite(testsuitename_t suite)
{
	// Ensure the test exists before continuing.
	if (!validate(suite, true, GolidlocksItemType::suite)) {
		return;
	}

	if (!suites[suite]->is_loaded()) {
		/* If the suite isn't already loaded, just load it.
		 * If load fails, give up and return.
		 */
		if (!load_suite(suite)) {
			return;
		}
	}

	channel << "Run test suite " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << suites[suite]->get_title()
			<< IOCtrl::end;
	channel << "[" << suite << "]? (y/N) " << IOCtrl::end;

	if (i_confirm(false)) {
		run_suite(suite);
	}
}

void TestManager::i_run_benchmark(testname_t test, unsigned int repeat)
{
	// Ensure the test exists before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(test, true, GolidlocksItemType::test) ||
		!validate(test, true, GolidlocksItemType::comparative)) {
		return;
	}

	channel << "Run comparative benchmark between " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << tests[test]->get_title()
			<< IOCtrl::end;
	channel << "[" << test << "] and " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << comparatives[test]->get_title()
			<< IOCtrl::end;
	channel << " at " << repeat << " repetitions? (y/N) " << IOCtrl::end;

	if (i_confirm(false)) {
		run_benchmark(test, repeat);
	}
}

void TestManager::i_run_compare(testname_t test1,
								testname_t test2,
								unsigned int repeat,
								bool showResults)
{
	// Ensure the test exists before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(test1, true, GolidlocksItemType::test) ||
		!validate(test2, true, GolidlocksItemType::test)) {
		return;
	}

	channel << "Run comparative benchmark between " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << tests[test1]->get_title()
			<< IOCtrl::end;
	channel << "[" << test1 << "] and " << IOCtrl::end;
	channel << IOFormatTextAttr::bold << tests[test2]->get_title()
			<< IOCtrl::end;
	channel << "[" << test2 << "] at " << repeat << " repetitions? (y/N) "
			<< IOCtrl::end;

	if (i_confirm(false)) {
		run_compare(test1, test2, repeat, showResults);
	}
}

bool TestManager::load_suite(testsuitename_t suite)
{
	/* ENTRY: Run a suite of tests.*/

	// If no suite is specified, load all available.
	if (suite == "") {
		/* Loop through all the indexes in the map `suites`...
		 * SOURCE: http://stackoverflow.com/a/110255/472647
		 */
		for (std::map<testsuitename_t, testsuiteptr_t>::iterator it =
				 suites.begin();
			 it != suites.end();
			 ++it) {
			// Load each suite that isn't already loaded.
			if (it->second->is_loaded() == false) {
				it->second->load_tests();
				it->second->is_loaded(true);
				channel << IOCat::normal << IOFormatTextAttr::bold << it->first
						<< ": " << it->second->get_title() << " loaded."
						<< IOCtrl::endl;
			}
		}
		return true;
	}

	// Otherwise, if we specified a test...

	// Ensure the suite exists before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(suite, true, GolidlocksItemType::suite)) {
		return false;
	}

	// Ensure the suite isn't already loaded...
	if (!suites[suite]->is_loaded()) {
		// Load the suite into the testmanager.
		suites[suite]->load_tests();
		// Mark the suite as loaded.
		suites[suite]->is_loaded(true);
		// Report the good news.
		channel << IOCat::normal << IOFormatTextAttr::bold
				<< suites[suite]->get_title() << IOCtrl::endl;
		channel << IOCat::normal << IOFormatTextAttr::bold
				<< IOFormatTextBG::green << IOFormatTextFG::white
				<< "Suite loaded." << IOCtrl::n << IOCtrl::endl;
		return true;
	} else {
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red
				<< "ERROR: The suite " << suite << " is already loaded."
				<< IOCtrl::endl;
		return false;
	}
	return false;
}

bool TestManager::run(testname_t item, unsigned int number)
{
	// Don't make this yell on fail...let the next conditional do it!
	if (validate(item, false, GolidlocksItemType::test)) {
		return run_test(item, number);
	} else if (validate(item, true, GolidlocksItemType::suite)) {
		return run_suite(item);
	}
	// If the item doesn't exist, return false.
	return false;
}

bool TestManager::runall()
{
	/* Loop through all the indexes in the map `suites`...
	 * SOURCE: http://stackoverflow.com/a/110255/472647
	 */
	for (std::map<testsuitename_t, testsuiteptr_t>::iterator it =
			 suites.begin();
		 it != suites.end();
		 ++it) {
		// Run each suite.
		if (!run_suite(it->first)) {
			// Abort on first failure.
			return false;
		}
	}
	return true;
}

bool TestManager::run_suite(testsuitename_t suite)
{
	/* ENTRY: Run a suite of tests.*/

	// Ensure the suite exists before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(suite, true, GolidlocksItemType::suite)) {
		return false;
	}

	if (!suites[suite]->is_loaded()) {
		/* If the suite isn't already loaded, just load it.
		 * If load fails, give up and return false.
		 */
		if (!load_suite(suite)) {
			return false;
		}
	}

	// Display the suite name in a banner.
	channel << "===== [" << suites[suite]->get_title()
			<< "] =====" << IOCtrl::endl;

	// Run the suite, and return whether it succeeded.
	bool r = suites[suite]->run_tests();

	// If the suite run fails (return false)...
	if (!r) {
		channel << IOCtrl::n << IOCtrl::clear << IOCat::error
				<< IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "SUITE FAILED" << IOCtrl::endl;

		// End of suite, fail.
		return false;
	}
	// If the suite run succeeds (return true)...
	else {
		// Let the user know that all went well.
		channel << IOCtrl::n << IOCtrl::clear << IOCat::normal
				<< IOFormatTextAttr::bold << IOFormatTextBG::green
				<< IOFormatTextFG::white << "SUITE COMPLETE" << IOCtrl::endl;

		// End of suite, success.
		return true;
	}
}

void TestManager::register_test(testname_t test_name, Test* test, Test* compare)
{
	/* ENTRY: Add a new test by name to the test manager. */

	// Ensure the test DOES NOT already exists before continuing.
	// If it DOES...
	if (validate(test_name, false, GolidlocksItemType::test)) {
		// Throw an error.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red
				<< "ERROR: A test by the name of " << test_name
				<< "is already registered in Golidlocks Test Manager."
				<< IOCtrl::endl;
		return;
	}

	/* We generally advise the end-developer to pass `new Test` as
	 * the second parameter. Thus, if the allocation fails (or they
	 * otherwise pass in a null pointer)...*/
	if (test == nullptr) {
		// Display an error message.
		channel << IOCat::error << "TestManager: Unable to create a new "
				<< "instance of the test \"" << test_name << "\". Test not "
				<< "registered." << IOCtrl::endl;

		// Return non-fatally.
		return;
	}

	/* Add the new test to the TestManager's map (tests), with the name
	 * string as the key, and a smart pointer (unique_ptr) to the test.
	 * emplace() allows us to define the new unique_ptr within the map,
	 * as insert() would literally NOT work (you can't copy a unique_ptr).*/
	tests.emplace(test_name, testptr_t(test));

	/* If a comparative test was included... */
	if (compare != nullptr) {
		/** Add the comparative test to the comparatives map using the
		 * test name as the key.
		 */
		comparatives.emplace(test_name, testptr_t(compare));
	}

	/* WARNING: The end-developer must be sure they aren't trying to
	 * retain ownership of the Test instance, as that will cause UB
	 * (in my observation, usually a segfault) when the TestManager
	 * instance is destroyed - the unique_ptr will not be able to
	 * destroy the instance. It's not pretty.*/
}

void TestManager::register_suite(testsuitename_t suite_name, TestSuite* suite)
{
	/* ENTRY: Add a new test by name to the test manager. */

	// Tell the suite instance which testmanager instance is controlling it.
	suite->backregister(this);

	// Ensure the suite DOES NOT already exist before continuing.
	// If it does...
	if (validate(suite_name, false, GolidlocksItemType::suite)) {
		// Throw an error.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextFG::red
				<< "ERROR: A suite by the name of " << suite_name
				<< " is already registered in Golidlocks Test Manager."
				<< IOCtrl::endl;
		return;
	}

	/* We generally advise the end-developer to pass `new Test` as
	 * the second parameter. Thus, if the allocation fails (or they
	 * otherwise pass in a null pointer)...*/
	if (suite == nullptr) {
		// Display an error message.
		channel << IOCat::error << "TestManager: Unable to create a new "
				<< "instance of the suite \"" << suite_name << "\". Suite not "
				<< "registered." << IOCtrl::endl;

		// Return non-fatally.
		return;
	}

	/* Add the new suite to the TestManager's map (suites), with the name
	 * string as the key, and a smart pointer (unique_ptr) to the suite.
	 * emplace() allows us to define the new unique_ptr within the map,
	 * as insert() would literally NOT work (you can't copy a unique_ptr).*/
	suites.emplace(suite_name, testsuiteptr_t(suite));

	/* WARNING: The end-developer must be sure they aren't trying to
	 * retain ownership of the TestSuite instance, as that will cause UB
	 * (in my observation, usually a segfault) when the TestManager
	 * instance is destroyed - the unique_ptr will not be able to
	 * destroy the instance. It's not pretty.*/
}

bool TestManager::run_test(testname_t test, unsigned int repeat)
{
	/* ENTRY: Run a single test.*/

	// Ensure the test exists before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(test, true, GolidlocksItemType::test)) {
		return false;
	}

	/* We should only allow repeating between 1 and 10,000 times,
	 * inclusively. If the user asked for more than that...*/
	if (repeat < 1 || repeat > 10000) {
		// Display an error.
		channel << IOCat::error
				<< "Test repeat value must be between 1 and 10000 inclusively."
				<< IOCtrl::endl;

		// Abort the test.
		return false;
	}

	// Display the test name in a banner.
	channel << "===== [" << tests[test]->get_title()
			<< "] =====" << IOCtrl::endl;

	/* Attempt to run the pretest function, which is intended to set up
	 * for multiple runs of the test. If that fails (returns false)...*/
	if (!(tests[test]->pre())) {
		// Alert the user with an error message.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "PRE-TEST FAILED - ABORTING"
				<< IOCtrl::endl;

		// Run the function to clean up after a pretest fail.
		tests[test]->prefail();

		// End of test.
		return false;
	}

	bool status = true;

	for (unsigned int i = 0; i < repeat; ++i) {
		channel << IOCat::normal << "Pass "
				<< stringify(i + 1, IOFormatBase::dec) << " of "
				<< stringify(repeat, IOFormatBase::dec) << IOCtrl::endl;

		tests[test]->janitor();

		// Run the test. If it fails (returned false)...
		if (!(tests[test]->run()) || !(tests[test]->verify())) {
			status = false;
			break;
		}
	}
	// If the test failed...
	if (!status) {
		// Alert the user with an error message.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "TEST FAILED" << IOCtrl::endl;

		// Run the test's fail-cleanup function.
		tests[test]->postmortem();

		// End of test.
		return false;
	}
	// If the test run succeeds (return true)...
	else {
		// Let the user know that all went well.
		channel << IOCat::normal << IOFormatTextAttr::bold
				<< IOFormatTextBG::green << IOFormatTextFG::white
				<< "TEST COMPLETE" << IOCtrl::endl;

		// Run the test's cleanup (post) function.
		tests[test]->post();

		// End of test.
		return true;
	}
}

void TestManager::benchmark_banner()
{
	// Display the fancy benchmark banner, with its disclaimer.
	channel << IOFormatTextAttr::bold << IOFormatTextFG::blue << "======"
			<< "==========="
			<< "=====\n"
			<< "|     "
			<< "BENCHMARKER"
			<< "    |\n"
			<< "======"
			<< "==========="
			<< "=====" << IOCtrl::endl;
	channel
		<< IOFormatTextAttr::none << IOFormatTextFG::red
		<< "Disclaimer: This benchmarker is intended to give a rough estimate "
		<< "of how many CPU cycles a particular test takes. It is not a "
		<< "replacement for a fully-featured code profiler. "
		<< "Cache warming will NOT be performed.\n"
		<< IOCtrl::endl;
}

bool TestManager::run_benchmark(testname_t test,
								unsigned int repeat,
								bool showStats,
								bool deadHeat)
{
	// Ensure both tests exist before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(test, true, GolidlocksItemType::test) ||
		!validate(test, true, GolidlocksItemType::comparative)) {
		return -1;
	}

	return run_compare(tests[test].get(),
					   comparatives[test].get(),
					   repeat,
					   showStats,
					   deadHeat);
}

bool TestManager::run_compare(testname_t test1,
							  testname_t test2,
							  unsigned int repeat,
							  bool showStats,
							  bool deadHeat)
{
	// Ensure both tests exist before continuing.
	/* NOTE: If you're getting a segfault in this function, check here!
	 * Attempting to access the functions in an unregistered test causes
	 * Undefined Behavior, typically a segfault. */
	if (!validate(test1, true, GolidlocksItemType::test) ||
		!validate(test2, true, GolidlocksItemType::test)) {
		return false;
	}

	return run_compare(tests[test1].get(),
					   tests[test2].get(),
					   repeat,
					   showStats,
					   deadHeat);
}

bool TestManager::run_compare(Test* test1,
							  Test* test2,
							  unsigned int repeat,
							  bool showStats,
							  bool deadHeat)
{
	/* ENTRY: Compare two function benchmarks.
	 * Largely based on run_benchmark, with several
	 * additions for the different comparison types.
	 */

	/* We should only allow repeating between 10 and 10,000 times,
	 * inclusively. If the user asked for more than that...*/
	if (repeat < 10 || repeat > 10000) {
		// Display an error.
		channel << IOCat::error << "Benchmark requires a repeat value "
				<< "between 10-10000 (inclusively)." << IOCtrl::endl;

		// Abort the benchmark.
		return false;
	}

	// Display the fancy benchmarker banner and disclaimer.
	benchmark_banner();
	// Display the names of the tests we're about to compare.
	channel << "===== [" << test1->get_title() << "] | [" << test2->get_title()
			<< "] =====" << IOCtrl::endl;

	// Attempt to run the "pre" function for test 1. If it fails...
	if (!(test1->pre())) {
		// Alert the user with an error message...
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "[" << test1->get_title()
				<< "] PRE-TEST FAILED - ABORTING" << IOCtrl::endl;
		// Perform pre-fail cleanup on test 1.
		test1->prefail();

		// Don't worry about test 2 - it hasn't been set up yet.

		// Abort the benchmarker.
		return false;
	}

	// Attempt to run the "pre" function for test 2. If it fails...
	if (!(test2->pre())) {
		// Alert the user with an error message...
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "[" << test2->get_title()
				<< "] PRE-TEST FAILED - ABORTING" << IOCtrl::endl;

		// Perform pre-fail cleanup on test 2.
		test2->prefail();

		/* Perform standard cleanup (post) on test 1. Otherwise, we can
		 * potentially wind up with memory leaks and other issues.*/
		test1->post();

		// Abort the benchmarker.
		return false;
	}

	// Let the user know what we're doing...
	channel << "Ensuring [" << test1->get_title()
			<< "] succeeds before benchmarking..." << IOCtrl::endl;

	test1->janitor();

	// Run test1 to make sure it works. If it fails...
	if (!(test1->run()) || !(test1->verify())) {
		// Alert the user with an error message.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "[" << test1->get_title()
				<< "] FAILED - ABORTING" << IOCtrl::endl;

		// Run postmortem cleanup on test1.
		test1->postmortem();

		/* Perform standard cleanup (post) on test 2. Otherwise, we can
		 * potentially wind up with memory leaks and other issues.*/
		test2->post();

		// Abort the benchmarker.
		return false;
	}
	// Otherwise, if test 1 succeeds...
	else {
		// Let the user know the good news...
		channel << IOCat::normal << IOFormatTextAttr::bold
				<< IOFormatTextBG::green << IOFormatTextFG::white << "["
				<< test1->get_title() << "] PASSED" << IOCtrl::endl;
		// ...and move on.
	}

	// Give the user a status update.
	channel << "Ensuring [" << test2->get_title()
			<< "] succeeds before benchmarking..." << IOCtrl::endl;

	test2->janitor();

	// Run test2 to make sure it works. If it fails...
	if (!(test2->run()) || !(test2->verify())) {
		// Alert the user with an error message.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white << "[" << test2->get_title()
				<< "] FAILED - ABORTING" << IOCtrl::endl;

		// Run postmortem cleanup on test2.
		test2->postmortem();

		/* Perform standard cleanup (post) on test 1. Otherwise, we can
		 * potentially wind up with memory leaks and other issues.*/
		test1->post();

		// Abort the benchmarker.
		return false;
	}
	// Otherwise, if test 2 succeeds...
	else {
		// Let the user know the good news about that as well...
		channel << IOCat::normal << IOFormatTextAttr::bold
				<< IOFormatTextBG::green << IOFormatTextFG::white << "["
				<< test2->get_title() << "] PASSED" << IOCtrl::endl;
		// ...and let the fun proceed!
	}

	// Let the user know that we're starting our measurements.
	channel << IOCat::normal << IOFormatTextBG::cyan << IOFormatTextFG::white
			<< "STARTING BENCHMARK" << IOCtrl::endl;

	/* Shut off all testing messages; they aren't useful after the initial
	 * test run has finished. We don't want to clog up or clutter output.*/
	channel.shut_up(IOCat::testing);

	/* The average execution time of the measurement functions, represented
	 * as a 64-bit unsigned integer.*/
	uint64_t base;
	/* The BenchmarkResult instances for storing the output of all our
	 * measurements.*/
	BenchmarkResult mama1, mama2, papa1, papa2, baby1, baby2, baseR;

	// Dynamically allocate two arrays for storing measurements.
	uint64_t* results1 = new uint64_t[repeat];
	uint64_t* results2 = new uint64_t[repeat];

	// If we were unable to allocate either (or both) array...
	if (results1 == nullptr || results2 == nullptr) {
		// Alert the user with an error message.
		channel << IOCat::error << IOFormatTextAttr::bold << IOFormatTextBG::red
				<< IOFormatTextFG::white
				<< "Cannot allocate results arrays. Aborting." << IOCtrl::endl;

		if (results1 != nullptr) {
			delete[] results1;
		}

		if (results2 != nullptr) {
			delete[] results2;
		}

		// Abort the benchmarker.
		return false;
	}

	// Calibrate our measurement functions. (See that function's comments.)
	calibrate();

	/* Take <repeat> baseline measurements (measuring the measurement
	 * function itself.)*/
	for (unsigned int i = 0; i < repeat; ++i) {
		/* Keep the user apprised of our progress. We use `IOCtrl::endc` so we
		 * keep overwriting the same line.*/
		channel << IOCat::normal
				<< "CALIBRATING: " << (((i + 1) / repeat) * 100) << "%"
				<< IOCtrl::endc;
		// Get a baseline measurement.
		results1[i] = clock();
	}

	// Move to a new line for output.
	channel << IOCtrl::endl;

	/* Calculate the statistical results from our array of baseline
	 * measurements.*/
	resultFromArray(baseR, results1, repeat);
	/* Store the average baseline measurement for offsetting all our later
	 * measurements, thereby measuring JUST the test.*/
	base = baseR.mean;

	/* MAMA BEAR (cold cache) alternates the tests A-B-A-B-A-B, to maximize
	 * the possibility of cache misses on each individual test run.*/

	// Display MAMA BEAR banner.
	channel << IOCat::normal << "\n"
			<< IOFormatTextBG::cyan << IOFormatTextFG::white
			<< "COMPARISON 1/3: MAMA BEAR" << IOCtrl::endl;
	channel << IOCat::normal << "(MAMA BEAR) Loading..." << IOCtrl::endc;

	// Take <repeat> measurements of test A and B.
	for (unsigned int i = 0; i < repeat; ++i) {
		// Display progress (overwrite line.)
		channel << IOCat::normal << "(MAMA BEAR) Pass " << (i + 1) << "-A of "
				<< repeat << ".  " << IOCtrl::endc;
		// Run pre-repeat cleanup on test1.
		test1->janitor();
		// Record test 1 measurement, offset by baseline.
		results1[i] = clock(test1) - base;
		// Run verification on test1.
		test1->verify();

		// Display progress (overwrite line.)
		channel << IOCat::normal << "(MAMA BEAR) Pass " << (i + 1) << "-B of "
				<< repeat << ".  " << IOCtrl::endc;
		// Run pre-repeat cleanup on test2.
		test2->janitor();
		// Record test 2 measurement, offset by baseline.
		results2[i] = clock(test2) - base;
		// Run verification on test2.
		test2->verify();
	}

	// Move to a new line for output.
	channel << IOCtrl::endl;

	// Calculate the statistical results for MAMA BEAR, tests 1 and 2.
	resultFromArray(mama1, results1, repeat);
	resultFromArray(mama2, results2, repeat);

	/* PAPA BEAR (hot cache) performs all runs of test A before test B, to
	 * minimize the possibility of cache misses on each individual test run;
	 * in other words, allowing natural cache warming to occur.*/

	// Display PAPA BEAR banner.
	channel << IOCat::normal << "\n"
			<< IOFormatTextBG::cyan << IOFormatTextFG::white
			<< "COMPARISON 2/3: PAPA BEAR" << IOCtrl::endl;
	channel << IOCat::normal << "(PAPA BEAR) Loading..." << IOCtrl::endc;

	// Take <repeat> measurements of test A.
	for (unsigned int i = 0; i < repeat; ++i) {
		// Display progress (overwrite line).
		channel << IOCat::normal << "(PAPA BEAR) Pass " << (i + 1) << "-A of "
				<< repeat << ".  " << IOCtrl::endc;
		// Run pre-repeat cleanup on test1.
		test1->janitor();
		// Record test 1 measurement, offset by baseline.
		results1[i] = clock(test1) - base;
		// Run verification on test1.
		test1->verify();
	}

	// Take <repeat> measurements of test B.
	for (unsigned int i = 0; i < repeat; ++i) {
		// Display progress (overwrite line).
		channel << IOCat::normal << "(PAPA BEAR) Pass " << (i + 1) << "-B of "
				<< repeat << ".  " << IOCtrl::endc;
		// Run pre-repeat cleanup on test2.
		test2->janitor();
		// Record test 2 measurement, offset by baseline.
		results2[i] = clock(test2) - base;
		// Run verification on test2.
		test2->verify();
	}

	// Move to a new line for output.
	channel << IOCtrl::endl;

	// Calculate the statistical results for PAPA BEAR, tests 1 and 2.
	resultFromArray(papa1, results1, repeat);
	resultFromArray(papa2, results2, repeat);

	/* BABY BEAR (warm cache, or "just right"), alternates tests A and B
	 * in sets of eight, therefore allowing some cache warming to occur,
	 * but also allowing cache misses to be a factor. In some cases, this
	 * is closer to what would occur with typical use of an algorithm or
	 * approach.*/

	// Display BABY BEAR banner.
	channel << IOCat::normal << "\n"
			<< IOFormatTextBG::cyan << IOFormatTextFG::white
			<< "COMPARISON 3/3: BABY BEAR" << IOCtrl::endl;
	channel << IOCat::normal << "(BABY BEAR) Loading..." << IOCtrl::endc;

	/* Define three 16-bit unsigned integers for managing BABY BEAR's more
	 * complex alternation - counters for the outer and inner loops, and
	 * one for all remaining tests when our repeat is not divisible by
	 * eight (which it usually won't be.) The inner loop should always be
	 * eight!*/
	uint16_t inner = 8, outer, remain;
	// The outer loop is the repeat count, integer-divided-by eight.
	outer = repeat / 8;
	// We store the remainder, so we still meet the exact repeat count.
	remain = repeat % 8;

	// We run 8 of each test in each set (outer)...
	for (int o = 0; o < outer; o++) {
		// Run test 1 eight consecutive times for this set...
		for (int i1 = 0; i1 < inner; i1++) {
			// Display progress (overwrite line.)
			channel << IOCat::normal << "(BABY BEAR) Pass "
					<< ((o + 1) * (i1 + 1)) << "-A of " << repeat << ".  "
					<< IOCtrl::endc;
			// Run pre-repeat cleanup on test1.
			test1->janitor();
			// Record test 1 measurement, offset by baseline.
			results1[i1] = clock(test1) - base;
			// Run verification on test1.
			test1->verify();
		}

		// Run test 2 eight consecutive times for this set...
		for (int i2 = 0; i2 < inner; i2++) {
			// Display progress (overwrite line.)
			channel << IOCat::normal << "(BABY BEAR) Pass "
					<< ((o + 1) * (i2 + 1)) << "-B of " << repeat << ".  "
					<< IOCtrl::endc;
			// Run pre-repeat cleanup on test2.
			test2->janitor();
			// Record test 2 measurement, offset by baseline.
			results2[i2] = clock(test2) - base;
			// Run verification on test2.
			test2->verify();
		}
	}

	// Run test 1 the remaining number of times consecutively...
	for (int r1 = 0; r1 < remain; r1++) {
		// Display progress (overwrite line.)
		channel << IOCat::normal << "(BABY BEAR) Pass "
				<< ((outer * inner) + (r1 + 1)) << "-A of " << repeat << ".  "
				<< IOCtrl::endc;
		// Run pre-repeat cleanup on test1.
		test1->janitor();
		// Record test 1 measurement, offset by baseline.
		results1[r1] = clock(test1) - base;
		// Run verification on test1.
		test1->verify();
	}

	// Run test 2 the remaining number of times consecutively...
	for (int r2 = 0; r2 < remain; r2++) {
		// Display progress (overwrite line.)
		channel << IOCat::normal << "(BABY BEAR) Pass "
				<< ((outer * inner) + (r2 + 1)) << "-B of " << repeat << "."
				<< IOCtrl::sendc;
		// Run pre-repeat cleanup on test2.
		test2->janitor();
		// Record test 2 measurement, offset by baseline.
		results2[r2] = clock(test2) - base;
		// Run verification on test2.
		test2->verify();
	}

	// Move to a new line for output.
	channel << IOCtrl::endl;

	// Calculate the statistical results for BABY BEAR, tests 1 and 2.
	resultFromArray(baby1, results1, repeat);
	resultFromArray(baby2, results2, repeat);

	// Turn on testing messages again, as we're done running the tests.
	channel.speak_up(IOCat::testing);

	// Let the user know that we're done running the benchmarker.
	channel << IOCat::normal << "\n"
			<< IOFormatTextAttr::bold << IOFormatTextBG::green
			<< IOFormatTextFG::white << "BENCHMARKER COMPLETE" << IOCtrl::endl;

	// Cleanup test 1 and 2 using their post functions.
	test1->post();
	test2->post();

	// Display information about results.
	if (showStats) {
		channel << "RESULTS" << IOCtrl::endl;
		channel << "\tMany numbers are displayed as VALUE/ADJUSTED" << IOCtrl::n
				<< "\t(The adjusted value excludes outlier values.)"
				<< IOCtrl::n << IOCtrl::endl;

		// Display the baseline measurements, for reference.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "BASELINE MEASUREMENTS" << IOCtrl::endl;
		printResult(baseR);

		// Display the results for MAMA BEAR, test 1.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "MAMA BEAR: [" << test1->get_title() << "]" << IOCtrl::endl;
		printResult(mama1);

		// Display the results for MAMA BEAR, test 2.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "MAMA BEAR: [" << test2->get_title() << "]" << IOCtrl::endl;
		printResult(mama2);

		// Display the verdict for MAMA BEAR in BOLD.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "MAMA BEAR: VERDICT" << IOCtrl::endl;
		channel << IOFormatTextAttr::bold;
		printVerdict(mama1, mama2, test1, test2);

		// Display the results for PAPA BEAR, test 1.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "PAPA BEAR: TEST [" << test1->get_title() << "]"
				<< IOCtrl::endl;
		printResult(papa1);

		// Display the results for PAPA BEAR, test 2.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "PAPA BEAR: TEST [" << test2->get_title() << "]"
				<< IOCtrl::endl;
		printResult(papa2);

		// Display the verdict for PAPA BEAR in BOLD.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "PAPA BEAR: VERDICT" << IOCtrl::endl;
		channel << IOFormatTextAttr::bold;
		printVerdict(papa1, papa2, test1, test2);

		// Display the results for BABY BEAR, test 1.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "BABY BEAR: TEST [" << test1->get_title() << "]"
				<< IOCtrl::endl;
		printResult(baby1);

		// Display the results for BABY BEAR, test 2.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "BABY BEAR: TEST [" << test2->get_title() << "]"
				<< IOCtrl::endl;
		printResult(baby2);

		// Display the verdict for BABY BEAR in BOLD.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "BABY BEAR: VERDICT" << IOCtrl::endl;
		channel << IOFormatTextAttr::bold;
		printVerdict(baby1, baby2, test1, test2);
	} else {
		// Display the baseline measurements, for reference.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "BASELINE MEASUREMENTS" << IOCtrl::endl;
		printResult(baseR);

		// Display the verdict for MAMA BEAR in BOLD.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "MAMA BEAR: VERDICT" << IOCtrl::endl;
		channel << IOFormatTextAttr::bold;
		printVerdict(mama1, mama2, test1, test2);

		// Display the verdict for PAPA BEAR in BOLD.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "PAPA BEAR: VERDICT" << IOCtrl::endl;
		channel << IOFormatTextAttr::bold;
		printVerdict(papa1, papa2, test1, test2);

		// Display the verdict for BABY BEAR in BOLD.
		channel << IOCat::normal << "\n"
				<< IOFormatTextBG::cyan << IOFormatTextFG::white
				<< "BABY BEAR: VERDICT" << IOCtrl::endl;
		channel << IOFormatTextAttr::bold;
		printVerdict(baby1, baby2, test1, test2);
	}

	// Cleanup: Delete the results arrays.
	delete[] results1;
	delete[] results2;
	// Null out the pointers to the results arrays (by habit).
	results1 = NULL;
	results2 = NULL;

	// Calculate which test won (-1=error, 0=dead heat, 1=test A, 2=test B)
	int8_t verdict = calculateVerdict(baby1, baby2);

	// If test A won...
	if (verdict == 1) {
		// Report success
		return true;
	}
	// If it was a dead heat, and that counts as a success (from args)...
	else if (deadHeat && verdict == 0) {
		return true;
	}

	// All other verdict conditions return false.
	return false;
}

void TestManager::resultFromArray(BenchmarkResult& result,
								  uint64_t arr[],
								  int repeat)
{
	// Sort the array.
	std::sort(arr, arr + repeat);

	// Store the repetition count.
	result.repeat = repeat;

	/* Calculate the accumulator (grand total).
	 * For each item in the array...*/
	for (int i = 0; i < repeat; i++) {
		// Add the value to the accumulator.
		result.acc += arr[i];
	}

	// Calculate mean.
	result.mean = result.acc / repeat;

	// Store the minimum and maximum values.
	result.min_val = arr[0];
	result.max_val = arr[repeat - 1];

	// Calculate the range as the maximum - minimum values.
	result.range = result.max_val - result.min_val;

	/* Calculate standard deviance (s) from variance (s^2).
	 * This is calculated as s^2 = [ (arr[i] - mean)^2 / (count-1) ]
	 * and s = √[s^2]
	 */
	// We'll be using this accumulator temporarily.
	int64_t v_acc = 0;
	// A temporary integer.
	int64_t temp = 0;
	// We'll loop to create the summation. For each value...
	for (int i = 0; i < repeat; i++) {
		// Add (arr[i] - mean)^2 to the accumulator.
		temp = arr[i] - result.mean;
		v_acc += (temp * temp);
	}

	// The variance is the accumulator / the count minus one.
	double variance = v_acc / (repeat - 1);
	// The standard deviation is the square root of the variance.
	result.std_dev = sqrt(variance);

	/* The relative standard deviation is the standard deviation / mean,
	 * expressed as a percentage.*/
	result.rsd = (result.std_dev / result.mean) * 100;

	// Calculate median.
	int mI = repeat / 2;
	// If we have a single value as our exact median...
	if (repeat % 2 == 0) {
		// Store that value as the median.
		result.median = arr[mI];
	}
	// Otherwise, if we do NOT have a single value as our exact median...
	else {
		// Store the mean of the middle two values as the median.
		result.median = ((arr[mI] + arr[mI + 1]) / 2);
	}

	// Calculate lower and upper quartile values.
	int q1I = repeat / 4;
	int q3I = repeat * 3 / 4;

	// We're following the same basic approach as with median.
	if (repeat % 4 == 0) {
		result.q1 = arr[q1I];
		result.q3 = arr[q3I];
	} else {
		result.q1 = (arr[q1I] + arr[q1I + 1]) / 2;
		result.q3 = (arr[q3I] + arr[q3I + 1]) / 2;
	}

	// Calculate the interquartile value (transitory).
	uint64_t iq = result.q3 - result.q1;

	// Calculate the lower and upper inner and outer fence.
	result.lif = result.q1 - (iq * 1.5);
	result.uif = result.q3 + (iq * 1.5);
	result.lof = result.q1 - (iq * 3);
	result.uof = result.q3 + (iq * 3);

	/* Calculate the number of minor and major LOWER outliers.*/

	/* These will correspond to the index just past the last outlier
	 * in each direction, for use in calculating the adjusted values,
	 * which omit outliers.*/
	int lower_cutoff = 0;
	int upper_cutoff = repeat - 1;

	// For each item (ascending)
	for (int i = 0; i < repeat; i++) {
		// If the item is smaller than the inner fence value...
		if (arr[i] < result.lif) {
			// If the item is also smaller than the outer fence value...
			if (arr[i] < result.lof) {
				// It is a major outlier.
				result.low_out_major++;
			}
			// Otherwise...
			else {
				// It is a minor outlier.
				result.low_out_minor++;
			}
		} else {
			/* We've found the first non-outlier value.
			 * Store the index as lower cutoff.*/
			lower_cutoff = i;
			// We're done. No sense counting further in a sorted array.
			break;
		}
	}

	// For each item (descending)
	for (int i = (repeat - 1); i >= 0; i--) {
		// If the item is larger than the inner fence value...
		if (arr[i] > result.uif) {
			// If the item is also larger than the outer fence value...
			if (arr[i] > result.uof) {
				// It is a major outlier.
				result.upp_out_major++;
			}
			// Otherwise...
			else {
				// It is a minor outlier.
				result.upp_out_minor++;
			}
		} else {
			/* We've found the first non-outlier value.
			 * Store the index as upper cutoff.*/
			upper_cutoff = i;
			// We're done. No sense counting further in a sorted array.
			break;
		}
	}

	/* Calculate the adjusted accumulator.
	 * For each item in the array, within cutoffs...*/
	for (int i = lower_cutoff; i <= upper_cutoff; i++) {
		// Add the value to the accumulator.
		result.acc_adj += arr[i];
	}

	// Calculate a new count to work with, omitting outliers.
	int repeat_adj = upper_cutoff - lower_cutoff;

	// Calculate adjusted mean.
	result.mean_adj = result.acc_adj / repeat_adj;
	// FIXME: Why is this giving a different answer than mean w/o any outliers?
	/* Possible answer (just found this comment again)...
	 * because it's REMOVING OUTLIERS??
	 */

	// Store the minimum and maximum non-outlier values.
	result.min_adj_val = arr[lower_cutoff];
	result.max_adj_val = arr[upper_cutoff];

	// Calculate the adjusted range as the maximum - minimum (adjusted) values.
	result.range_adj = result.max_adj_val - result.min_adj_val;

	/* Calculate adjusted standard deviance (s) from variance (s^2).
	 * This is calculated as s^2 = [ (arr[i] - mean)^2 / (count-1) ]
	 * and s = √[s^2], except using the adjusted numbers.
	 */
	// We'll be reusing this accumulator from earlier.
	v_acc = 0;
	// We'll loop to create the summation. For each value...
	for (int i = 0; i < repeat_adj; i++) {
		temp = arr[i] - result.mean_adj;
		// Add (arr[i] - mean)^2 to the accumulator.
		v_acc += (temp * temp);
	}
	// The variance is the accumulator / the count minus one.
	double variance_adj = v_acc / (repeat_adj - 1);
	// The standard deviation is the square root of the variance.
	result.std_dev_adj = sqrt(variance_adj);

	/* The relative standard deviation is the standard deviation / mean,
	 * expressed as a percentage.*/
	result.rsd_adj = (result.std_dev_adj / result.mean_adj) * 100;
}

void TestManager::printResult(BenchmarkResult& result)
{
	/* If the adjusted relative standard deviation is greater than this
	 * value, the accuracy of the other numbers is in serious doubt, and
	 * should be taken with a grain of salt. In that situation, we will
	 * highlight the RSD output line in red.*/
	int rsd_threshold = 25;

	// Output the relevant data, keeping formatting until the end.
	channel << "\tMEAN (μ): " << result.mean << " / " << result.mean_adj
			<< IOCtrl::sendl;

	channel << "\tMIN-MAX(RANGE): " << result.min_val << "-" << result.max_val
			<< "(" << result.range << ") / " << result.min_adj_val << "-"
			<< result.max_adj_val << "(" << result.range_adj << ")"
			<< IOCtrl::sendl;

	channel << "\tOUTLIERS: " << result.low_out_major + result.low_out_minor
			<< " LOW, " << result.upp_out_major + result.upp_out_minor
			<< " HIGH" << IOCtrl::sendl;

	channel << "\tSD (σ): " << IOFormatDecimalPlaces(2) << result.std_dev
			<< " / " << result.std_dev_adj << "" << IOCtrl::sendl;

	channel << "\t"
			<< (result.rsd_adj > rsd_threshold ? IOFormatTextBG::red
											   : IOFormatTextBG::none)
			<< "RSD: " << result.rsd << "% / " << result.rsd_adj << "%"
			<< IOCtrl::endl;
}

uint8_t TestManager::calculateVerdict(BenchmarkResult& result1,
									  BenchmarkResult& result2)
{
	// Calculate difference between the adjusted mean averages.
	int64_t difference_adj = result1.mean_adj - result2.mean_adj;

	// If the absolute difference is less than either standard deviation...
	if (labs(difference_adj) <= result1.std_dev_adj ||
		labs(difference_adj) <= result2.std_dev_adj) {
		// The tests are roughly the same. Return 0.
		return 0;
	} else {
		// If the first test won (its adjusted mean was smaller)...
		if (difference_adj < 0) {
			// Declare the first test as faster.
			return 1;
		}
		// Else if the second test won (its adjusted mean was smaller)...
		else if (difference_adj > 0) {
			// Declare the second test as faster.
			return 2;
		}
	}

	// If we get here, math is broken. Panic and return -1 (error).
	return -1;
}

void TestManager::printVerdict(BenchmarkResult& result1,
							   BenchmarkResult& result2,
							   Test* test1,
							   Test* test2)
{
	// Calculate difference between the non-adjusted mean averages.
	int64_t difference = result1.mean - result2.mean;
	// Calculate difference between the adjusted mean averages.
	int64_t difference_adj = result1.mean_adj - result2.mean_adj;

	// FOR NON-ADJUSTED (RAW) DATA...

	// If the absolute difference is less than either standard deviation...
	if (labs(difference) <= result1.std_dev ||
		labs(difference) <= result2.std_dev) {
		// Non-adjusted, the tests are roughly identical.
		channel
			<< "\t     RAW: Both tests are roughly identical. (DIFF <= STD DEV)"
			<< IOCtrl::sendl;
	} else {
		// If the first test won (its non-adjusted mean was smaller)...
		if (difference < 0) {
			// Declare the first test as faster, and by how much.
			channel << "\t     RAW: [" << test1->get_title()
					<< "] faster by approx. " << (labs(difference))
					<< " cycles." << IOCtrl::sendl;
		}
		// Else if the second test won (its non-adjusted mean was smaller)...
		else if (difference > 0) {
			// Declare the second test as faster, and by how much.
			channel << "\t     RAW: [" << test2->get_title()
					<< "] faster by approx. " << (labs(difference))
					<< " cycles." << IOCtrl::sendl;
		}
	}

	// FOR ADJUSTED DATA...

	// If the absolute difference is less than either standard deviation...
	if (labs(difference_adj) <= result1.std_dev_adj ||
		labs(difference_adj) <= result2.std_dev_adj) {
		// Adjusted, the tests are roughly identical.
		channel
			<< "\tADJUSTED: Both tests are roughly identical. (DIFF <= STD DEV)"
			<< IOCtrl::endl;
	} else {
		// If the first test won (its adjusted mean was smaller)...
		if (difference_adj < 0) {
			// Declare the first test as faster, and by how much.
			channel << "\tADJUSTED: [" << test1->get_title()
					<< "] faster by approx. "
					<< (labs(difference_adj) - result1.std_dev_adj)
					<< " cycles." << IOCtrl::endl;
		}
		// Else if the second test won (its adjusted mean was smaller)...
		else if (difference_adj > 0) {
			// Declare the second test as faster, and by how much.
			channel << "\tADJUSTED: [" << test2->get_title()
					<< "] faster by approx. "
					<< (labs(difference_adj) - result2.std_dev_adj)
					<< " cycles." << IOCtrl::endl;
		}
	}
}

bool TestManager::validate(testname_t item_name,
						   bool yell,
						   GolidlocksItemType type)
{
	/* Use `std::map::count` to test for the key in the map.
	 * Using `std::map::find` in this way has some memory issues,
	 * according to Clang.
	 */
	bool r = false;
	switch (type) {
		case GolidlocksItemType::test:
			r = (tests.count(item_name) != 0);
			break;
		case GolidlocksItemType::comparative:
			r = (comparatives.count(item_name) != 0);
			break;
		case GolidlocksItemType::suite:
			r = (suites.count(item_name) != 0);
			break;
	}

	if (yell && !r) {
		switch (type) {
			case GolidlocksItemType::test:
			case GolidlocksItemType::suite:
				channel << IOCat::error << IOFormatTextAttr::bold
						<< IOFormatTextFG::red << "ERROR: The item "
						<< item_name
						<< " is not registered with Goldilocks Test Manager. "
						   "Aborting."
						<< IOCtrl::endl;
				break;
			case GolidlocksItemType::comparative:
				channel << IOCat::error << IOFormatTextAttr::bold
						<< IOFormatTextFG::red << "ERROR: The item "
						<< item_name
						<< " does not have a comparative test registered with "
						   "Golidlocks Test Manager. Aborting."
						<< IOCtrl::endl;
				break;
		}
	}

	return r;
}

// HACK: This is here because it needs attribute access for TestManager
// This is just a hack to keep 1.0 alive. Don't judge me.

void TestSuite::register_test(testname_t test_name,
							  Test* test,
							  bool will_run /*= true*/,
							  Test* compare /*= 0*/)
{
	// If we have a valid testmanager pointer...
	if (testmanager) {
		// Register the test with the testmanager.
		testmanager->register_test(test_name, test, compare);
		// Store the test name for loading on demand.
		tests_load.push_back(test_name);

		// If we're supposed to run the test...
		if (will_run) {
			tests_run.push_back(test_name);
		}
	} else {
		channel << IOCat::error << IOVrb::normal
				<< "ERROR: TestSuite is lonely and has no one to talk to. :("
				<< IOCtrl::endl;
		return;
	}
}

bool TestSuite::run_tests()
{
	// If we have a valid testmanager and tests to run...
	if (testmanager && tests_run.size() > 0) {
		// Run each test in this suite through the test manager.
		for (unsigned int i = 0; i < tests_run.size(); ++i) {
			// If a test fails...
			if (!testmanager->run_test(tests_run[i])) {
				// Don't bother running the rest, just fail.
				return false;
			}
		}
		// If we reach this point, all of the tests passed; report success.
		return true;
	}
	// Else if we don't have any tests marked to run...
	else if (tests_run.size()) {
		channel << IOCat::error << IOVrb::normal
				<< "ERROR: TestSuite has no tests marked to run. Aborting."
				<< IOCtrl::endl;
		return false;
	}
	// Else if we don't have a valid testmananger...
	else if (!testmanager) {
		channel << IOCat::error << IOVrb::normal
				<< "ERROR: TestSuite is lonely and has no one to talk to. :("
				<< IOCtrl::endl;
		return false;
	}

	// If we reach this point, something weird was wrong; report failure.
	return false;
}
