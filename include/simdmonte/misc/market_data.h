#pragma once
namespace simdmonte {
  struct MarketData {
    float spot;
    float risk_free_rate;
    float volatility;
    MarketData(float s, float r, float v)
        : spot(s), risk_free_rate(r), volatility(v) {}
  };
} 
