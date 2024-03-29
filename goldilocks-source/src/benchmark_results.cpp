#include "goldilocks/benchmark_results.hpp"

#include <algorithm>  // std::sort
#include <cmath>      // sqrt

void BenchmarkResult::finalize(const BenchmarkResult& result1,
							   const BenchmarkResult& result2)
{
	// Sort the array.
	std::sort(results.begin(), results.end());

	// Store the repetition count.
	this->repeat = results.size();

	/* Calculate the accumulator (grand total).
	 * For each item in the array...*/
	for (uint64_t i = 0; i < this->repeat; i++) {
		// Add the value to the accumulator.
		this->acc += results.at(i);
	}

	// Calculate mean.
	this->mean = this->acc / repeat;

	// Store the minimum and maximum values.
	this->min_val = this->results.at(0);
	this->max_val = this->results.at(results.size() - 1);

	// Calculate the range as the maximum - minimum values.
	this->range = this->max_val - this->min_val;

	/* Calculate standard deviance (s) from variance (s^2).
	 * This is calculated as s^2 = [ (arr[i] - mean)^2 / (count-1) ]
	 * and s = √[s^2]
	 */
	// We'll be using this accumulator temporarily.
	int64_t v_acc = 0;
	// A temporary integer.
	// cppcheck-suppress unreadVariable
	int64_t temp = 0;
	// We'll loop to create the summation. For each value...
	for (uint64_t i = 0; i < repeat; i++) {
		// Add (arr[i] - mean)^2 to the accumulator.
		temp = results[i] - this->mean;
		v_acc += (temp * temp);
	}

	// The variance is the accumulator / the count minus one.
	double variance = v_acc / (repeat - 1);
	// The standard deviation is the square root of the variance.
	this->std_dev = sqrt(variance);

	/* The relative standard deviation is the standard deviation / mean,
	 * expressed as a percentage.*/
	this->rsd = (this->std_dev / this->mean) * 100;

	// Calculate median.
	int mI = repeat / 2;
	// If we have a single value as our exact median...
	if (repeat % 2 == 0) {
		// Store that value as the median.
		this->median = results[mI];
	}
	// Otherwise, if we do NOT have a single value as our exact median...
	else {
		// Store the mean of the middle two values as the median.
		this->median = ((results[mI] + results[mI + 1]) / 2);
	}

	// Calculate lower and upper quartile values.
	int q1I = repeat / 4;
	int q3I = repeat * 3 / 4;

	// We're following the same basic approach as with median.
	if (repeat % 4 == 0) {
		this->q1 = results[q1I];
		this->q3 = results[q3I];
	} else {
		this->q1 = (results[q1I] + results[q1I + 1]) / 2;
		this->q3 = (results[q3I] + results[q3I + 1]) / 2;
	}

	// Calculate the interquartile value (transitory).
	uint64_t iq = this->q3 - this->q1;

	// Calculate the lower and upper inner and outer fence.
	this->lif = this->q1 - (iq * 1.5);
	this->uif = this->q3 + (iq * 1.5);
	this->lof = this->q1 - (iq * 3);
	this->uof = this->q3 + (iq * 3);

	/* Calculate the number of minor and major LOWER outliers.*/

	/* These will correspond to the index just past the last outlier
	 * in each direction, for use in calculating the adjusted values,
	 * which omit outliers.*/
	int lower_cutoff = 0;
	int upper_cutoff = repeat - 1;

	// For each item (ascending)
	for (uint64_t i = 0; i < repeat; i++) {
		// If the item is smaller than the inner fence value...
		if (results[i] < this->lif) {
			// If the item is also smaller than the outer fence value...
			if (results[i] < this->lof) {
				// It is a major outlier.
				this->low_out_major++;
			}
			// Otherwise...
			else {
				// It is a minor outlier.
				this->low_out_minor++;
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
	for (uint64_t i = (repeat - 1); i != 0; i--) {
		// If the item is larger than the inner fence value...
		if (results[i] > this->uif) {
			// If the item is also larger than the outer fence value...
			if (results[i] > this->uof) {
				// It is a major outlier.
				this->upp_out_major++;
			}
			// Otherwise...
			else {
				// It is a minor outlier.
				this->upp_out_minor++;
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
		this->acc_adj += results[i];
	}

	// Calculate a new count to work with, omitting outliers.
	int repeat_adj = upper_cutoff - lower_cutoff;

	// Calculate adjusted mean.
	this->mean_adj = this->acc_adj / repeat_adj;

	// Store the minimum and maximum non-outlier values.
	this->min_adj_val = results[lower_cutoff];
	this->max_adj_val = results[upper_cutoff];

	// Calculate the adjusted range as the maximum - minimum (adjusted) values.
	this->range_adj = this->max_adj_val - this->min_adj_val;

	/* Calculate adjusted standard deviance (s) from variance (s^2).
	 * This is calculated as s^2 = [ (arr[i] - mean)^2 / (count-1) ]
	 * and s = √[s^2], except using the adjusted numbers.
	 */
	// We'll be reusing this accumulator from earlier.
	v_acc = 0;
	// We'll loop to create the summation. For each value...
	for (int i = 0; i < repeat_adj; i++) {
		temp = results[i] - this->mean_adj;
		// Add (arr[i] - mean)^2 to the accumulator.
		v_acc += (temp * temp);
	}
	// The variance is the accumulator / the count minus one.
	double variance_adj = v_acc / (repeat_adj - 1);
	// The standard deviation is the square root of the variance.
	this->std_dev_adj = sqrt(variance_adj);

	/* The relative standard deviation is the standard deviation / mean,
	 * expressed as a percentage.*/
	this->rsd_adj = (this->std_dev_adj / this->mean_adj) * 100;

	// Calculate difference between the adjusted mean averages.
	this->verdict = BenchmarkVerdict::none;
	int64_t difference_adj = result1.mean_adj - result2.mean_adj;

	if (labs(difference_adj) <= result1.std_dev_adj ||
		labs(difference_adj) <= result2.std_dev_adj) {
		// The tests are roughly the same.
		this->verdict = BenchmarkVerdict::draw;
	} else {
		// If first test won
		if (difference_adj < 0) {
			this->verdict = BenchmarkVerdict::win;
		} else if (difference_adj > 0) {
			this->verdict = BenchmarkVerdict::loss;
		} else if (difference_adj > std_dev_adj) {  // TODO RSD too high
			this->verdict = BenchmarkVerdict::questionable;
		}
	}
}