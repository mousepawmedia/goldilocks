.. _suites:

Suites
######################################################

A **Suite** is a collection of tests. In a typical use of Goldilocks, all tests
are organized into Suites.

In addition to allowing on-demand loading groups of tests, a Suite can be "batch
run", where all of its tests are run in succession. When one test fails, the
batch run halts and returns false.


.. _suites_structure:

..  index::
    single: suite; structure

Structure
=====================================================

Every Goldilocks suite is derived from the ``Suite`` abstract class. This
only has two functions to overload, but both are required.

..  index::
    single: suite; get_title()

.. _suites_structure_gettitle:

``get_title()``
-----------------------------------------------------

Returns a string (of type ``Suitedoc_t``) with the title of the
suite. This is the a required function for any test.

..  NOTE:: The title is separate from the ID (name) of the test used to
    register the test with the TestManager. You use the ID (name) to refer
    to the test; the title is displayed on the screen before running
    the test.

..  _suites_structure_loadtests:

``load_tests()``
-----------------------------------------------------

This function specifies which tests belong to the suite.

``Suite`` provides a function ``register_test()`` which properly registers
each test with both the suite and the TestManager itself. For convenience, it
follows the same format as ``TestManager::register_test()``, with the exception
of an optional boolean argument for specifying a test which belongs to the
suite, but should not be part of the Suite's batch run.

One reason to exclude a test from the batch run for the Suite is if the test
is a stress test that takes a long time to run.

We can also register the comparative tests as an optional fourth argument.

Below is an example of a Suite's ``load_tests``.

..  code-block:: c++

    void Suite_MagicThing::load_tests()
    {
        /* Register this test with both the suite and the test manager.
         * Also register the comparative form. */
        register_test("t101", new MagicThing_Poof(), true, new OtherThing_Poof());

        register_test("t102", new MagicThing_Vanish());

        register_test("t103", new MagicThing_Levitate());

        register_test("t104", new MagicThing_Telepathy());

        /* This test will be loaded by the suite, but will be excluded
         * from the batch run. */
        register_test("t105", new MagicThing_SawInHalf(), true);
    }

We have registered five tests with this suite, not counting the comparative form
of the one. Upon loading the suite, all five tests will be loaded into the test
manager. However, if we were to batch run this suite, only four of those tests
(t101, t102, t103, and t104) would be run.

.. _suites_registering:

Registering a Suite
=====================================================

Registering a suite with Goldilocks is as easy as registering a test. Simply
use its ``register_suite()`` function. Once a suite class has been defined,
as above, it is registered with...

..  code-block:: c++

    //Assuming testmanager is our instance of the Goldilocks test manager.
    testmanager.register_suite("SuiteFoo", new SuiteFoo());

As with tests, Goldilocks owns the instance of ``SuiteFoo``, and
automatically handles its deletion at the proper time.

..  WARNING:: Goldilocks requires exclusive ownership of each suite
    object registered to it, the same as it does tests.

.. _suites_loading:

Loading a Suite
=====================================================

One of the major advantages of using a suite is that you can load its tests
on demand. This is especially useful if you have hundreds or thousands of tests.

..  code-block:: c++

    //Load a particular suite.
    testmanager.load_suite("SuiteFoo");

Of course, sometimes you don't want to have to load each suite manually.
As a shortcut, you can just load all suites currently registered with the
test manager by calling...

..  code-block:: c++

    //Load a particular suite.
    testmanager.load_suite();

.. _suites_running:

Running a Suite
=====================================================

You can start a batch run of all the suite's tests using...

..  code-block:: c++

    //Batch run all tests in a suite.
    testmanager.run_suite("SuiteFoo");
