Goldilocks 2.0 Documentation
######################################################

Goldilocks is a complete testing and runtime-benchmark framework,
based on MousePaw Media's :abbr:`LIT (Live-In Testing)` Standard. Although
:abbr:`LIT (Live-In Testing)` is inherently different from "unit testing" and
:abbr:`TDD (test-driven development)`, Goldilocks may be used for either
approach. It may also be used in conjunction with other testing systems.

The core idea of Goldilocks is that tests ship in the final code,
and can be loaded and executed within normal program execution via
a custom interface. A major advantage of this system is that tests and
benchmarks may be performed on many systems without the need for harnesses,
debug flags, or additional software.

The fastest way to run tests in Goldilocks is with the :ref:`shell`.

..  toctree::
    :maxdepth: 2
    :caption: Contents:

    goldilocks
    shell

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
