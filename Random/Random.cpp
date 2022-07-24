#include "Random/Random.h"
#include <cstdlib>
/**
 * @brief helper function for generates integer in [l, r)
 *
 * @param l lower bound (inclusive)
 * @param r upper bound (exclusive)
 * @return the result
 */
int rand_range(int l, int r) { return rand() % (r - l) + l; }

/**
 * @brief random number between [0, 1)
 *
 * @return float
 */
float rand_float() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
