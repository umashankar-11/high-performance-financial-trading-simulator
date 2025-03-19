#include <iostream>
#include <vector>
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
};

enum OrderType {LIMIT, MARKET};

class Market {
public:
    MarketData generateMarketData() {
        MarketData data;
        data.price = getRandomPrice();
        data.bidPrice = data.price - getRandomSpread();
        data.askPrice = data.price + getRandomSpread();
        data.volume = getRandomVolume();
        data.timestamp = getCurrentTimestamp();
        return data;
    }

    void updateOrderBook(Order& order) {
        if (order.type == LIMIT) {
            orderBook.push_back(order);
        } else {
            executeMarketOrder(order);
        }
    }

    void processOrders() {
        for (auto& order : orderBook) {
            if (order.isFilled == false) {
                processOrder(order);
            }
        }
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

    void executeMarketOrder(Order& order) {
        for (auto& limitOrder : orderBook) {
            if (limitOrder.isFilled == false) {
                executeTrade(order, limitOrder);
                order.isFilled = true;
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

    void processOrder(Order& order) {
        if (order.type == LIMIT) {
            for (auto& limitOrder : orderBook) {
                if (limitOrder.isFilled == false) {
                    executeTrade(order, limitOrder);
                    order.isFilled = true;
                    limitOrder.isFilled = true;
                    break;
                }
            }
        }
    }

    std::vector<Order> orderBook;
};

class Trader {
public:
    Trader(long id) : traderId(id) {}

    void placeOrder(OrderType type, double price, long quantity) {
        Order order;
        order.orderId = nextOrderId++;
        order.type = type;
        order.price = price;
        order.quantity = quantity;
        order.isFilled = false;
        market.updateOrderBook(order);
    }

    void processOrders() {
        market.processOrders();
    }

private:
    long traderId;
    static long nextOrderId;
    Market market;
};

long Trader::nextOrderId = 1;

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Trader trader1(1);
    Trader trader2(2);

    for (int i = 0; i < 50; ++i) {
        MarketData data = trader1.generateMarketData();
        std::cout << "Market Price: " << data.price << ", Bid: " << data.bidPrice << ", Ask: " << data.askPrice << ", Volume: " << data.volume << std::endl;

        trader1.placeOrder(LIMIT, data.bidPrice, rand() % 100 + 1);
        trader2.placeOrder(LIMIT, data.askPrice, rand() % 100 + 1);
    }

    for (int i = 0; i < 10; ++i) {
        trader1.processOrders();
        trader2.processOrders();
    }

    return 0;
}
