/** Benchmark Results [Goldilocks]
 * Version: 2.0
 *
 * Author(s): Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2019 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */

#ifndef GOLDILOCKS_BENCHMARKRESULTS_HPP
#define GOLDILOCKS_BENCHMARKRESULTS_HPP

#include "pawlib/pawsort.hpp"

/**The BenchmarkResult struct stores all of the statistical data
 * from a single test benchmark. Having this struct makes our
 * code more efficient, and allows us to write standard
 * functions for calculating results and comparisons, and
 * outputting everything.*/
class BenchmarkResult
{
public:
    /**Convert a raw array of clock measurements into a complete
     * benchmark result This does all of our statistical computations.
     * \param the array of CPU cycle measurements, stored as
     * 64-bit integers
     * \param the size of the array (number of measurements)*/
    BenchmarkResult(uint64_t arr[], int repeat)
    {
        // Sort the array.
        pawsort::introsort(arr, repeat);

        // Store the repetition count.
        this->repeat = repeat;

        /* Calculate the accumulator (grand total).
            * For each item in the array...*/
        for(int i=0; i<repeat; i++)
        {
            // Add the value to the accumulator.
            this->acc += arr[i];
        }

        // Calculate mean.
        this->mean = this->acc / repeat;

        // Store the minimum and maximum values.
        this->min_val = arr[0];
        this->max_val = arr[repeat-1];

        // Calculate the range as the maximum - minimum values.
        this->range = this->max_val - this->min_val;

        /* Calculate standard deviance (s) from variance (s^2).
            * This is calculated as s^2 = [ (arr[i] - mean)^2 / (count-1) ]
            * and s = √[s^2]
        */
        // We'll be using this accumulator temporarily.
        int64_t v_acc = 0;
        // A temporary integer.
        int64_t temp = 0;
        // We'll loop to create the summation. For each value...
        for(int i=0; i<repeat; i++)
        {
            // Add (arr[i] - mean)^2 to the accumulator.
            temp = arr[i] - this->mean;
            v_acc += (temp * temp);
        }

        // The variance is the accumulator / the count minus one.
        double variance = v_acc/(repeat-1);
        // The standard deviation is the square root of the variance.
        this->std_dev = sqrt(variance);

        /* The relative standard deviation is the standard deviation / mean,
            * expressed as a percentage.*/
        this->rsd = (this->std_dev / this->mean) * 100;

        // Calculate median.
        int mI = repeat/2;
        // If we have a single value as our exact median...
        if(repeat%2 == 0)
        {
            // Store that value as the median.
            this->median = arr[mI];
        }
        // Otherwise, if we do NOT have a single value as our exact median...
        else
        {
            // Store the mean of the middle two values as the median.
            this->median = ((arr[mI] + arr[mI+1])/2);
        }

        // Calculate lower and upper quartile values.
        int q1I = repeat/4;
        int q3I = repeat*3/4;

        // We're following the same basic approach as with median.
        if(repeat%4 == 0)
        {
            this->q1 = arr[q1I];
            this->q3 = arr[q3I];
        }
        else
        {
            this->q1 = (arr[q1I] + arr[q1I+1])/2;
            this->q3 = (arr[q3I] + arr[q3I+1])/2;
        }

        // Calculate the interquartile value (transitory).
        uint64_t iq = this->q3 - this->q1;

        // Calculate the lower and upper inner and outer fence.
        this->lif = this->q1-(iq*1.5);
        this->uif = this->q3+(iq*1.5);
        this->lof = this->q1-(iq*3);
        this->uof = this->q3+(iq*3);

        /* Calculate the number of minor and major LOWER outliers.*/

        /* These will correspond to the index just past the last outlier
            * in each direction, for use in calculating the adjusted values,
            * which omit outliers.*/
        int lower_cutoff = 0;
        int upper_cutoff = repeat-1;

        // For each item (ascending)
        for(int i=0; i<repeat; i++)
        {
            //If the item is smaller than the inner fence value...
            if(arr[i] < this->lif)
            {
                //If the item is also smaller than the outer fence value...
                if(arr[i] < this->lof)
                {
                    //It is a major outlier.
                    this->low_out_major++;
                }
                //Otherwise...
                else
                {
                    //It is a minor outlier.
                    this->low_out_minor++;
                }
            }
            else
            {
                /* We've found the first non-outlier value.
                    * Store the index as lower cutoff.*/
                lower_cutoff = i;
                //We're done. No sense counting further in a sorted array.
                break;
            }
        }

            // For each item (descending)
        for(int i=(repeat-1); i>=0; i--)
        {
            //If the item is larger than the inner fence value...
            if(arr[i] > this->uif)
            {
                //If the item is also larger than the outer fence value...
                if(arr[i] > this->uof)
                {
                    //It is a major outlier.
                    this->upp_out_major++;
                }
                //Otherwise...
                else
                {
                    //It is a minor outlier.
                    this->upp_out_minor++;
                }
            }
            else
            {
                /* We've found the first non-outlier value.
                    * Store the index as upper cutoff.*/
                upper_cutoff = i;
                //We're done. No sense counting further in a sorted array.
                break;
            }
        }

        /* Calculate the adjusted accumulator.
            * For each item in the array, within cutoffs...*/
        for(int i=lower_cutoff; i<=upper_cutoff; i++)
        {
            // Add the value to the accumulator.
            this->acc_adj += arr[i];
        }

        // Calculate a new count to work with, omitting outliers.
        int repeat_adj = upper_cutoff - lower_cutoff;

        // Calculate adjusted mean.
        this->mean_adj = this->acc_adj / repeat_adj;
        // Why is this giving a different answer than mean w/o any outliers?
        /* Possible answer (just found this comment again)...
         * because it's REMOVING OUTLIERS?? */

        // Store the minimum and maximum non-outlier values.
        this->min_adj_val = arr[lower_cutoff];
        this->max_adj_val = arr[upper_cutoff];

        // Calculate the adjusted range as the maximum - minimum (adjusted) values.
        this->range_adj = this->max_adj_val - this->min_adj_val;

        /* Calculate adjusted standard deviance (s) from variance (s^2).
            * This is calculated as s^2 = [ (arr[i] - mean)^2 / (count-1) ]
            * and s = √[s^2], except using the adjusted numbers.
        */
        // We'll be reusing this accumulator from earlier.
        v_acc = 0;
        // We'll loop to create the summation. For each value...
        for(int i=0; i<repeat_adj; i++)
        {
            temp = arr[i] - this->mean_adj;
            // Add (arr[i] - mean)^2 to the accumulator.
            v_acc += (temp*temp);
        }
        // The variance is the accumulator / the count minus one.
        double variance_adj = v_acc/(repeat_adj-1);
        // The standard deviation is the square root of the variance.
        this->std_dev_adj = sqrt(variance_adj);

        /* The relative standard deviation is the standard deviation / mean,
            * expressed as a percentage.*/
        this->rsd_adj = (this->std_dev_adj / this->mean_adj) * 100;
    }
protected:
    /// The accumulated count.
    // cppcheck-suppress unusedStructMember
    uint64_t acc = 0;

    /// The adjusted accumulated count.
    // cppcheck-suppress unusedStructMember
    uint64_t acc_adj = 0;

    /// The number of repetitions (size).
    // cppcheck-suppress unusedStructMember
    uint64_t repeat = 0;

    /// The average (mean).
    // cppcheck-suppress unusedStructMember
    uint64_t mean = 0;

    /// The adjusted average (mean).
    // cppcheck-suppress unusedStructMember
    uint64_t mean_adj = 0;

    /// The median value.
    // cppcheck-suppress unusedStructMember
    uint64_t median = 0;

    /// The lower quartile value.
    // cppcheck-suppress unusedStructMember
    uint64_t q1 = 0;

    /// The upper quartile value.
    // cppcheck-suppress unusedStructMember
    uint64_t q3 = 0;

    /// The range.
    // cppcheck-suppress unusedStructMember
    uint64_t range = 0;

    /// The adjusted range.
    // cppcheck-suppress unusedStructMember
    uint64_t range_adj = 0;

    /// The minimum value.
    // cppcheck-suppress unusedStructMember
    uint64_t min_val = 0;

    /// The minimum non-outlier value.
    // cppcheck-suppress unusedStructMember
    uint64_t min_adj_val = 0;

    /// The maximum value.
    // cppcheck-suppress unusedStructMember
    uint64_t max_val = 0;

    /// The maximum non-outlier value.
    // cppcheck-suppress unusedStructMember
    uint64_t max_adj_val = 0;

    /// Lower inner fence value.
    // cppcheck-suppress unusedStructMember
    uint64_t lif = 0;

    /// Lower outer fence value
    // cppcheck-suppress unusedStructMember
    uint64_t lof = 0;

    /// Upper inner fence value.
    // cppcheck-suppress unusedStructMember
    uint64_t uif = 0;

    /// Upper outer fence value.
    // cppcheck-suppress unusedStructMember
    uint64_t uof = 0;

    /// The number of low minor outliers
    // cppcheck-suppress unusedStructMember
    uint64_t low_out_minor = 0;

    /// The number of low major outliers
    // cppcheck-suppress unusedStructMember
    uint64_t low_out_major = 0;

    /// The number of upper minor outliers
    // cppcheck-suppress unusedStructMember
    uint64_t upp_out_minor = 0;

    /// The number of upper major outliers
    // cppcheck-suppress unusedStructMember
    uint64_t upp_out_major = 0;

    /// The standard deviance
    // cppcheck-suppress unusedStructMember
    double std_dev = 0;

    /// The adjusted standard deviance
    // cppcheck-suppress unusedStructMember
    double std_dev_adj = 0;

    /// The relative standard deviation (coefficient of variation)
    // cppcheck-suppress unusedStructMember
    uint8_t rsd = 0;

    /// The adjusted relative standard deviation
    // cppcheck-suppress unusedStructMember
    uint8_t rsd_adj = 0;
};

#endif // GOLDILOCKS_BENCHMARKRESULTS_HPP
