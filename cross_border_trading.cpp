#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <cmath>

class Order {
public:
    std::string orderId;
    std::string symbol;
    double price;
    double quantity;
    std::string side; 
    std::string exchange;
    time_t timestamp;

    Order(std::string id, std::string sym, double pr, double qty, std::string s, std::string ex)
        : orderId(id), symbol(sym), price(pr), quantity(qty), side(s), exchange(ex) {
        timestamp = std::time(0);
    }

    void printOrder() const {
        std::cout << "Order ID: " << orderId << ", Symbol: " << symbol
                  << ", Price: " << price << ", Quantity: " << quantity
                  << ", Side: " << side << ", Exchange: " << exchange << std::endl;
    }
};

class CurrencyConverter {
public:
    static double convertCurrency(double amount, const std::string& fromCurrency, const std::string& toCurrency) {
        std::map<std::string, double> exchangeRates;

        exchangeRates["USD_EUR"] = 0.85; 
        exchangeRates["EUR_USD"] = 1.18; 
        exchangeRates["USD_GBP"] = 0.75; 
        exchangeRates["GBP_USD"] = 1.33; 

        std::string key = fromCurrency + "_" + toCurrency;
        if (exchangeRates.find(key) != exchangeRates.end()) {
            return amount * exchangeRates[key];
        } else {
            std::cout << "Currency conversion rate not available" << std::endl;
            return amount;
        }
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

class CrossBorderTradingSystem {
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

    double applyTransactionFee(double amount, const std::string& exchange) {
        std::map<std::string, double> fees = {
            {"NYSE", 0.002}, 
            {"LSE", 0.0015}, 
            {"TSE", 0.0025}  
        };

        if (fees.find(exchange) != fees.end()) {
            return amount * (1 + fees[exchange]);
        } else {
            std::cout << "Transaction fee for exchange not found. Using 0% fee." << std::endl;
            return amount;
        }
    }

    double executeCrossBorderTrade(Order order, const std::string& fromCurrency, const std::string& toCurrency) {
        double convertedAmount = CurrencyConverter::convertCurrency(order.price * order.quantity, fromCurrency, toCurrency);
        double finalAmount = applyTransactionFee(convertedAmount, order.exchange);

        std::cout << "Order executed on " << order.exchange << " for " << order.symbol << ": "
                  << order.quantity << " units at " << order.price << " " << fromCurrency
                  << ", converted to " << finalAmount << " " << toCurrency << std::endl;

        return finalAmount;
    }
};

class MarketData {
public:
    static double simulatePriceChange(double currentPrice) {
        double change = (rand() % 2001 - 1000) / 10000.0; 
        return currentPrice * (1 + change);
    }

    static void simulateMarket(CrossBorderTradingSystem& tradingSystem, const std::string& symbol, const std::string& fromCurrency, const std::string& toCurrency) {
        for (int i = 0; i < 50; ++i) {
            double price = 100.0; 
            double quantity = (rand() % 10) + 1; 
            std::string exchange = (i % 2 == 0) ? "NYSE" : "LSE";

            Order buyOrder("BUY" + std::to_string(i), symbol, price, quantity, "BUY", exchange);
            tradingSystem.placeOrder(buyOrder);

            price = simulatePriceChange(price);
            tradingSystem.viewOrderBook();

            std::cout << "trade\n";
        }
    }
};

int main() {
    srand(time(0));

    CrossBorderTradingSystem tradingSystem;
    MarketData::simulateMarket(tradingSystem, "AAPL", "USD", "EUR");

    return 0;
}
