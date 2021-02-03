#include "goldilocks/suite.hpp"

void TestSuite::register_test(testname_t test_name, Test* test, bool will_run, Test* compare)
{
    // If we have a valid testmanager pointer...
    if(testmanager)
    {
        // Register the test with the testmanager.
        testmanager->register_test(test_name, test, compare);
        // Store the test name for loading on demand.
        tests_load.push_back(test_name);

        // If we're supposed to run the test...
        if(will_run)
        {
            tests_run.push_back(test_name);
        }
    }
    else
    {
        channel << IOCat::error << IOVrb::normal
            << "ERROR: TestSuite is lonely and has no one to talk to. :("
            << IOCtrl::endl;
        return;
    }

}

bool TestSuite::run_tests()
{
    // If we have a valid testmanager and tests to run...
    if(testmanager && tests_run.size() > 0)
    {
        // Run each test in this suite through the test manager.
        for(unsigned int i = 0; i < tests_run.size(); ++i)
        {
            // If a test fails...
            if(!testmanager->run_test(tests_run[i]))
            {
                // Don't bother running the rest, just fail.
                return false;
            }
        }
        // If we reach this point, all of the tests passed; report success.
        return true;
    }
    // Else if we don't have any tests marked to run...
    else if(tests_run.size())
    {
        channel << IOCat::error << IOVrb::normal
            << "ERROR: TestSuite has no tests marked to run. Aborting."
            << IOCtrl::endl;
        return false;
    }
    // Else if we don't have a valid testmananger...
    else if(!testmanager)
    {
        channel << IOCat::error << IOVrb::normal
            << "ERROR: TestSuite is lonely and has no one to talk to. :("
            << IOCtrl::endl;
        return false;
    }

    // If we reach this point, something weird was wrong; report failure.
    return false;
}

void TestSuite::backregister(TestManager* tm)
{
    testmanager = tm;
}
