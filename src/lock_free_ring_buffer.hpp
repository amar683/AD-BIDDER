#ifndef LOCK_FREE_RING_BUFFER_HPP
#define LOCK_FREE_RING_BUFFER_HPP

#include<array>
#include<atomic>
#include<vector>

template<typename T, size_t Size>
class LockFreeRingBuffer {
    private:
        std::array<T, Size> buffer;
        std::atomic<size_t> write_index_{0};
    
    public:
    void push(const T& item){
        size_t index = write_index_.fetch_add(1,std::memory_order_relaxed)%size;
        buffer[index] = item;
    }
    std::vector<T> get_all() const{
        std::vector<T> result;
        result.reserve(size);
        for(const auto& item:buffer_){
            result.push_back(item);
        }
        return result;
    }
};

#endif // LOCK_FREE_RING_BUFFER_HPP