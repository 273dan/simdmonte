#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/pricer/pricer_sisd.h"

/*  -- TEST BOILERPLATE --  */

using namespace simdmonte;
const static long TEST_SIMS = 1e8;

struct SISDEuropeanTestCase {
  std::string name;
  float spot;
  float strike;
  float volatility;
  float risk_free_rate;
  float expiry;
  EuropeanOption::OptionType side;
  float exp_price;
};

class SISDEuropeanTest : public ::testing::TestWithParam<SISDEuropeanTestCase> {
public:
  MarketData market;
  Params params;
  EuropeanOption option;
  MCPricerSISD pricer;
  const SISDEuropeanTestCase& tp = GetParam();
  SISDEuropeanTest() {}

  void SetUp() override {
    market = MarketData{tp.spot, tp.risk_free_rate, tp.volatility};
    params = Params{1, TEST_SIMS, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller};
    option = EuropeanOption{tp.strike, tp.expiry, tp.side};
    pricer = MCPricerSISD{params};
  }


};
TEST_P(SISDEuropeanTest, SISDEuropeanPricing) {
  const SISDEuropeanTestCase& tp = GetParam();
  double price = pricer.price(option, market);
  ASSERT_NEAR(price, tp.exp_price, 0.01f);

}
struct NameGenerator {
  std::string operator()(const ::testing::TestParamInfo<SISDEuropeanTestCase>& info) {
    return info.param.name;
  }
};

void PrintTo(const SISDEuropeanTestCase& params, ::std::ostream* os) {} // Remove params byte dump

/*  -- TEST CASES --  */

const SISDEuropeanTestCase SISDATMCall {
  /* test name  */ "ATMCall",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 10.45f,
};
const SISDEuropeanTestCase SISDATMPut {
  /* test name  */ "ATMPut",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 5.57f,
};
const SISDEuropeanTestCase SISDITMCall {
  /* test name  */ "ITMCall",
  /* spot       */ 120.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 26.17f,
};
const SISDEuropeanTestCase SISDITMPut {
  /* test name  */ "ITMPut",
  /* spot       */ 120.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 1.30f,
};
const SISDEuropeanTestCase SISDOTMCall {
  /* test name  */ "OTMCall",
  /* spot       */ 80.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 1.86f,
};
const SISDEuropeanTestCase SISDOTMPut {
  /* test name  */ "OTMPut",
  /* spot       */ 80.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 16.98f,
};
const SISDEuropeanTestCase SISDATMHiVolCall {
  /* test name  */ "ATMHiVolCall",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.50f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 21.79f,
};
const SISDEuropeanTestCase SISDATMHiVolPut {
  /* test name  */ "ATMHiVolPut",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.50f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 16.91f,
};
const SISDEuropeanTestCase SISDATMShortExpiryCall {
  /* test name  */ "ATMShortExpiryCall",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 0.25f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 4.61f,
};
const SISDEuropeanTestCase SISDATMShortExpiryPut {
  /* test name  */ "ATMShortExpiryPut",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 0.25f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 3.37f,
};

INSTANTIATE_TEST_SUITE_P(
    EuropeanPricing,
    SISDEuropeanTest,
    ::testing::Values(
      SISDATMCall,
      SISDATMPut,
      SISDITMCall,
      SISDITMPut,
      SISDOTMCall,
      SISDOTMPut,
      SISDATMHiVolCall,
      SISDATMHiVolPut,
      SISDATMShortExpiryCall,
      SISDATMShortExpiryPut
    ),
    NameGenerator{}
);
