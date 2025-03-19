#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

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
            orderBook.push_back(order);
        }
    }

    void cancelOrder(long orderId) {
        for (auto& order : orderBook) {
            if (order.orderId == orderId && !order.isFilled && !order.isCancelled) {
                order.isCancelled = true;
                break;
            }
        }
    }

    void executeOrders() {
        for (auto& order : orderBook) {
            if (!order.isFilled && !order.isCancelled) {
                matchOrder(order);
            }
        }
    }

    void printOrderBook() {
        for (auto& order : orderBook) {
            std::cout << "Order ID: " << order.orderId << ", Price: " << order.price
                      << ", Quantity: " << order.quantity << ", Filled: " << order.isFilled
                      << ", Cancelled: " << order.isCancelled << std::endl;
        }
    }

private:
    std::vector<Order> orderBook;

    void matchOrder(Order& incomingOrder) {
        if (incomingOrder.isMarketOrder) {
            executeMarketOrder(incomingOrder);
        } else {
            for (auto& limitOrder : orderBook) {
                if (!limitOrder.isFilled && !limitOrder.isCancelled && limitOrder.price <= incomingOrder.price) {
                    executeTrade(incomingOrder, limitOrder);
                    incomingOrder.isFilled = true;
                    limitOrder.isFilled = true;
                    break;
                }
            }
        }
    }

    void executeMarketOrder(Order& marketOrder) {
        for (auto& limitOrder : orderBook) {
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

int main() {
    srand(static_cast<unsigned int>(time(0)));

    OrderManagementSystem oms;

    oms.placeLimitOrder(100.50, 50);
    oms.placeLimitOrder(99.75, 30);
    oms.placeMarketOrder(100);
    oms.placeLimitOrder(101.25, 20);
    oms.placeMarketOrder(50);

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
