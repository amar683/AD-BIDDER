#include "gtest/gtest.h"
#include "advertiser.hpp"

TEST(AdvertiserTest, BudgetDeduction) {
    ValueDistribution values = {{"top", 10.0}, {"side", 5.0}};
    Advertiser advertiser("adv1", 100.0, values, BiddingStrategy::TRUTHFUL);

    ASSERT_TRUE(advertiser.can_afford(50.0));
    EXPECT_TRUE(advertiser.deduct_budget(50.0));
    EXPECT_DOUBLE_EQ(advertiser.get_remaining_budget(), 50.0);

    ASSERT_FALSE(advertiser.deduct_budget(60.0));
}

TEST(AdvertiserTest, CalculateBid) {
    ValueDistribution values = {{"top", 10.0}, {"side", 5.0}};
    Advertiser advertiser("adv2", 100.0, values, BiddingStrategy::AGGRESSIVE);

    EXPECT_DOUBLE_EQ(advertiser.calculate_bid("top", 0.5), 6.0);  // 10 * 0.5 * 1.2
    EXPECT_DOUBLE_EQ(advertiser.calculate_bid("side", 0.5), 3.0); // 5 * 0.5 * 1.2
}
