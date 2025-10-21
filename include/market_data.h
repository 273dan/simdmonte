#pragma once
struct MarketData {
  double spot;
  double risk_free_rate;
  double volatility;
  MarketData(double s, double r, double v)
      : spot(s), risk_free_rate(r), volatility(v) {}
};
