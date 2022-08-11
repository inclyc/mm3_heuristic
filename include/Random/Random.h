#pragma once
#include <set>

/**
 * @brief helper function for generates integer in [l, r)
 *
 * @param l lower bound (inclusive)
 * @param r upper bound (exclusive)
 * @return the result
 */
int rand_range(int l, int r);

/**
 * @brief random number between [0, 1)
 *
 * @return float
 */
float rand_float();

/**
 * @brief n elements random set with element belongs to [l, r)
 *
 * @param n number of elements
 * @param l lower bound (inclusive)
 * @param r upper bound (exclusive)
 * @note r >= l && r - l >= n
 * @return std::set<int>
 */
std::set<int> rand_set(int n, int l, int r);
