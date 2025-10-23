#include <vector>
#include "simdmonte/option/option.h"
namespace simdmonte {


Option::~Option() = default;
Option::Option(float K, float T) : strike(K), expiry(T) {};
}
