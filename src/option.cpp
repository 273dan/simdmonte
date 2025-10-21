#include <vector>
#include "option.h"

Option::~Option() = default;
Option::Option(double K, double T) : strike(K), expiry(T) {};
