#include <gtest/gtest.h>
#include "adslot.hpp"

TEST(AdSlotTest, QualityScoreUpdate) {
    AdSlot slot("top", 5.0, 0.2);
    slot.update_quality_score("adv1", true, 0.1);   // First click (clicked)
    slot.update_quality_score("adv1", false, 0.1);  // Second click (not clicked)

    double quality = slot.get_quality_score("adv1");
    EXPECT_GT(quality, 0.9);  // Expect quality > 0.9 but < 1 due to learning rate adjustment
}

TEST(AdSlotTest, ReservePriceAndCTR) {
    AdSlot slot("top", 5.0, 0.2);

    EXPECT_EQ(slot.get_position(), "top");
    EXPECT_DOUBLE_EQ(slot.get_reserve_price(), 5.0);
    EXPECT_DOUBLE_EQ(slot.get_base_ctr(), 0.2);
}
