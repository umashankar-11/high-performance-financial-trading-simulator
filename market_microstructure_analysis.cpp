#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>
#include <ctime>
#include <cmath>

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
                  << ", Side: " << side << std::endl;
    }
};

class OrderBook {
private:
    std::map<double, std::queue<Order>> buyOrders;
    std::map<double, std::queue<Order>> sellOrders;

public:
    void addOrder(const Order& order) {
        if (order.side == "BUY") {
            buyOrders[order.price].push(order);
        } else if (order.side == "SELL") {
            sellOrders[order.price].push(order);
        }
    }

    void removeOrder(const std::string& orderId, const std::string& side) {
        if (side == "BUY") {
            for (auto& [price, queue] : buyOrders) {
                if (!queue.empty() && queue.front().orderId == orderId) {
                    queue.pop();
                    break;
                }
            }
        } else if (side == "SELL") {
            for (auto& [price, queue] : sellOrders) {
                if (!queue.empty() && queue.front().orderId == orderId) {
                    queue.pop();
                    break;
                }
            }
        }
    }

    void matchOrders() {
        while (!buyOrders.empty() && !sellOrders.empty()) {
            auto buyIt = buyOrders.rbegin();
            auto sellIt = sellOrders.begin();

            if (buyIt->first >= sellIt->first) {
                double price = sellIt->first;
                double quantity = std::min(buyIt->second.front().quantity, sellIt->second.front().quantity);

                std::cout << "Match: " << quantity << " units at price " << price << std::endl;

                buyIt->second.front().quantity -= quantity;
                sellIt->second.front().quantity -= quantity;

                if (buyIt->second.front().quantity == 0) {
                    buyIt->second.pop();
                    if (buyIt->second.empty()) {
                        buyOrders.erase(buyIt->first);
                    }
                }

                if (sellIt->second.front().quantity == 0) {
                    sellIt->second.pop();
                    if (sellIt->second.empty()) {
                        sellOrders.erase(sellIt->first);
                    }
                }
            } else {
                break;
            }
        }
    }

    void printOrderBook() const {
        std::cout << "\nOrder Book:" << std::endl;
        std::cout << "Buy Orders:" << std::endl;
        for (auto& [price, queue] : buyOrders) {
            std::cout << "Price: " << price << ", Quantity: " << queue.size() << std::endl;
        }

        std::cout << "Sell Orders:" << std::endl;
        for (auto& [price, queue] : sellOrders) {
            std::cout << "Price: " << price << ", Quantity: " << queue.size() << std::endl;
        }
    }
};

class Market {
private:
    OrderBook orderBook;

public:
    void placeOrder(const Order& order) {
        orderBook.addOrder(order);
        orderBook.matchOrders();
    }

    void cancelOrder(const std::string& orderId, const std::string& side) {
        orderBook.removeOrder(orderId, side);
    }

    void viewOrderBook() const {
        orderBook.printOrderBook();
    }

    void marketSpread() const {
        if (!orderBook.buyOrders.empty() && !orderBook.sellOrders.empty()) {
            auto bestBid = orderBook.buyOrders.begin();
            auto bestAsk = orderBook.sellOrders.begin();

            double bid = bestBid->first;
            double ask = bestAsk->first;

            std::cout << "Best Bid: " << bid << ", Best Ask: " << ask
                      << ", Spread: " << (ask - bid) << std::endl;
        } else {
            std::cout << "No bid/ask data available" << std::endl;
        }
    }
};

class MarketData {
public:
    static double simulatePriceChange(double currentPrice) {
        double change = (rand() % 2001 - 1000) / 10000.0; 
        return currentPrice * (1 + change);
    }

    static void simulateMarket(Market& market, const std::string& symbol) {
        for (int i = 0; i < 50; ++i) {
            double price = 100.0; 
            double quantity = (rand() % 10) + 1; 

            if (i % 2 == 0) {
                
                Order buyOrder("BUY" + std::to_string(i), symbol, price, quantity, "BUY");
                market.placeOrder(buyOrder);
            } else {
                
                Order sellOrder("SELL" + std::to_string(i), symbol, price, quantity, "SELL");
                market.placeOrder(sellOrder);
            }

            price = simulatePriceChange(price);
            market.marketSpread();
            market.viewOrderBook();

            std::cout << "market\n";
        }
    }
};

int main() {
    srand(time(0));

    Market market;
    MarketData::simulateMarket(market, "AAPL");

    return 0;
}
