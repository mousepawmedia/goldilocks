.. _manager:

Test Manager
######################################################

TODO: Fill me in.

Test Manager API
=========================================================

Goldilocks provides a number of convenience functions to aid in creating an
interactive command-line interface for the system.

In most cases, you can probably just use the GoldilocksShell (see
`goldilocksshell`).

Functions
-----------------------------------------------------------

``list_suites()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can display the names and titles of all the tests currently registered
in the test manager using...

..  code-block:: c++

    // List all registered suites with their names and titles.
    testmanager.list_suites();

    // List all registered suites with their name only (no title).
    testmanager.list_suites(false);

``list_tests()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can display the names and titles of all the tests currently registered
(loaded) in the test manager using...

..  code-block:: c++

    // List all registered tests with their names and titles.
    testmanager.list_tests();

    // List all registered tests with their name only (no title).
    testmanager.list_tests(false);

If a test is loaded via a suite, it will not appear in this list until its
suite has actually been loaded during that session.

``i_load_suite()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Identical usage to ``load_suite()``, except it prompts the user for
confirmation before loading a suite.

``i_run_benchmark()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Identical usage to ``run_benchmark()``, except it prompts the user for
confirmation before running the benchmark.

``i_run_compare()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Identical usage to ``run_compare()``, except it prompts the user for
confirmation before running the compare.

``i_run_suite()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Identical usage to ``run_suite()``, except it prompts the user for
confirmation before running the suite.

``i_run_test()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Identical usage to ``run_test()``, except it prompts the user for
confirmation before running the test.
