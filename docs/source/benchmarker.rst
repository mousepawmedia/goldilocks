..  _goldilocks_benchmarker:

Benchmarker Output
######################################################

The Goldilocks benchmarker outputs a *lot* of information.
This section describes how to read it.

Pass Types
=====================================================

To account for the effects of cache warming, Goldilocks makes three passes,
each with a specific behavior:

* **Mama Bear** attempts to simulate a "cold cache," increasing the
  likelihood of cache misses. This is done by running tests A and B
  alternatingly.

* **Papa Bear** attempts to simulate a "hot cache," decreasing the
  likelihood of cache misses. This is done by running all repetitions of
  test A before running all repetitions of test B.

* **Baby Bear** attempts to simulate average (or "just right") cache
  warming effects, such as what might be seen in typical program
  executions. This is done by running eight repetitions of each test
  alternatingly - 8 As, 8 Bs, 8 As, etc.

After running all three passes, the benchmarker results are displayed.

Result Groups
=====================================================

At the top of the results, we see the ``BASELINE MEASUREMENTS``. These
are based on measuring the actual measurement function of our
benchmarker.

These results are important, as this is an indicator of fluctuations in results
from external factors. If either of the :abbr:`RSD (Relative Standard Deviation)`
numbers are high (>10%), the results of the benchmarker may be thrown off.

Next, we see the individual results for each test beneath each pass type.
The verdict is displayed below both sets of results, indicating which test
was faster, and by how much. The verdict is ultimately the difference between
means, but if that difference is less than the standard deviation, it
will indicate that the tests are "roughly equal."

Statistical Data
=====================================================

Let's break down the statistical data in our results.

Most lines show two sets of values, separated with a ``/`` character. The *left*
side is the **RAW** value, accounting for each measurement taken. The *right*
side is the **ADJUSTED** value, which is the value after outlier measurements
have been removed from the data.

The **MEAN (μ)** is the average number of CPU cycles for a single run of the
test.

The **MIN-MAX(RANGE)** shows the lowest and highest measurement in the set,
as well as the difference between the two (the range).

**OUTLIERS** shows how many values have been removed from the ADJUSTED set.
Outliers are determined mathematically, and removing them allows us to account
for external factors, such as other processes using the CPU during the
benchmark.

**SD (σ)** shows our standard deviation, which indicates how much fluctuation
occurs between results. By itself, the standard deviation is not usually
meaningful.

The **RSD**, or Relative Standard Deviation, is the percentage form of the
standard deviation. This is perhaps the most important statistic! The lower
the RSD, the more precise the benchmark results are. If the RSD is too high,
it will actually be flagged as red.

The statistical data above can provide a useful indicator of the reliability
of the benchmark results.

A high RSD may indicate that the results are "contaminated" by external factors.
It is often helpful to run the comparative benchmark multiple times, and taking
the pass with the lowest RSD.

However, higher RSDs may be the result of the tests themselves, as we'll see
in the following example.

Other warning signs that the results may be contaminated or inaccurate include:

* The presence of outliers in BASELINE.

* RSDs > 10% in BASELINE.

* Red-flagged RSDs (> 25%) (unless the test has a technical reason to fluctuate
  in CPU cycle measurements between tests).

* Significantly different verdicts between passes.

The precision and accuracy of the results may be further validated by
running the comparative benchmark multiple times, especially across computers,
and directly comparing the RSDs and verdict outcomes. While actual CPU cycle
measurements may vary greatly between similar systems, the relative outcomes
should remain fairly consistent on most systems with the same processor
architecture.

Statistical Data Example
-----------------------------------------------------

Let's look at the comparison between the "shift" (insert at beginning)
functionality of FlexArray and ``std::vector``. You can run this yourself
using the PawLIB tester, with the command ``benchmark P-tB1002``.

We always start by screening the baseline::

    BASELINE MEASUREMENTS
        MEAN (μ): 64 / 65
    	MIN-MAX(RANGE): 58-75(17) / 58-75(17)
    	OUTLIERS: 0 LOW, 0 HIGH
    	SD (σ): 5.47 / 5.38
    	RSD: 8% / 8%

We have no outliers and very low RSDs, so our results probably aren't
contaminated. Of course, benchmarking is unpredictable, and external factors
may change during the benchmarking itself. However, we have no reason here to
throw out the results.

Had we seen an RSD greater than 10% for either result, it would have
been wise to discard these results and rerun the benchmark altogether.

Now let's look at the first pass, MAMA BEAR, which is designed to demonstrate
the effects of cache misses::

    MAMA BEAR: [FlexArray: Shift 1000 Integers to Front (FlexArray)]
    	MEAN (μ): 414650 / 401451
    	MIN-MAX(RANGE): 262280-739036(476756) / 262280-323876(61596)
    	OUTLIERS: 0 LOW, 5 HIGH
    	SD (σ): 106700.22 / 76270.09
    	RSD: 25% / 18%

    MAMA BEAR: [FlexArray: Shift 1000 Integers to Front (std::vector)]
    	MEAN (μ): 904723 / 876586
    	MIN-MAX(RANGE): 664354-1537966(873612) / 664354-714892(50538)
    	OUTLIERS: 0 LOW, 5 HIGH
    	SD (σ): 232960.59 / 169329.87
    	RSD: 25% / 19%

Unsurprisingly, both results show some high outliers. The RSDs are roughly
equal, however, so this is probably the result of those cache misses
or other related factors.

..  WARNING:: How the two tests are structured matters! We are very careful
    to ensure both tests have the same structure and implementation, so the
    only difference between the two is the functions or algorithms we are
    directly comparing.

Looking at the result::

    MAMA BEAR: VERDICT
    	     RAW: [FlexArray: Shift 1000 Integers to Front (FlexArray)] faster by approx. 490073 cycles.
    	ADJUSTED: [FlexArray: Shift 1000 Integers to Front (FlexArray)] faster by approx. 398864.90807662549195 cycles.

FlexArray wins that round.

Now let's look at PAPA BEAR, which attempts to demonstrate cache warming::

    PAPA BEAR: TEST [FlexArray: Shift 1000 Integers to Front (FlexArray)]
    	MEAN (μ): 321917 / 325168
    	MIN-MAX(RANGE): 305608-310824(5216) / 305608-310824(5216)
    	OUTLIERS: 0 LOW, 0 HIGH
    	SD (σ): 28252.27 / 28548.56
    	RSD: 8% / 8%

    PAPA BEAR: TEST [FlexArray: Shift 1000 Integers to Front (std::vector)]
    	MEAN (μ): 654278 / 659817
    	MIN-MAX(RANGE): 608020-765749(157729) / 608020-685548(77528)
    	OUTLIERS: 0 LOW, 1 HIGH
    	SD (σ): 53785.7 / 53494.46
    	RSD: 8% / 8%

Unlike MAMA BEAR, these results have much lower RSDs - in fact, they are
equal to the BENCHMARK RSDs (the ideal scenario) - and only one outlier
between the two. This further lends itself to our theory that the higher
RSDs in MAMA BEAR are the result of cache misses.

FlexArray wins this as well, albeit by a somewhat narrower margin::

    PAPA BEAR: VERDICT
    	     RAW: [FlexArray: Shift 1000 Integers to Front (FlexArray)] faster by approx. 332361 cycles.
    	ADJUSTED: [FlexArray: Shift 1000 Integers to Front (FlexArray)] faster by approx. 306100.43052620673552 cycles.

Finally, we look at BABY BEAR, which is intended to be the most similar to
typical use scenarios::

    BABY BEAR: TEST [FlexArray: Shift 1000 Integers to Front (FlexArray)]
    	MEAN (μ): 317852 / 321814
    	MIN-MAX(RANGE): 247433-323226(75793) / 306612-323226(16614)
    	OUTLIERS: 1 LOW, 0 HIGH
    	SD (σ): 33872.37 / 33610.86
    	RSD: 10% / 10%

    BABY BEAR: TEST [FlexArray: Shift 1000 Integers to Front (std::vector)]
    	MEAN (μ): 648568 / 652663
    	MIN-MAX(RANGE): 537774-780641(242867) / 537774-755231(217457)
    	OUTLIERS: 0 LOW, 2 HIGH
    	SD (σ): 60925.17 / 58541.29
    	RSD: 9% / 8%

Our RSDs are slightly higher than with PAPA BEAR, but we still see relatively
few outliers (a total of 3).

The BABY BEAR verdict indicates that FlexArray is the fastest, even in this
scenario::

    BABY BEAR: VERDICT
    	     RAW: [FlexArray: Shift 1000 Integers to Front (FlexArray)] faster by approx. 330716 cycles.
    	ADJUSTED: [FlexArray: Shift 1000 Integers to Front (FlexArray)] faster by approx. 297238.13385525450576 cycles.
