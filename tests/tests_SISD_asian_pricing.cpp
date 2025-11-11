#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/params.h"
#include "simdmonte/pricer/pricer_sisd.h"
/*  -- TEST BOILERPLATE --  */
using namespace simdmonte;
const static long TEST_SIMS = 1e6;
const static int TEST_STEPS = 252;

struct SISDAsianTestCase {
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
class SISDAsianTest : public ::testing::TestWithParam<SISDAsianTestCase> {
public:
  MarketData market;
  Params params;
  AsianOption option;
  PricerSISD pricer;
  const SISDAsianTestCase& tp = GetParam();
  SISDAsianTest() {}

  void SetUp() override {
    market = MarketData{tp.spot, tp.risk_free_rate, tp.volatility};
    params = Params{TEST_STEPS, TEST_SIMS, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller};
    option = AsianOption{tp.strike, tp.expiry, tp.option_type, tp.strike_type, tp.avg_period};
    pricer = PricerSISD{params};
  }


};


// More lenient on asian option tests as the "true" price is also from simulation
TEST_P(SISDAsianTest, SISDAsianPricing) {
  const SISDAsianTestCase& tp = GetParam();
  double price = pricer.price(option, market);
  ASSERT_NEAR(price, tp.exp_price, 0.05f);

}
struct NameGenerator {
  std::string operator()(const ::testing::TestParamInfo<SISDAsianTestCase>& info) {
    return info.param.name;
  }
};
void PrintTo(const SISDAsianTestCase& params, ::std::ostream* os) {} // Remove params byte dump


/*  -- TEST CASES --  */
/* Prices from https://www.coggit.com/freetools */

const SISDAsianTestCase ATMFixedCall {
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
const SISDAsianTestCase ATMFixedPut {
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
const SISDAsianTestCase ITMFixedCall {
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
const SISDAsianTestCase ITMFixedPut {
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
const SISDAsianTestCase OTMFixedCall {
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
const SISDAsianTestCase OTMFixedPut {
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
const SISDAsianTestCase ATMFixedHiVolCall {
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
const SISDAsianTestCase ATMFixedHiVolPut {
  /* test name   */ "ATMFixedHiVolPut",
  /* spot        */ 100.0f,                           
  /* strike      */ 100.0f,
  /* volatility  */ 0.50f,
  /* risk free   */ 0.05f,
  /* expiry      */ 1.0f,
  /* option type */ AsianOption::OptionType::Put,
  /* strike type */ AsianOption::StrikeType::Fixed,
  /* avg period  */ 1.0f,
  /* exp price   */ 9.98f,
};
const SISDAsianTestCase ATMFixedPartialCall {
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
const SISDAsianTestCase ATMFixedPartialPut {
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
    SISDAsianTest,
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
