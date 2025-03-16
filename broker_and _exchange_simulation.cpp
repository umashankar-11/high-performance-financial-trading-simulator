#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <string>
#include <chrono>

class Order {
public:
    enum class Type {
        BUY,
        SELL
    };

    std::string symbol;
    double price;
    int quantity;
    Type orderType;
    std::string userID;
    std::string timestamp;

    Order(std::string s, double p, int q, Type t, std::string u, std::string ts)
        : symbol(s), price(p), quantity(q), orderType(t), userID(u), timestamp(ts) {}
};

class OrderBook {
public:
    std::map<double, std::queue<Order>> buyOrders;
    std::map<double, std::queue<Order>> sellOrders;

    void addOrder(const Order& order) {
        if (order.orderType == Order::Type::BUY) {
            buyOrders[order.price].push(order);
        } else {
            sellOrders[order.price].push(order);
        }
    }

    bool matchOrders() {
        while (!buyOrders.empty() && !sellOrders.empty()) {
            auto buyIt = buyOrders.rbegin();
            auto sellIt = sellOrders.begin();

            if (buyIt->first >= sellIt->first) {
                int matchQuantity = std::min(buyIt->second.front().quantity, sellIt->second.front().quantity);
                executeTrade(buyIt->second.front(), sellIt->second.front(), matchQuantity);
                buyIt->second.front().quantity -= matchQuantity;
                sellIt->second.front().quantity -= matchQuantity;

                if (buyIt->second.front().quantity == 0) {
                    buyOrders.erase(buyIt->first);
                }
                if (sellIt->second.front().quantity == 0) {
                    sellOrders.erase(sellIt->first);
                }
            } else {
                break;
            }
        }
        return true;
    }

    void executeTrade(const Order& buyOrder, const Order& sellOrder, int quantity) {
        std::cout << "Trade Executed: " << quantity << " units of "
                  << buyOrder.symbol << " at " << sellOrder.price << " price. "
                  << "Buyer: " << buyOrder.userID << ", Seller: " << sellOrder.userID << std::endl;
    }

    void printOrderBook() const {
        std::cout << "\nOrder Book:\n";
        std::cout << "Buy Orders:\n";
        for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); ++it) {
            std::cout << "Price: " << it->first << " Quantity: " << it->second.front().quantity << std::endl;
        }

        std::cout << "Sell Orders:\n";
        for (auto it = sellOrders.begin(); it != sellOrders.end(); ++it) {
            std::cout << "Price: " << it->first << " Quantity: " << it->second.front().quantity << std::endl;
        }
    }
};

class Exchange {
public:
    OrderBook orderBook;

    void placeOrder(const Order& order) {
        orderBook.addOrder(order);
        orderBook.matchOrders();
    }

    void cancelOrder(const std::string& userID, const std::string& symbol) {
        bool orderFound = false;
        for (auto& buyOrders : orderBook.buyOrders) {
            std::queue<Order>& orders = buyOrders.second;
            std::queue<Order> tempQueue;
            while (!orders.empty()) {
                if (orders.front().userID == userID && orders.front().symbol == symbol) {
                    orders.pop();
                    orderFound = true;
                } else {
                    tempQueue.push(orders.front());
                    orders.pop();
                }
            }
            buyOrders.second = tempQueue;
        }

        for (auto& sellOrders : orderBook.sellOrders) {
            std::queue<Order>& orders = sellOrders.second;
            std::queue<Order> tempQueue;
            while (!orders.empty()) {
                if (orders.front().userID == userID && orders.front().symbol == symbol) {
                    orders.pop();
                    orderFound = true;
                } else {
                    tempQueue.push(orders.front());
                    orders.pop();
                }
            }
            sellOrders.second = tempQueue;
        }

        if (orderFound) {
            std::cout << "Order cancelled for " << userID << " on " << symbol << std::endl;
        } else {
            std::cout << "Order not found for " << userID << " on " << symbol << std::endl;
        }
    }
};

class Broker {
private:
    Exchange exchange;

public:
    void submitBuyOrder(const std::string& symbol, double price, int quantity, const std::string& userID, const std::string& timestamp) {
        Order order(symbol, price, quantity, Order::Type::BUY, userID, timestamp);
        exchange.placeOrder(order);
    }

    void submitSellOrder(const std::string& symbol, double price, int quantity, const std::string& userID, const std::string& timestamp) {
        Order order(symbol, price, quantity, Order::Type::SELL, userID, timestamp);
        exchange.placeOrder(order);
    }

    void cancelOrder(const std::string& userID, const std::string& symbol) {
        exchange.cancelOrder(userID, symbol);
    }

    void displayOrderBook() const {
        exchange.orderBook.printOrderBook();
    }
};

int main() {
    Broker broker;

    broker.submitBuyOrder("AAPL", 150.00, 100, "user1", "2025-03-15 10:00");
    broker.submitSellOrder("AAPL", 149.50, 50, "user2", "2025-03-15 10:01");
    broker.submitSellOrder("AAPL", 149.50, 60, "user3", "2025-03-15 10:02");
    broker.submitBuyOrder("AAPL", 149.50, 50, "user4", "2025-03-15 10:03");
    
    broker.displayOrderBook();
    
    broker.cancelOrder("user2", "AAPL");

    broker.displayOrderBook();

    broker.submitBuyOrder("AAPL", 149.60, 100, "user5", "2025-03-15 10:05");
    broker.submitSellOrder("AAPL", 149.60, 100, "user6", "2025-03-15 10:06");
    broker.displayOrderBook();
    
    return 0;
}
