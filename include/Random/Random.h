#pragma once
#include <set>

/**
 * @brief helper function for generates integer in [L, R)
 *
 * @param L lower bound (inclusive)
 * @param R upper bound (exclusive)
 * @return the result
 */
int randRange(int L, int R);

/**
 * @brief random number between [0, 1)
 *
 * @return float
 */
float randFloat();

/**
 * @brief N elements random set with element belongs to [L, R)
 *
 * @param N number of elements
 * @param L lower bound (inclusive)
 * @param R upper bound (exclusive)
 * @note R >= L && R - L >= n
 * @return std::set<int>
 */
std::set<int> randSet(int N, int L, int R);
