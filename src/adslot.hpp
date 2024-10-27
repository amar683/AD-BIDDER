#include<string>
#include<unordered_map>
#include<shared_mutex>

using AdvertiserId = std::string; 
using Price = double;
using CTR = double;
using QualityScore = double;

class AdSlot{
    private:
        std::string position_;
        Price reserve_price_;
        CTR base_ctr_;
        std::unordered_map<AdvertiserId, QualityScore> quality_scores_;
        mutable std::shared_mutex quality_scores_mutex_;

    public:

        AdSlot(const std::string& position, Price reserve_price, CTR base_ctr)
        :position_(position), reserve_price_(reserve_price), base_ctr_(base_ctr){}

        QualityScore get_quality_score(const AdvertiserId &advertiserId) const{
            std::shared_lock lock(quality_scores_mutex_);
            auto it= quality_scores_.find(advertiserId);
            return (it!=quality_scores_.end())? it->second : 1.0;
        }

        void update_quality_score(const AdvertiserId &advertiser_id, bool clicked, double learning_rate){
            std::unique_lock lock(quality_scores_mutex_);
            auto& score = quality_scores_[advertiser_id];
            if(score==0.0)score =1.0;
            score = (1-learning_rate)*score + learning_rate*(clicked?1.0:0.0);
        }
        const std::string& get_position() const{return position_;}
        Price get_reserve_price() const {return reserve_price_;}
        CTR get_base_ctr() const {return base_ctr_;}
};