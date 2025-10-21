#pragma once
#include <vector>
class Option {
public:
  virtual ~Option() = 0;
  virtual double payoff(const std::vector<double>& paths) const = 0;

  double strike;
  double expiry; // 1.0 = 1 year
protected:
  Option(double K, double T);
};
