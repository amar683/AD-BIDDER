#include <vector>
#include <memory>
#include <unordered_map>
#include <random>
#include <algorithm>
#include "advertiser.hpp"
#include "adslot.hpp"
struct AuctionResult {
    AdvertiserId winner_id;                       // ID of the winning advertiser
    Price clearing_price;                         // Price paid by the winner
    std::unordered_map<AdvertiserId, Price> original_bids;  // Bids placed by each advertiser
};
class AuctionEngine {
private:
    std::vector<std::shared_ptr<Advertiser>> advertisers_;
    std::vector<std::shared_ptr<AdSlot>> slots_;
    double learning_rate_;
    std::mt19937 rng_;

    CTR predict_ctr(const Advertiser& advertiser, const AdSlot& slot) {
        std::vector<ClickData> history = advertiser.get_click_history();
        if (history.empty()) return slot.get_base_ctr();

        int successes = std::count_if(history.begin(), history.end(),
                                      [](const ClickData& data) { return data.clicked; });
        int trials = history.size();
        //as we cant use beta distribution we will use gamma distribution because beta distribution is not available in c++17
        //std::beta_distribution<double> dist(successes + 1, trials - successes + 1);
        double alpha = successes + 1;
        double beta = trials - successes + 1;
        std::gamma_distribution<double> gamma_alpha(alpha, 1.0);
        std::gamma_distribution<double> gamma_beta(beta, 1.0);
        double sample_alpha = gamma_alpha(rng_);
        double sample_beta = gamma_beta(rng_);
        return (sample_alpha / (sample_alpha + sample_beta)) * slot.get_base_ctr();
    }

public:
    AuctionEngine(double learning_rate = 0.01)
        : learning_rate_(learning_rate), rng_(std::random_device{}()) {}

    void add_advertiser(std::shared_ptr<Advertiser> advertiser) {
        advertisers_.push_back(std::move(advertiser));
    }

    void add_slot(std::shared_ptr<AdSlot> slot) {
        slots_.push_back(std::move(slot));
    }

    std::vector<AuctionResult> run_auction_round() {
        std::vector<AuctionResult> results;
        results.reserve(slots_.size());

        for (const auto& slot : slots_) {
            std::unordered_map<AdvertiserId, Price> bids;
            
            for (const auto& advertiser : advertisers_) {
                if (!advertiser->can_afford(slot->get_reserve_price())) continue;
                CTR predicted_ctr = predict_ctr(*advertiser, *slot);
                Price bid = advertiser->calculate_bid(slot->get_position(), predicted_ctr);
                if (bid >= slot->get_reserve_price()) {
                    bids[advertiser->get_id()] = bid;
                }
            }

            AuctionResult result = run_gsp_auction(*slot, bids);
            if (!result.winner_id.empty()) {
                results.push_back(std::move(result));
            }
        }

        return results;
    }

private:
    AuctionResult run_gsp_auction(const AdSlot& slot, const std::unordered_map<AdvertiserId, Price>& bids) {
        if (bids.empty()) return AuctionResult{};
        std::vector<std::pair<AdvertiserId, Price>> adjusted_bids;
        for (const auto& [id, bid] : bids) {
            QualityScore quality = slot.get_quality_score(id);
            adjusted_bids.emplace_back(id, bid * quality);
        }
        std::sort(adjusted_bids.begin(), adjusted_bids.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });
        const auto& winner = adjusted_bids[0];
        QualityScore winner_quality = slot.get_quality_score(winner.first);
        Price clearing_price = slot.get_reserve_price();
        if (adjusted_bids.size() > 1) {
            clearing_price = std::max(slot.get_reserve_price(), adjusted_bids[1].second / winner_quality);
        }
        return AuctionResult{winner.first, clearing_price, bids};
    }
};
