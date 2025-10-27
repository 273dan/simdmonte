#include <gtest/gtest.h>
#include "simdmonte/misc/market_data.h"
#include "simdmonte/option/option_european.h"
#include "simdmonte/pricer/pricer_simd.h"
#include "simdmonte/pricer/params.h"
#include <memory>

using namespace simdmonte;

TEST(EuropeanPricing, ATMCall) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 10.45f, 0.01f);

}
TEST(EuropeanPricing, ATMPut) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Put);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 5.57f, 0.01f);

}

TEST(EuropeanPricing, ITMCall) {
  MarketData market(
      120.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 26.17f, 0.01f);
}
TEST(EuropeanPricing, ITMPut) {
  MarketData market(
      120.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Put);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 1.30f, 0.01f);
}
TEST(EuropeanPricing, OTMCall) {
  MarketData market(
      80.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 1.86f, 0.01f);
}
TEST(EuropeanPricing, OTMPut) {
  MarketData market(
      80.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Put);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 16.98f, 0.01f);
}

TEST(EuropeanPricing, ATMHighVolCall) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.50f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 21.79f, 0.01f);
}
TEST(EuropeanPricing, ATMHighVolPut) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.50f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 1.0f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Put);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 16.91f, 0.01f);
}
TEST(EuropeanPricing, ATMShortExpiryCall) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 0.25f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Call);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 4.61f, 0.01f);
}
TEST(EuropeanPricing, ATMShortExpiryPut) {
  MarketData market(
      100.0f, // Spot
      0.05f,  // Risk free rate
      0.20f   // Volatility
      );
  double strike = 100.0f;
  double expiry = 0.25f; // Years decimal

  int n_sims = 1000000000;
  int n_steps = 1;

  Params params(n_steps, n_sims, params::UnderlyingModel::GBM, params::NormalMethod::BoxMuller);

  std::unique_ptr<Option> option =
    std::make_unique<EuropeanOption>(strike, expiry, EuropeanOption::OptionType::Put);

  std::unique_ptr<IPricer> pricer = 
    std::make_unique<MCPricerSIMD>(params);

  double price = pricer->price(*option, market);

  ASSERT_NEAR(price, 3.37f, 0.01f);
}
