#include <gtest/gtest.h>
#include "advertiser.hpp"
#include "adslot.hpp"
#include "auction_engine.hpp"

TEST(AuctionEngineTest, AuctionWinnerSelection) {
    AuctionEngine engine(0.01);
    auto adv1 = std::make_shared<Advertiser>("adv1", 100.0, {{"top", 10.0}}, BiddingStrategy::TRUTHFUL);
    auto adv2 = std::make_shared<Advertiser>("adv2", 100.0, {{"top", 15.0}}, BiddingStrategy::AGGRESSIVE);

    auto slot = std::make_shared<AdSlot>("top", 5.0, 0.2);

    engine.add_advertiser(adv1);
    engine.add_advertiser(adv2);
    engine.add_slot(slot);

    auto results = engine.run_auction_round();
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].winner_id, "adv2");
    EXPECT_GE(results[0].clearing_price, 5.0);
}

TEST(AuctionEngineTest, PredictCTR) {
    AuctionEngine engine(0.01);
    auto adv = std::make_shared<Advertiser>("adv", 100.0, {{"top", 10.0}}, BiddingStrategy::TRUTHFUL);
    auto slot = std::make_shared<AdSlot>("top", 5.0, 0.2);

    adv->get_click_history().push_back({std::chrono::system_clock::now(), true, 1.0});
    adv->get_click_history().push_back({std::chrono::system_clock::now(), false, 1.0});

    CTR ctr = engine.predict_ctr(*adv, *slot);
    EXPECT_GT(ctr, 0.1);  // Expect ctr to be reasonable based on clicks
}
