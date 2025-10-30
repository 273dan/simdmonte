#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/params.h"
#include <memory>
/*  -- TEST BOILERPLATE --  */
using namespace simdmonte;
const static int TEST_SIMS = 10000000;
const static int TEST_STEPS = 252; // TODO: Move this into test params to test different steps

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
  std::unique_ptr<MCPricerSIMD> pricer;
  const AsianTestCase& tp = GetParam();
  AsianTest() {}

  void SetUp() override {
    market = MarketData{tp.spot, tp.risk_free_rate, tp.volatility};
    params = Params{TEST_STEPS, TEST_SIMS, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller};
    option = std::make_unique<AsianOption>(tp.strike, tp.expiry, tp.option_type, tp.strike_type, tp.avg_period);
    pricer = std::make_unique<MCPricerSIMD>(params);
  }


};


TEST_P(AsianTest, AsianPricing) {
  const AsianTestCase& tp = GetParam();
  double price = pricer->price(*option, market);
  ASSERT_NEAR(price, tp.exp_price, 0.01f);

}
struct NameGenerator {
  std::string operator()(const ::testing::TestParamInfo<AsianTestCase>& info) {
    return info.param.name;
  }
};
void PrintTo(const AsianTestCase& params, ::std::ostream* os) {} // Remove params byte dump


/*  -- TEST CASES --  */

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
  /* exp price   */ 4.31f,
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
  /* exp price   */ 14.90f,
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
  /* exp price   */ 0.51f,
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
  /* exp price   */ 0.77f,
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
  /* exp price   */ 16.35f,
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
  /* exp price   */ 13.79f,
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
  /* exp price   */ 12.33f,
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
  /* exp price   */ 6.83f,
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
  /* exp price   */ 5.38f,
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
