#include<string>
#include<atomic>
#include<unordered_map>
#include "lock_free_ring_buffer.hpp"
#include <chrono>

using AdvertiserId = std::string;
using Price = double;
using CTR = double;

enum class BiddingStrategy {
    TRUTHFUL,
    AGGRESSIVE,
    CONSERVATIVE
};

struct ClickData{
    std::chrono::system_clock::time_point timestamp;
    bool clicked;
    Price cost;    
};

using ValueDistributon = std::unordered_map<std::string, double>;

class Advertiser{
    private:
        AdvertiserId id_;
        std::atomic<double>budget_;
        std::atomic<double>spent_;
        ValueDistributon value_distribution_;
        BiddingStrategy strategy_;
        LockFreeRingBuffer<ClickData, 1000> click_history_;
    public:
        Advertiser(const AdvertiserId& id, double budget, const ValueDistributon& values, BiddingStrategy strategy)
        : id_(id), budget_(budget), spent_(0), value_distribution_(values), strategy_(strategy){}

        bool can_afford(Price amount) const{return (budget_ - spent_)>=amount;}

        bool deduct_budget(Price amount){
            double current_spent = spent_.load(std::memory_order_relaxed);
            double new_spent = current_spent + amount;
            while(new_spent<=budget_){
                if(spent_.compare_exchange_weak(current_spent, new_spent))return true;
                new_spent= current_spent+ amount;
            }
            return false;
        } 
        Price calculate_bid(const std:: string& slot_position, CTR prediction_ctr)const{
            auto value_it = value_distribution_.find(slot_position);
            if(value_it == value_distribution_.end())return 0.0;

            double base_value= value_it->second* prediction_ctr;
            switch(strategy_){
                case BiddingStrategy:: AGGRESSIVE: return base_value*1.2;
                case BiddingStrategy:: CONSERVATIVE : return base_value*0.8;
                default: base_value;
            }
        }
        const AdvertiserId& get_id() const {return id_;}
        double get_remaining_budget() const {return budget_ - spent_.load(std::memory_order_relaxed);}
        std::vector<ClickData> get_click_history() const {return click_history_.get_all();}
};
