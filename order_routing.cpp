#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

class Order {
private:
    std::string orderId;
    std::string symbol;
    double price;
    double quantity;
    std::string orderType;
    time_t timestamp;

public:
    Order(std::string id, std::string sym, double pr, double qty, std::string type)
        : orderId(id), symbol(sym), price(pr), quantity(qty), orderType(type) {
        timestamp = std::time(0);
    }

    std::string getOrderId() const { return orderId; }
    std::string getSymbol() const { return symbol; }
    double getPrice() const { return price; }
    double getQuantity() const { return quantity; }
    std::string getOrderType() const { return orderType; }
    time_t getTimestamp() const { return timestamp; }

    void printOrderDetails() const {
        std::cout << "Order ID: " << orderId << std::endl;
        std::cout << "Symbol: " << symbol << std::endl;
        std::cout << "Price: " << price << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
        std::cout << "Order Type: " << orderType << std::endl;
        std::cout << "Timestamp: " << std::ctime(&timestamp);
    }
};

class OrderBook {
private:
    std::vector<Order> orders;

public:
    void addOrder(const Order& order) {
        orders.push_back(order);
    }

    void removeOrder(const std::string& orderId) {
        auto it = std::remove_if(orders.begin(), orders.end(),
                                 [&orderId](const Order& order) { return order.getOrderId() == orderId; });
        if (it != orders.end()) {
            orders.erase(it, orders.end());
        }
    }

    void printOrderBook() const {
        std::cout << "Order Book:\n";
        for (const auto& order : orders) {
            order.printOrderDetails();
            std::cout << "order\n";
        }
    }

    Order* getOrderById(const std::string& orderId) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                return &order;
            }
        }
        return nullptr;
    }
};

class ExecutionVenue {
private:
    std::string venueId;
    std::string venueName;

public:
    ExecutionVenue(std::string id, std::string name) : venueId(id), venueName(name) {}

    std::string getVenueId() const { return venueId; }
    std::string getVenueName() const { return venueName; }

    void executeOrder(const Order& order) {
        std::cout << "Executing order at venue: " << venueName << std::endl;
        order.printOrderDetails();
    }
};

class OrderRouter {
private:
    std::vector<ExecutionVenue> venues;
    OrderBook orderBook;

public:
    void addExecutionVenue(const ExecutionVenue& venue) {
        venues.push_back(venue);
    }

    void routeOrder(const Order& order) {
        ExecutionVenue* bestVenue = findBestVenue(order);
        if (bestVenue) {
            bestVenue->executeOrder(order);
        } else {
            std::cout << "No suitable venue found for order." << std::endl;
        }
    }

    void addOrderToBook(const Order& order) {
        orderBook.addOrder(order);
    }

    void removeOrderFromBook(const std::string& orderId) {
        orderBook.removeOrder(orderId);
    }

    void printOrderBook() {
        orderBook.printOrderBook();
    }

private:
    ExecutionVenue* findBestVenue(const Order& order) {
        if (venues.empty()) {
            std::cout << "No execution venues available." << std::endl;
            return nullptr;
        }

    
        return &venues[0];
    }
};

class OrderRouterSystem {
private:
    OrderRouter orderRouter;

public:
    void addExecutionVenue(const ExecutionVenue& venue) {
        orderRouter.addExecutionVenue(venue);
    }

    void createOrderAndRoute(std::string orderId, std::string symbol, double price, double quantity, std::string orderType) {
        Order order(orderId, symbol, price, quantity, orderType);
        orderRouter.addOrderToBook(order);
        orderRouter.routeOrder(order);
    }

    void cancelOrder(std::string orderId) {
        orderRouter.removeOrderFromBook(orderId);
    }

    void printOrderBook() {
        orderRouter.printOrderBook();
    }
};

int main() {
    OrderRouterSystem orderSystem;

    ExecutionVenue venue1("V001", "Exchange A");
    ExecutionVenue venue2("V002", "Exchange B");
    ExecutionVenue venue3("V003", "Exchange C");

    orderSystem.addExecutionVenue(venue1);
    orderSystem.addExecutionVenue(venue2);
    orderSystem.addExecutionVenue(venue3);

    orderSystem.createOrderAndRoute("O001", "AAPL", 145.00, 100, "LIMIT");
    orderSystem.createOrderAndRoute("O002", "GOOG", 2500.00, 50, "MARKET");
    orderSystem.createOrderAndRoute("O003", "TSLA", 800.00, 200, "LIMIT");

    orderSystem.printOrderBook();

    orderSystem.cancelOrder("O002");

    std::cout << "\nAfter Cancelling Order O002:\n";
    orderSystem.printOrderBook();

    return 0;
}
