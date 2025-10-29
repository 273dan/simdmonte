#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_asian.h"
#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/params.h"
#include <memory>
using namespace simdmonte;
const static int TEST_SIMS = 10000000;

TEST(AsianPricing, ATMFixedCall) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 5.77, 0.01f);
}
TEST(AsianPricing, ATMFixedPut) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Put, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 4.31f, 0.01f);
}
TEST(AsianPricing, ITMFixedCall) {
  MarketData market(
      120.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 14.90f, 0.01f);
}
TEST(AsianPricing, ITMFixedPut) {
  MarketData market(
      120.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Put, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 0.51f, 0.01f);
}
TEST(AsianPricing, OTMFixedCall) {
  MarketData market(
      80.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 0.77f, 0.01f);
}
TEST(AsianPricing, OTMFixedPut) {
  MarketData market(
      80.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Put, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 16.35f, 0.01f);
}
TEST(AsianPricing, ATMFixedCallHiVol) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.5f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 13.79f, 0.01f);
}
TEST(AsianPricing, ATMFixedPutHiVol) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.5f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 1.0f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Put, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 12.33f, 0.01f);
}
TEST(AsianPricing, ATMFixedCallPartial) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.5f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 0.25f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Call, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 6.83f, 0.01f);
}
TEST(AsianPricing, ATMFixedPutPartial) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.5f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal
  double avg_period = 0.25f;

  int n_sims = TEST_SIMS;
  int n_steps = 252;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<AsianOption>(strike, expiry, AsianOption::OptionType::Put, AsianOption::StrikeType::Fixed, avg_period);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 5.38f, 0.01f);
}
