#include "Extractor.h"

Extractor::Extractor(const std::string& key)
    : _key(key) {}

std::string Extractor::getKey() const {
    return _key;
}