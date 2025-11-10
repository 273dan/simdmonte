#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/pricer.h"
#include "simdmonte/pricer/params.h"
#include <memory>
/*  -- TEST BOILERPLATE --  */
using namespace simdmonte;
const static long TEST_SIMS = 1e7;
const static int TEST_STEPS = 252;

struct AsianTestCase {
  std::string name;
  float spot;
  float strike;
  float volatility;
  float risk_free_rate;
  float expiry;
  AsianOption::OptionType option_type;
  AsianOption::StrikeType strike_type;
  float avg_period;
  float exp_price;
};
class AsianTest : public ::testing::TestWithParam<AsianTestCase> {
public:
  MarketData market;
  Params params;
  std::unique_ptr<Option> option;
  MCPricer pricer;
  const AsianTestCase& tp = GetParam();
  AsianTest() {}

  void SetUp() override {
    market = MarketData{tp.spot, tp.risk_free_rate, tp.volatility};
    params = Params{TEST_STEPS, TEST_SIMS, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller};
    option = std::make_unique<AsianOption>(tp.strike, tp.expiry, tp.option_type, tp.strike_type, tp.avg_period);
    pricer = MCPricer{params};
  }


};


// More lenient on asian option tests as the "true" price is also from simulation
TEST_P(AsianTest, AsianPricing) {
  const AsianTestCase& tp = GetParam();
  double price = pricer.price(*option, market);
  ASSERT_NEAR(price, tp.exp_price, 0.05f);

}
struct NameGenerator {
  std::string operator()(const ::testing::TestParamInfo<AsianTestCase>& info) {
    return info.param.name;
  }
};
void PrintTo(const AsianTestCase& params, ::std::ostream* os) {} // Remove params byte dump


/*  -- TEST CASES --  */
/* Prices from https://www.coggit.com/freetools */

const AsianTestCase ATMFixedCall {
  /* test name   */ "ATMFixedCall",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Call,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 5.77f,
};
const AsianTestCase ATMFixedPut {
  /* test name   */ "ATMFixedPut",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Put,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 3.35f,
};
const AsianTestCase ITMFixedCall {
  /* test name   */ "ITMFixedCall",
  /* spot        */ 120.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Call,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 22.10f,
};
const AsianTestCase ITMFixedPut {
  /* test name   */ "ITMFixedPut",
  /* spot        */ 120.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Put,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 0.17f,
};
const AsianTestCase OTMFixedCall {
  /* test name   */ "OTMFixedCall",
  /* spot        */ 80.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Call,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 0.19f,
};
const AsianTestCase OTMFixedPut {
  /* test name   */ "OTMFixedPut",
  /* spot        */ 80.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Put,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 17.28f,
};
const AsianTestCase ATMFixedHiVolCall {
  /* test name   */ "ATMFixedHiVolCall",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.50f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Call,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 12.36f,
};
const AsianTestCase ATMFixedHiVolPut {
  /* test name   */ "ATMFixedHiVolPut",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.50f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Put,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 9.89f,
};
const AsianTestCase ATMFixedPartialCall {
  /* test name   */ "ATMFixedPartialCall",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Call,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 0.25f,
  /* exp price   */ 9.40f,
};
const AsianTestCase ATMFixedPartialPut {
  /* test name   */ "ATMFixedPartialPut",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.20f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Put,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 0.25f,
  /* exp price   */ 5.15f,
};

INSTANTIATE_TEST_SUITE_P(
    AsianPricing,
    AsianTest,
    ::testing::Values(
      ATMFixedCall,
      ATMFixedPut,
      ITMFixedCall,
      ITMFixedPut,
      OTMFixedCall,
      OTMFixedPut,
      ATMFixedHiVolCall,
      ATMFixedHiVolPut,
      ATMFixedPartialCall,
      ATMFixedPartialPut
      ),
    NameGenerator{}
);
