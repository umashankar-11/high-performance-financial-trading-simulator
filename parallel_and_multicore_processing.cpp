#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
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

class OrderBook {
public:
    OrderBook() : orderIdCounter(1) {}

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
    }

    void cancelOrder(long orderId) {
        std::lock_guard<std::mutex> lock(orderBookMutex);
        for (auto& order : limitOrders) {
            if (order.orderId == orderId && !order.isFilled) {
                order.isFilled = true;  // Mark as cancelled
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

    void executeTrade(Order& marketOrder, Order& limitOrder) {
        long tradedQuantity = std::min(marketOrder.quantity, limitOrder.quantity);
        marketOrder.quantity -= tradedQuantity;
        limitOrder.quantity -= tradedQuantity;
        std::cout << "Trade Executed: " << tradedQuantity << " units at price " << limitOrder.price << std::endl;
    }
};

class OrderManagementSystem {
public:
    OrderManagementSystem() : nextOrderId(1) {}

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

private:
    long nextOrderId;
    OrderBook orderBook;
};

class OrderProcessor {
public:
    OrderProcessor(OrderManagementSystem& oms) : oms(oms), stopFlag(false) {}

    void startProcessing() {
        processingThread = std::thread(&OrderProcessor::processOrders, this);
    }

    void stopProcessing() {
        stopFlag = true;
        if (processingThread.joinable()) {
            processingThread.join();
        }
    }

    void processOrders() {
        while (!stopFlag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            oms.processOrders();
        }
    }

private:
    OrderManagementSystem& oms;
    std::thread processingThread;
    std::atomic<bool> stopFlag;
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
    OrderManagementSystem oms;
    OrderProcessor processor(oms);
    MarketSimulator simulator(oms);

    processor.startProcessing();

    simulator.simulateMarketData(100);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    oms.printOrderBook();

    processor.stopProcessing();

    return 0;
}
