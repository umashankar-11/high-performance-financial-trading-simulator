#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <cstdlib>

struct MarketData {
    double price;
    double bidPrice;
    double askPrice;
    long timestamp;
    long volume;
};

struct Order {
    long orderId;
    double price;
    long quantity;
    bool isFilled;
    bool isCancelled;
    bool isMarketOrder;
};

enum OrderType {LIMIT, MARKET};

class OrderBook {
public:
    void addOrder(Order order) {
        if (order.isMarketOrder) {
            executeMarketOrder(order);
        } else {
            limitOrderBook.push_back(order);
        }
    }

    void cancelOrder(long orderId) {
        for (auto& order : limitOrderBook) {
            if (order.orderId == orderId && !order.isFilled && !order.isCancelled) {
                order.isCancelled = true;
                break;
            }
        }
    }

    void executeOrders() {
        while (!limitOrderBook.empty() && !marketOrderQueue.empty()) {
            Order& marketOrder = marketOrderQueue.front();
            Order& limitOrder = limitOrderBook.front();
            
            if (marketOrder.price >= limitOrder.price) {
                executeTrade(marketOrder, limitOrder);
                marketOrder.isFilled = true;
                limitOrder.isFilled = true;
                limitOrderBook.erase(limitOrderBook.begin());
                marketOrderQueue.pop();
            } else {
                break;
            }
        }
    }

    void printOrderBook() {
        for (auto& order : limitOrderBook) {
            std::cout << "Order ID: " << order.orderId << ", Price: " << order.price
                      << ", Quantity: " << order.quantity << ", Filled: " << order.isFilled
                      << ", Cancelled: " << order.isCancelled << std::endl;
        }
    }

private:
    std::vector<Order> limitOrderBook;
    std::queue<Order> marketOrderQueue;

    void executeMarketOrder(Order& marketOrder) {
        for (auto& limitOrder : limitOrderBook) {
            if (!limitOrder.isFilled && !limitOrder.isCancelled) {
                executeTrade(marketOrder, limitOrder);
                marketOrder.isFilled = true;
                limitOrder.isFilled = true;
                break;
            }
        }
    }

    void executeTrade(Order& buyOrder, Order& sellOrder) {
        double tradePrice = sellOrder.price;
        long tradedQuantity = std::min(buyOrder.quantity, sellOrder.quantity);
        buyOrder.quantity -= tradedQuantity;
        sellOrder.quantity -= tradedQuantity;
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
        order.isCancelled = false;
        order.isMarketOrder = false;
        orderBook.addOrder(order);
    }

    void placeMarketOrder(long quantity) {
        Order order;
        order.orderId = nextOrderId++;
        order.price = 0;
        order.quantity = quantity;
        order.isFilled = false;
        order.isCancelled = false;
        order.isMarketOrder = true;
        orderBook.addOrder(order);
    }

    void cancelOrder(long orderId) {
        orderBook.cancelOrder(orderId);
    }

    void processOrders() {
        orderBook.executeOrders();
    }

    void printOrderBook() {
        orderBook.printOrderBook();
    }

private:
    long nextOrderId;
    OrderBook orderBook;
};

class MarketSimulator {
public:
    MarketSimulator() {
        srand(static_cast<unsigned int>(time(0)));
    }

    MarketData generateMarketData() {
        MarketData data;
        data.price = getRandomPrice();
        data.bidPrice = data.price - getRandomSpread();
        data.askPrice = data.price + getRandomSpread();
        data.volume = getRandomVolume();
        data.timestamp = getCurrentTimestamp();
        return data;
    }

private:
    double getRandomPrice() {
        return 100.0 + (rand() % 1000) / 100.0;
    }

    double getRandomSpread() {
        return (rand() % 10) / 100.0;
    }

    long getRandomVolume() {
        return rand() % 1000 + 1;
    }

    long getCurrentTimestamp() {
        return static_cast<long>(time(0));
    }
};

int main() {
    OrderManagementSystem oms;
    MarketSimulator simulator;

    for (int i = 0; i < 50; ++i) {
        MarketData data = simulator.generateMarketData();
        std::cout << "Market Price: " << data.price << ", Bid: " << data.bidPrice
                  << ", Ask: " << data.askPrice << ", Volume: " << data.volume << std::endl;

        if (i % 2 == 0) {
            oms.placeLimitOrder(data.bidPrice, rand() % 100 + 1);
        } else {
            oms.placeMarketOrder(rand() % 100 + 1);
        }
    }

    std::cout << "Order Book Before Processing:" << std::endl;
    oms.printOrderBook();

    oms.processOrders();

    std::cout << "Order Book After Processing:" << std::endl;
    oms.printOrderBook();

    oms.cancelOrder(3);

    std::cout << "Order Book After Canceling Order 3:" << std::endl;
    oms.printOrderBook();

    return 0;
}
