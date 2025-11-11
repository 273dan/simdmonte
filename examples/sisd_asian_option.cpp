#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/pricer_sisd.h"
#include <cstdio>
using namespace simdmonte;

// const SISDAsianTestCase ATMFixedCall {
//   /* test name   */ "ATMFixedCall",
//   /* spot        */ 100.0f,                           
//   /* strike      */ 100.0f,
//   /* volatility  */ 0.20f,
//   /* risk free   */ 0.05f,
//   /* expiry      */ 1.0f,
//   /* option type */ AsianOption::OptionType::Call,
//   /* strike type */ AsianOption::StrikeType::Fixed,
//   /* avg period  */ 1.0f,
//   /* exp price   */ 5.77f,
// };

int main() {
  
  MarketData market{100.0f, 0.05f, 0.20f};
  Params params{252, 100'000, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller};
  AsianOption option{100.0f, 1.0f, AsianOption::OptionType::Call,AsianOption::StrikeType::Fixed, 1.0f};
  PricerSISD pricer{params};

  float price = pricer.price(option, market);
  printf("%f\n", price);



}
