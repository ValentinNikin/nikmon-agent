#pragma once

#include <vector>
#include <deque>
#include <mutex>

template<typename ItemType>
class WriteSyncQueue {
public:
    virtual ~WriteSyncQueue() = default;
    virtual void insert(ItemType&& item) = 0;
    virtual void insertRange(std::vector<ItemType>&& items) = 0;
};

template<typename ItemType>
class ReadSyncQueue {
public:
    virtual ~ReadSyncQueue() = default;
    virtual std::vector<ItemType> getAll() = 0;
    virtual ItemType getFront() = 0;
};

template<typename ItemType>
class SyncQueue : public WriteSyncQueue<ItemType>, public ReadSyncQueue<ItemType> {
public:
    void insert(ItemType&& item) override;
    void insertRange(std::vector<ItemType>&& items) override;

    std::vector<ItemType> getAll() override;
    ItemType getFront() override;

private:
    std::mutex _mutex;
    std::deque<ItemType> _queue;
};

template<typename ItemType>
void SyncQueue<ItemType>::insert(ItemType&& item) {
    std::lock_guard<std::mutex> lg(_mutex);
    _queue.push_back(std::move(item));
}

template<typename ItemType>
void SyncQueue<ItemType>::insertRange(std::vector<ItemType>&& items) {
    std::lock_guard<std::mutex> lg(_mutex);
    _queue.insert(_queue.end(), std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));
    items.clear();
}

template<typename ItemType>
std::vector<ItemType> SyncQueue<ItemType>::getAll() {
    if (_queue.empty()) return {};

    std::lock_guard<std::mutex> lg(_mutex);

    std::vector<ItemType> results;
    results.reserve(_queue.size());
    results.insert(results.end(), std::make_move_iterator(_queue.begin()), std::make_move_iterator(_queue.end()));

    _queue.clear();

    return results;
}

template<typename ItemType>
ItemType SyncQueue<ItemType>::getFront() {
    std::lock_guard<std::mutex> lg(_mutex);

    auto frontItem = std::move(_queue.front());
    _queue.pop_front();

    return frontItem;
}