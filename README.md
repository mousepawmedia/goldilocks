Goldilocks
========================================

<img src=docs/source/goldilocks.svg width=300vw/>

**Testing reimagined to be Just Right.**

Goldilocks operates from the novel idea of shipping tests *within* a final
binary, enabling testing and benchmarking on virtually any machine your
project is deployed on.

The built-in benchmarker allows you to run comparative performance tests
without the need to install specialist tools on the system.* It accounts for
cache warming, as well as outlier results endemic to user-mode CPU cycle
measurements.

The included GoldilocksShell allows you to rapidly create an interactive
terminal session for running tests and benchmarks. Or, if you prefer, you
can leverage the power of [IOSqueak](https://mousepawmedia.com/iosqueak)
to build a custom interface your way.

See the [official project page](https://mousepawmedia.com/goldilocks).

> Goldilocks 1.0 was originally part of PawLIB.

<sup><sub>
    *Benchmarker currently only supports AMD64 architecture UNIX systems.
    More supported architectures and systems coming soon.
</sub></sup>

Authors
----------------------------------------

 - Jason C. McDonald

Dependencies
----------------------------------------

* IOSqueak 2.0

Building
----------------------------------------

If you have the full source repository, see BUILDING.md for information
on how to build Goldilocks, Goldilocks Tester, and the documentation.

The Ready-To-Use version of Goldilocks only contains the folders `include`
and `lib`. You can find the full source repository on
[GitHub](https://github.com/mousepawmedia/goldilocks).

Contributions
----------------------------------------

We do NOT accept pull requests through GitHub.
If you would like to contribute code, please read our
[Contribution Guide](https://mousepawmedia.com/developers/contribution).

All contributions are licensed to us under the
[MousePaw Media Terms of Development](https://mousepawmedia.com/termsofdevelopment).

License
----------------------------------------

IOSqueak is licensed under the BSD-3 License. (See LICENSE.md)

The project is owned and maintained by [MousePaw Media](https://mousepawmedia.com/developers).

<img src=docs/source/mousepaw_logo_circle.svg width=200vw/>
