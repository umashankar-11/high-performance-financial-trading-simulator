#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <mutex>

class MarketData {
public:
    std::string symbol;
    double price;
    double volume;
    time_t timestamp;

    MarketData(std::string sym, double pr, double vol) 
        : symbol(sym), price(pr), volume(vol) {
        timestamp = std::time(0);
    }

    void printMarketData() const {
        std::cout << "Symbol: " << symbol 
                  << ", Price: " << price 
                  << ", Volume: " << volume
                  << ", Timestamp: " << timestamp << std::endl;
    }
};

class MarketFeed {
private:
    std::map<std::string, MarketData> marketDataFeed;
    std::mutex feedMutex;

public:
    void updateMarketData(const MarketData& data) {
        std::lock_guard<std::mutex> lock(feedMutex);
        marketDataFeed[data.symbol] = data;
    }

    void printMarketFeed() const {
        std::lock_guard<std::mutex> lock(feedMutex);
        for (const auto& data : marketDataFeed) {
            data.second.printMarketData();
        }
    }

    MarketData getMarketData(const std::string& symbol) const {
        std::lock_guard<std::mutex> lock(feedMutex);
        if (marketDataFeed.find(symbol) != marketDataFeed.end()) {
            return marketDataFeed.at(symbol);
        }
        return MarketData(symbol, 0.0, 0.0);
    }
};

class PriceFeedSimulator {
private:
    std::vector<std::string> symbols;
    std::vector<MarketFeed>& feeds;

public:
    PriceFeedSimulator(std::vector<std::string> sym, std::vector<MarketFeed>& f)
        : symbols(sym), feeds(f) {}

    double simulatePriceChange(double currentPrice) {
        double change = (rand() % 2001 - 1000) / 10000.0; 
        return currentPrice * (1 + change);
    }

    double simulateVolumeChange(double currentVolume) {
        return currentVolume + (rand() % 1000 - 500); 
    }

    void startSimulating() {
        while (true) {
            for (auto& feed : feeds) {
                for (const auto& symbol : symbols) {
                    MarketData data(symbol, simulatePriceChange(100.0), simulateVolumeChange(1000));
                    feed.updateMarketData(data);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
    }
};

class TradingSystem {
private:
    std::vector<MarketFeed> marketFeeds;

public:
    void addMarketFeed(const MarketFeed& feed) {
        marketFeeds.push_back(feed);
    }

    void showMarketData() const {
        for (const auto& feed : marketFeeds) {
            feed.printMarketFeed();
        }
    }

    void subscribeToSymbol(const std::string& symbol) {
        for (auto& feed : marketFeeds) {
            MarketData data = feed.getMarketData(symbol);
            data.printMarketData();
        }
    }
};

class Order {
public:
    std::string orderId;
    std::string symbol;
    double price;
    double quantity;
    std::string side; 
    time_t timestamp;

    Order(std::string id, std::string sym, double pr, double qty, std::string s)
        : orderId(id), symbol(sym), price(pr), quantity(qty), side(s) {
        timestamp = std::time(0);
    }

    void printOrder() const {
        std::cout << "Order ID: " << orderId << ", Symbol: " << symbol
                  << ", Price: " << price << ", Quantity: " << quantity
                  << ", Side: " << side << ", Timestamp: " << timestamp << std::endl;
    }
};

class OrderBook {
private:
    std::map<std::string, std::map<double, std::queue<Order>>> buyOrders;
    std::map<std::string, std::map<double, std::queue<Order>>> sellOrders;

public:
    void addOrder(const Order& order) {
        if (order.side == "BUY") {
            buyOrders[order.symbol][order.price].push(order);
        } else if (order.side == "SELL") {
            sellOrders[order.symbol][order.price].push(order);
        }
    }

    void removeOrder(const std::string& orderId, const std::string& side, const std::string& symbol) {
        if (side == "BUY") {
            for (auto& [price, queue] : buyOrders[symbol]) {
                if (!queue.empty() && queue.front().orderId == orderId) {
                    queue.pop();
                    break;
                }
            }
        } else if (side == "SELL") {
            for (auto& [price, queue] : sellOrders[symbol]) {
                if (!queue.empty() && queue.front().orderId == orderId) {
                    queue.pop();
                    break;
                }
            }
        }
    }

    void matchOrders(const std::string& symbol) {
        while (!buyOrders[symbol].empty() && !sellOrders[symbol].empty()) {
            auto buyIt = buyOrders[symbol].rbegin();
            auto sellIt = sellOrders[symbol].begin();

            if (buyIt->first >= sellIt->first) {
                double price = sellIt->first;
                double quantity = std::min(buyIt->second.front().quantity, sellIt->second.front().quantity);

                std::cout << "Match: " << quantity << " units at price " << price << " for " << symbol << std::endl;

                
                buyIt->second.front().quantity -= quantity;
                sellIt->second.front().quantity -= quantity;

                if (buyIt->second.front().quantity == 0) {
                    buyIt->second.pop();
                    if (buyIt->second.empty()) {
                        buyOrders[symbol].erase(buyIt->first);
                    }
                }

                if (sellIt->second.front().quantity == 0) {
                    sellIt->second.pop();
                    if (sellIt->second.empty()) {
                        sellOrders[symbol].erase(sellIt->first);
                    }
                }
            } else {
                break;
            }
        }
    }

    void printOrderBook(const std::string& symbol) const {
        std::cout << "\nOrder Book for " << symbol << ":" << std::endl;
        std::cout << "Buy Orders:" << std::endl;
        for (auto& [price, queue] : buyOrders.at(symbol)) {
            std::cout << "Price: " << price << ", Quantity: " << queue.size() << std::endl;
        }

        std::cout << "Sell Orders:" << std::endl;
        for (auto& [price, queue] : sellOrders.at(symbol)) {
            std::cout << "Price: " << price << ", Quantity: " << queue.size() << std::endl;
        }
    }
};

int main() {
    srand(time(0));

    MarketFeed feed1, feed2;
    std::vector<MarketFeed> feeds = {feed1, feed2};

    PriceFeedSimulator simulator({"AAPL", "GOOG", "AMZN"}, feeds);
    std::thread simulationThread(&PriceFeedSimulator::startSimulating, &simulator);

    TradingSystem tradingSystem;
    tradingSystem.addMarketFeed(feed1);
    tradingSystem.addMarketFeed(feed2);

    OrderBook orderBook;
    Order order1("O1", "AAPL", 150.0, 100, "BUY");
    Order order2("O2", "AAPL", 152.0, 200, "SELL");
    orderBook.addOrder(order1);
    orderBook.addOrder(order2);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    tradingSystem.showMarketData();
    orderBook.matchOrders("AAPL");
    orderBook.printOrderBook("AAPL");

    simulationThread.join();

    return 0;
}
