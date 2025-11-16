#include "simdmonte/option/option.h"

#include <vector>
namespace simdmonte {

Option::~Option() = default;
Option::Option(float K, float T) : strike(K), expiry(T) {};
}  // namespace simdmonte
