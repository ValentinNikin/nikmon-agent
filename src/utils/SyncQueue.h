#pragma once

#include <vector>
#include <deque>
#include <mutex>

template<typename ItemType>
class SyncQueue {
public:
    void insert(ItemType&& item);
    void insertRange(std::vector<ItemType>&& items);

    std::vector<ItemType> getAll();
    ItemType getFront();

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

    auto frontItem = _queue.front();
    _queue.pop_front();

    return frontItem;
}