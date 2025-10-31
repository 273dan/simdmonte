#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/params.h"
#include <memory>

/*  -- TEST BOILERPLATE --  */

using namespace simdmonte;
const static int TEST_SIMS = 1e9;

struct EuropeanTestCase {
  std::string name;
  float spot;
  float strike;
  float volatility;
  float risk_free_rate;
  float expiry;
  EuropeanOption::OptionType side;
  float exp_price;
};

class EuropeanTest : public ::testing::TestWithParam<EuropeanTestCase> {
public:
  MarketData market;
  Params params;
  std::unique_ptr<Option> option;
  MCPricer pricer;
  const EuropeanTestCase& tp = GetParam();
  EuropeanTest() {}

  void SetUp() override {
    market = MarketData{tp.spot, tp.risk_free_rate, tp.volatility};
    params = Params{1, TEST_SIMS, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller};
    option = std::make_unique<EuropeanOption>(tp.strike, tp.expiry, tp.side);
    pricer = MCPricer{params};
  }


};
TEST_P(EuropeanTest, EuropeanPricing) {
  const EuropeanTestCase& tp = GetParam();
  double price = pricer.price(*option, market);
  ASSERT_NEAR(price, tp.exp_price, 0.01f);

}
struct NameGenerator {
  std::string operator()(const ::testing::TestParamInfo<EuropeanTestCase>& info) {
    return info.param.name;
  }
};

void PrintTo(const EuropeanTestCase& params, ::std::ostream* os) {} // Remove params byte dump

/*  -- TEST CASES --  */

const EuropeanTestCase ATMCall {
  /* test name  */ "ATMCall",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 10.45f,
};
const EuropeanTestCase ATMPut {
  /* test name  */ "ATMPut",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 5.57f,
};
const EuropeanTestCase ITMCall {
  /* test name  */ "ITMCall",
  /* spot       */ 120.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 26.17f,
};
const EuropeanTestCase ITMPut {
  /* test name  */ "ITMPut",
  /* spot       */ 120.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 1.30f,
};
const EuropeanTestCase OTMCall {
  /* test name  */ "OTMCall",
  /* spot       */ 80.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 1.86f,
};
const EuropeanTestCase OTMPut {
  /* test name  */ "OTMPut",
  /* spot       */ 80.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 16.98f,
};
const EuropeanTestCase ATMHiVolCall {
  /* test name  */ "ATMHiVolCall",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.50f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 21.79f,
};
const EuropeanTestCase ATMHiVolPut {
  /* test name  */ "ATMHiVolPut",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.50f,
  /* risk free  */ 0.05f,
  /* expiry     */ 1.0f,
  /* type       */ EuropeanOption::OptionType::Put,
  /* exp price  */ 16.91f,
};
const EuropeanTestCase ATMShortExpiryCall {
  /* test name  */ "ATMShortExpiryCall",
  /* spot       */ 100.0f,                           
  /* strike     */ 100.0f,
  /* volatility */ 0.20f,
  /* risk free  */ 0.05f,
  /* expiry     */ 0.25f,
  /* type       */ EuropeanOption::OptionType::Call,
  /* exp price  */ 4.61f,
};
const EuropeanTestCase ATMShortExpiryPut {
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
    EuropeanTest,
    ::testing::Values(
      ATMCall,
      ATMPut,
      ITMCall,
      ITMPut,
      OTMCall,
      OTMPut,
      ATMHiVolCall,
      ATMHiVolPut,
      ATMShortExpiryCall,
      ATMShortExpiryPut
    ),
    NameGenerator{}
);
