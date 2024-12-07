#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <algorithm>
#include <ctime>

class Order {
public:
    enum class Type { BUY, SELL };
    Order(int id, Type type, double price, int quantity)
        : id(id), type(type), price(price), quantity(quantity) {}

    int id;
    Type type;
    double price;
    int quantity;

    bool operator<(const Order& other) const {
        // For BUY orders, we want the highest price to be prioritized
        if (type == Type::BUY && other.type == Type::BUY)
            return price < other.price;
        // For SELL orders, we want the lowest price to be prioritized
        if (type == Type::SELL && other.type == Type::SELL)
            return price > other.price;
        return false;
    }
};

class OrderBook {
public:
    void addOrder(Order order) {
        if (order.type == Order::Type::BUY)
            buyOrders.push(order);
        else
            sellOrders.push(order);
    }

    void matchOrders() {
        while (!buyOrders.empty() && !sellOrders.empty()) {
            Order buyOrder = buyOrders.top();
            Order sellOrder = sellOrders.top();

            if (buyOrder.price >= sellOrder.price) {
                // Match the order
                std::cout << "Matched Order: Buy Order #" << buyOrder.id
                          << " with Sell Order #" << sellOrder.id
                          << " at price " << sellOrder.price << std::endl;

                // Execute the trade: assume the full quantity of the orders are matched
                buyOrders.pop();
                sellOrders.pop();
            } else {
                break; // No more matches possible
            }
        }
    }

private:
    std::priority_queue<Order> buyOrders;
    std::priority_queue<Order> sellOrders;
};

class Trader {
public:
    Trader(int id, double initialCash) : id(id), cash(initialCash), shares(0) {}

    void placeOrder(Order::Type type, double price, int quantity, OrderBook& orderBook) {
        if (type == Order::Type::BUY && cash < price * quantity) {
            std::cout << "Trader #" << id << " cannot afford the buy order." << std::endl;
            return;
        }

        if (type == Order::Type::SELL && shares < quantity) {
            std::cout << "Trader #" << id << " does not have enough shares to sell." << std::endl;
            return;
        }

        int orderId = ++orderIdCounter;
        Order order(orderId, type, price, quantity);
        orderBook.addOrder(order);

        // Update the portfolio based on the order type
        if (type == Order::Type::BUY) {
            cash -= price * quantity;
            shares += quantity;
        } else {
            cash += price * quantity;
            shares -= quantity;
        }

        std::cout << "Trader #" << id << " placed " 
                  << (type == Order::Type::BUY ? "buy" : "sell")
                  << " order for " << quantity << " shares at price " << price << std::endl;
    }

private:
    int id;
    double cash;
    int shares;
    int orderIdCounter = 0;
};

class Market {
public:
    Market() : randomEngine(time(0)) {}

    double generatePrice() {
        std::uniform_real_distribution<double> dist(100.0, 200.0);
        return dist(randomEngine);
    }

private:
    std::default_random_engine randomEngine;
};

int main() {
    Market market;
    OrderBook orderBook;
    Trader trader1(1, 10000.0); // Trader with $10,000
    Trader trader2(2, 5000.0);  // Trader with $5,000

    // Simulate placing orders
    trader1.placeOrder(Order::Type::BUY, market.generatePrice(), 50, orderBook);
    trader2.placeOrder(Order::Type::SELL, market.generatePrice(), 30, orderBook);
    trader1.placeOrder(Order::Type::BUY, market.generatePrice(), 20, orderBook);
    trader2.placeOrder(Order::Type::SELL, market.generatePrice(), 20, orderBook);

    // Match orders
    orderBook.matchOrders();

    return 0;
}
