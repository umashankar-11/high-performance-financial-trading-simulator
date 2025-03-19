#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <algorithm>

struct Order {
    long orderId;
    double price;
    long quantity;
    bool isMarketOrder;
    bool isFilled;
};

struct Trade {
    long buyOrderId;
    long sellOrderId;
    double price;
    long quantity;
};

class Cache {
public:
    Cache(size_t maxSize) : maxSize(maxSize) {}

    void addOrderToCache(const Order& order) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        if (orderCache.size() >= maxSize) {
            orderCache.erase(orderCache.begin());
        }
        orderCache[order.orderId] = order;
    }

    bool getOrderFromCache(long orderId, Order& order) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        auto it = orderCache.find(orderId);
        if (it != orderCache.end()) {
            order = it->second;
            return true;
        }
        return false;
    }

    void addTradeToCache(const Trade& trade) {
        std::lock_guard<std::mutex> lock(cacheMutex);
        if (tradeCache.size() >= maxSize) {
            tradeCache.erase(tradeCache.begin());
        }
        tradeCache.push_back(trade);
    }

    void printCache() {
        std::lock_guard<std::mutex> lock(cacheMutex);
        std::cout << "Orders in Cache:" << std::endl;
        for (auto& entry : orderCache) {
            std::cout << "OrderID: " << entry.second.orderId << ", Price: " << entry.second.price
                      << ", Quantity: " << entry.second.quantity << std::endl;
        }

        std::cout << "Trades in Cache:" << std::endl;
        for (auto& trade : tradeCache) {
            std::cout << "BuyOrderID: " << trade.buyOrderId << ", SellOrderID: " << trade.sellOrderId
                      << ", Price: " << trade.price << ", Quantity: " << trade.quantity << std::endl;
        }
    }

private:
    std::unordered_map<long, Order> orderCache;
    std::vector<Trade> tradeCache;
    size_t maxSize;
    std::mutex cacheMutex;
};

class OrderBook {
public:
    OrderBook(Cache& cache) : cache(cache), orderIdCounter(1) {}

    void addOrder(Order order) {
        std::lock_guard<std::mutex> lock(orderBookMutex);
        if (order.isMarketOrder) {
            marketOrders.push(order);
        } else {
            limitOrders.push_back(order);
            std::sort(limitOrders.begin(), limitOrders.end(), [](const Order& a, const Order& b) {
                return a.price < b.price;
            });
        }
        cache.addOrderToCache(order);
    }

    void cancelOrder(long orderId) {
        std::lock_guard<std::mutex> lock(orderBookMutex);
        for (auto& order : limitOrders) {
            if (order.orderId == orderId && !order.isFilled) {
                order.isFilled = true;
                break;
            }
        }
    }

    void processOrders() {
        std::lock_guard<std::mutex> lock(orderBookMutex);
        while (!marketOrders.empty() && !limitOrders.empty()) {
            Order marketOrder = marketOrders.front();
            Order limitOrder = limitOrders.front();

            if (marketOrder.price >= limitOrder.price) {
                executeTrade(marketOrder, limitOrder);
                marketOrders.pop();
                limitOrders.erase(limitOrders.begin());
            } else {
                break;
            }
        }
    }

    void printOrderBook() {
        std::lock_guard<std::mutex> lock(orderBookMutex);
        std::cout << "Limit Orders:" << std::endl;
        for (auto& order : limitOrders) {
            std::cout << "ID: " << order.orderId << ", Price: " << order.price
                      << ", Quantity: " << order.quantity << ", Filled: " << order.isFilled << std::endl;
        }
    }

private:
    std::vector<Order> limitOrders;
    std::queue<Order> marketOrders;
    std::mutex orderBookMutex;
    long orderIdCounter;
    Cache& cache;

    void executeTrade(Order& buyOrder, Order& sellOrder) {
        long tradedQuantity = std::min(buyOrder.quantity, sellOrder.quantity);
        buyOrder.quantity -= tradedQuantity;
        sellOrder.quantity -= tradedQuantity;
        Trade trade{buyOrder.orderId, sellOrder.orderId, sellOrder.price, tradedQuantity};
        cache.addTradeToCache(trade);
        std::cout << "Trade Executed: " << tradedQuantity << " units at price " << sellOrder.price << std::endl;
    }
};

class OrderManagementSystem {
public:
    OrderManagementSystem(size_t cacheSize) : nextOrderId(1), cache(cacheSize), orderBook(cache) {}

    void placeLimitOrder(double price, long quantity) {
        Order order;
        order.orderId = nextOrderId++;
        order.price = price;
        order.quantity = quantity;
        order.isFilled = false;
        order.isMarketOrder = false;
        orderBook.addOrder(order);
    }

    void placeMarketOrder(long quantity) {
        Order order;
        order.orderId = nextOrderId++;
        order.price = 0;
        order.quantity = quantity;
        order.isFilled = false;
        order.isMarketOrder = true;
        orderBook.addOrder(order);
    }

    void cancelOrder(long orderId) {
        orderBook.cancelOrder(orderId);
    }

    void processOrders() {
        orderBook.processOrders();
    }

    void printOrderBook() {
        orderBook.printOrderBook();
    }

    void printCache() {
        cache.printCache();
    }

private:
    long nextOrderId;
    Cache cache;
    OrderBook orderBook;
};

class MarketSimulator {
public:
    MarketSimulator(OrderManagementSystem& oms) : oms(oms) {}

    void simulateMarketData(int cycles) {
        for (int i = 0; i < cycles; ++i) {
            double price = 100.0 + rand() % 100;
            long quantity = rand() % 100 + 1;
            if (i % 2 == 0) {
                oms.placeLimitOrder(price, quantity);
            } else {
                oms.placeMarketOrder(quantity);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

private:
    OrderManagementSystem& oms;
};

int main() {
    OrderManagementSystem oms(10);
    MarketSimulator simulator(oms);

    simulator.simulateMarketData(50);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    oms.printOrderBook();
    oms.printCache();

    return 0;
}
