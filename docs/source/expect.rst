.. _expect:

Expect
######################################################

An **Expect** is a single expression whose evaluation is monitored
and recorded. Tests are generally composed of one or more Expectations.

The structure of an Expect is as follows:

..  code-block:: cpp

    Expect<That, Should=Should::Pass>(value, expected_value);

Some expectations have additional arguments, but the template parameters
are the same throughout.

..  _expect_macro:

Macros
=====================================================

An Expect is executed in a Test via one of three macros: ``REQUIRE``,
``UNLESS``, or ``CHECK``.

Typically, these macros would be used in the ``run()`` function of a
Test, but they can also be used anywhere else in the Test where you need
to verify an expectation before continuing.

..  _expect_macro_require:

``REQUIRE``
-----------------------------------------------------

Requires an Expect to be met, else the function will fail.

..  code-block:: cpp

    REQUIRE(Expect<That::IsEqual>(40, 40));  // okay
    REQUIRE(Expect<That::IsEqual>(2, 3));  // causes function to return false

..  _expect_macro_check:

``CHECK``
-----------------------------------------------------

Evaluates an Expect, but never causes the function to fail.

..  code-block:: cpp

    CHECK(Expect<That::IsEqual>(40, 40));  // okay
    CHECK(Expect<That::IsEqual>(2, 3));  // okay, although Expect failed

..  _expect_macro_unless:

``UNLESS``
-----------------------------------------------------

Requires an Expect to fail, else the function will fail.
Unlike ``Should::Fail``, this lets the Expect still fail as normal,
but the function succeeds.

In practice, you're usually better off using ``Should::Fail`` in your
Expect instead of this macro. This is only if you want the Expect itself to
fail, but the function to pass as a result.

..  code-block:: cpp

    UNLESS(Expect<That::IsEqual>(true, false));  // okay
    UNLESS(Expect<That::IsEqual>(2, 2);  // causes function to return false

..  _expect_that:

That
=====================================================

The behavior of the Expect is primarily determined by the ``That::``
template parameter.

Most of these expectations depend on the indicated comparison operator being
supported by all data types passed to the ``That::`. For example, if you use
``Expect<That::IsLess>(foo, bar)``, then the types of ``foo`` and ``bar`` must
be comparable via ``foo < bar``.

..  _expect_that_istrue:

IsTrue
-----------------------------------------------------

``Expect<That::IsTrue>(op)``

Expects ``op`` to implicitly evaluate to ``true``:

..  code-block:: cpp

    return (op == true);

If ``op`` is a non-null pointer, dereferences ``op`` and evaluates
value. If ``op`` is ``nullptr``, the Expect fails.

..  _expect_that_isfalse:

IsFalse
-----------------------------------------------------

``Expect<That::IsFalse>(op)``

Expects ``op`` to implicitly evaluate to ``false``:

..  code-block:: cpp

    return (op == false);

If ``op`` is a non-null pointer, dereferences ``op`` and evaluates
value. If ``op`` is ``nullptr``, the Expect fails.

..  _expect_that_isequal:

IsEqual
-----------------------------------------------------

``Expect<That::IsEqual>(left, right)``

Expects ``left`` and ``right`` to evaluate as equal:

..  code-block:: cpp

    return (left == right);

If either ``left`` or ``right`` are non-null pointers, they are
dereferenced as appropriate, and the evaluation is run against the two values.
If either is ``nullptr``, the Expect fails.

..  _expect_that_isnotequal:

IsNotEqual
-----------------------------------------------------

``Expect<That::IsNotEqual>(left, right)``

Expects ``left`` and ``right`` to evaluate as *not* equal:

..  code-block:: cpp

    return (left != right);

If either ``left`` or ``right`` are non-null pointers, they are
dereferenced as appropriate, and the evaluation is run against the two values.
If either is ``nullptr``, the Expect fails.

..  _expect_that_isless:

IsLess
-----------------------------------------------------

``Expect<That::IsLess>(left, right)``

Expects ``left`` evaluates to less than ``right``:

..  code-block:: cpp

    return (left < right);

If either ``left`` or ``right`` are non-null pointers, they are
dereferenced as appropriate, and the evaluation is run against the two values.
If either is ``nullptr``, the Expect fails.

..  _expect_that_islessequal:

IsLessEqual
-----------------------------------------------------

``Expect<That::IsLessEqual>(left, right)``

Expects ``left`` evaluates to less than or equal to ``right``:

..  code-block:: cpp

    return (left <= right);

If either ``left`` or ``right`` are non-null pointers, they are
dereferenced as appropriate, and the evaluation is run against the two values.
If either is ``nullptr``, the Expect fails.

..  _expect_that_isgreater:

IsGreater
-----------------------------------------------------

``Expect<That::IsGreater>(left, right)``

Expects ``left`` evaluates to greater than ``right``:

..  code-block:: cpp

    return (left > right);

If either ``left`` or ``right`` are non-null pointers, they are
dereferenced as appropriate, and the evaluation is run against the two values.
If either is ``nullptr``, the Expect fails.

..  _expect_that_isgreaterequal:

IsGreaterEqual
-----------------------------------------------------

``Expect<That::IsGreaterEqual>(left, right)``

Expects ``left`` evaluates to greater than or equal to ``right``:

..  code-block:: cpp

    return (left >= right);

If either ``left`` or ``right`` are non-null pointers, they are
dereferenced as appropriate, and the evaluation is run against the two values.
If either is ``nullptr``, the Expect fails.

..  _expect_that_ptrisnull:

PtrIsNull
-----------------------------------------------------

``Expect<That::PtrIsNull>(ptr)``

You must pass a pointer to this. Expects the pointer ``ptr`` to be ``nullptr``:

..  code-block:: cpp

    return (ptr == nullptr);

..  _expect_that_ptrisnotnull:

PtrIsNotNull
-----------------------------------------------------

``Expect<That::PtrIsNotNull>(ptr)``

You must pass a pointer to this. Expects the pointer ``ptr`` to *not*
be ``nullptr``:

..  code-block:: cpp

    return (ptr != nullptr);

..  _expect_that_ptrisequal:

PtrIsEqual
-----------------------------------------------------

``Expect<That::PtrIsEqual>(left, right)``

You must pass pointers to this. Expects the pointers ``left`` and ``right``
to point to the same address in memory:

..  code-block:: cpp

    return (left == right);

This does not check that the pointers are non-null or valid,
nor does it check that the pointers can be dereferenced.

..  _expect_that_ptrisnotequal:

PtrIsNotEqual
-----------------------------------------------------

``Expect<That::PtrIsNotEqual>(left, right)``

You must pass pointers to this. Expects the pointers ``left`` and ``right``
to *not* point to the same address in memory:

..  code-block:: cpp

    return (left != right);

This does not check that the pointers are non-null or valid,
nor does it check that the pointers can be dereferenced.

..  _expect_that_funcreturns:

FuncReturns
-----------------------------------------------------

``Expect<That::FuncReturns>(target, name_hint, func, args...)``

Passes the arguments ``args...`` to the function ``func``, and
expects the returned value to match ``target``.

..  code-block:: cpp

    return (func(args...) == target);

The argument ``name_hint`` is a string. It is used only for displaying the
name of the function in the test report.

..  _expect_that_functhrows:

FuncThrows
-----------------------------------------------------

``Expect<That::FuncThrows>(target, name_hint, func, args...)``

Passes the arguments ``args...`` to the function ``func``, and
expects the returned value to throw the exception ``target``.

..  code-block:: cpp

    try {
        func(args...);
    } catch (const T& e) { // T is type of target
        return true;
    } catch (...) {
        return false;
    }
    return false;

If the function is not supposed to throw anything, you can
pass the value ``Nothing()`` to ``target``.

The argument ``name_hint`` is used only for displaying the name
of the function in the test report.

..  _expect_that_isapproxequal:

IsApproxEqual
-----------------------------------------------------

``Expect<That::IsApproxEqual>(value, target, margin)``

Expects value to be approximately equal to target, within the margin.

..  code-block:: cpp

    return ((value - target) < 0 ? ((value - target) * (-1.0)) < margin : (value - target) < margin));

If value is a non-null pointer, dereferences value and evaluates. If value is nullptr, returns false.

..  code-block:: cpp

    return (value != nullptr && (*value - target) < 0 ? ((*value - target) * (-1.0)) < margin : (*value - target) < margin);

..  _expect_that_isapproxnotequal:

IsApproxNotEqual
-----------------------------------------------------

``Expect<That::IsApproxNotEqual>(value, target, margin)``

Expects value to not be approximately equal to target, within the margin.

..  code-block:: cpp

    return ((value - target) < 0 ? ((value - target) * (-1.0)) > margin : (value - target) > margin);

If value is a non-null pointer, dereferences value and evaluates. If value is nullptr, returns false.

..  code-block:: cpp

    return (value != nullptr && (*value - target) < 0 ? ((*value - target) * (-1.0)) > margin : (*value - target) > margin));

..  _expect_that_isinrange:

IsInRange
-----------------------------------------------------

``Expect<That::IsInRange>(value, lower, upper)``

Expects value to be in the inclusive range defined by lower and upper.

..  code-block:: cpp

    return (value >= lower && value <= upper);

If value is a non-null pointer, dereferences value and evaluates. If op is nullptr, returns false.

.. code-block:: cpp

    return (value != nullptr && *value >= lower && *value <= upper);

..  _expect_that_isnotinrange:

IsNotInRange
-----------------------------------------------------

``Expect<That::IsNotInRange>(value, lower, upper)``

Expects value to be outside the inclusive range defined by lower and upper.

.. code-block:: cpp

    return (value < lower || value > upper);

If value is a non-null pointer, dereferences value and evaluates. If op is nullptr, returns false.

.. code-block:: cpp

    return (value != nullptr && (*value < lower || *value > upper));

..  _expect_should:

Should
=====================================================

The ``Should`` template parameter determines how the outcome of the
evaluation is interpreted.

``Should::Pass`` means the evaluation should succeed for the expectation
to be met. This is the default.

``Should::Fail`` means the evaluation is NOT supposed to succeed for the
expectation to be met. This is useful for guarding against false positives.

..  code-block:: cpp

    Minute min(60);
    Hour hour(1);
    Hour bad_hour(2);

    // Check min == hour
    REQUIRE(Expect<That::IsEqual>(min, hour));

    // Check min != hour
    REQUIRE(Expect<That::IsNotEqual>(min, bad_hour));

    // Check min == bad_hour FAILS
    REQUIRE(Expect<That::IsEqual, Should::Fail>(min, bad_hour));

    // If the last Expect was OK, that would mean `min == bad_hour`, throwing
    // the reliability of `min == hour` into doubt.

``Should::Pass_Silent`` and ``Should::Fail_Silent`` are the same as
their non-silent counterparts, but if the expectation is met, no output is
produced. Instead, output is only produced if the expectation is NOT met.
