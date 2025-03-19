#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <zlib.h>

struct MarketData {
    long symbolId;
    double price;
    long volume;
    std::string timestamp;
};

class DataCompressor {
public:
    static std::vector<unsigned char> compressData(const std::vector<unsigned char>& data) {
        uLong sourceLen = data.size();
        uLong destLen = compressBound(sourceLen);
        std::vector<unsigned char> compressedData(destLen);

        int result = compress(compressedData.data(), &destLen, data.data(), sourceLen);
        if (result != Z_OK) {
            throw std::runtime_error("Compression failed");
        }

        compressedData.resize(destLen);
        return compressedData;
    }

    static std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData) {
        uLong sourceLen = compressedData.size();
        uLong destLen = sourceLen * 4;
        std::vector<unsigned char> decompressedData(destLen);

        int result = uncompress(decompressedData.data(), &destLen, compressedData.data(), sourceLen);
        if (result != Z_OK) {
            throw std::runtime_error("Decompression failed");
        }

        decompressedData.resize(destLen);
        return decompressedData;
    }
};

class DataSerializer {
public:
    static std::vector<unsigned char> serializeMarketData(const MarketData& data) {
        size_t size = sizeof(data.symbolId) + sizeof(data.price) + sizeof(data.volume) + data.timestamp.size() + 1;
        std::vector<unsigned char> serializedData(size);

        unsigned char* buffer = serializedData.data();
        std::memcpy(buffer, &data.symbolId, sizeof(data.symbolId));
        buffer += sizeof(data.symbolId);
        std::memcpy(buffer, &data.price, sizeof(data.price));
        buffer += sizeof(data.price);
        std::memcpy(buffer, &data.volume, sizeof(data.volume));
        buffer += sizeof(data.volume);
        std::memcpy(buffer, data.timestamp.c_str(), data.timestamp.size() + 1);

        return serializedData;
    }

    static MarketData deserializeMarketData(const std::vector<unsigned char>& serializedData) {
        MarketData data;
        const unsigned char* buffer = serializedData.data();

        std::memcpy(&data.symbolId, buffer, sizeof(data.symbolId));
        buffer += sizeof(data.symbolId);
        std::memcpy(&data.price, buffer, sizeof(data.price));
        buffer += sizeof(data.price);
        std::memcpy(&data.volume, buffer, sizeof(data.volume));
        buffer += sizeof(data.volume);
        data.timestamp = std::string(reinterpret_cast<const char*>(buffer));

        return data;
    }
};

class DataCache {
public:
    void addToCache(long symbolId, const MarketData& data) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        cache[symbolId] = data;
    }

    bool getFromCache(long symbolId, MarketData& data) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        auto it = cache.find(symbolId);
        if (it != cache.end()) {
            data = it->second;
            return true;
        }
        return false;
    }

    void printCache() {
        std::lock_guard<std::mutex> lock(cacheMutex);
        for (const auto& entry : cache) {
            std::cout << "SymbolID: " << entry.second.symbolId
                      << ", Price: " << entry.second.price
                      << ", Volume: " << entry.second.volume
                      << ", Timestamp: " << entry.second.timestamp << std::endl;
        }
    }

private:
    std::unordered_map<long, MarketData> cache;
    std::mutex cacheMutex;
};

class RealTimeDataProcessor {
public:
    RealTimeDataProcessor(DataCache& cache) : cache(cache) {}

    void processData(const MarketData& data) {
        std::vector<unsigned char> serializedData = DataSerializer::serializeMarketData(data);
        std::vector<unsigned char> compressedData = DataCompressor::compressData(serializedData);

        cache.addToCache(data.symbolId, data);

        MarketData deserializedData = DataSerializer::deserializeMarketData(
            DataCompressor::decompressData(compressedData));

        std::cout << "Processed Market Data: SymbolID " << deserializedData.symbolId
                  << ", Price " << deserializedData.price
                  << ", Volume " << deserializedData.volume
                  << ", Timestamp " << deserializedData.timestamp << std::endl;
    }

private:
    DataCache& cache;
};

class MarketSimulator {
public:
    MarketSimulator(RealTimeDataProcessor& processor) : processor(processor) {}

    void generateMarketData(int cycles) {
        for (int i = 0; i < cycles; ++i) {
            MarketData data;
            data.symbolId = rand() % 1000;
            data.price = 100.0 + rand() % 500;
            data.volume = rand() % 1000 + 1;
            data.timestamp = getCurrentTimestamp();
            processor.processData(data);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

private:
    RealTimeDataProcessor& processor;

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::string timestamp = std::ctime(&time);
        timestamp.pop_back();  // Remove newline character
        return timestamp;
    }
};

int main() {
    DataCache cache;
    RealTimeDataProcessor processor(cache);
    MarketSimulator simulator(processor);

    simulator.generateMarketData(10);

    cache.printCache();

    return 0;
}
