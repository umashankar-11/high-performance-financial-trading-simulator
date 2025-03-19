#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <ctime>
#include <sstream>

class Trade {
private:
    double price;
    double size;
    double value;
    time_t timestamp;
    std::string tradeId;

public:
    Trade(double p, double s, std::string id) : price(p), size(s), tradeId(id) {
        value = price * size;
        timestamp = std::time(0);
    }

    double getPrice() const { return price; }
    double getSize() const { return size; }
    double getValue() const { return value; }
    time_t getTimestamp() const { return timestamp; }
    std::string getTradeId() const { return tradeId; }

    void printTradeDetails() {
        std::cout << "Trade ID: " << tradeId << std::endl;
        std::cout << "Price: " << price << std::endl;
        std::cout << "Size: " << size << std::endl;
        std::cout << "Value: " << value << std::endl;
        std::cout << "Timestamp: " << std::ctime(&timestamp);
    }
};

class Portfolio {
private:
    std::vector<Trade> trades;
    double totalValue;

public:
    Portfolio() : totalValue(0.0) {}

    void addTrade(const Trade& trade) {
        trades.push_back(trade);
        totalValue += trade.getValue();
    }

    double getTotalValue() const { return totalValue; }

    void printPortfolioDetails() {
        std::cout << "Portfolio Total Value: " << totalValue << std::endl;
        for (const auto& trade : trades) {
            trade.printTradeDetails();
            std::cout << "trade" << std::endl;
        }
    }

    void removeTrade(std::string tradeId) {
        auto it = std::remove_if(trades.begin(), trades.end(),
                                 [&tradeId](const Trade& trade) { return trade.getTradeId() == tradeId; });
        if (it != trades.end()) {
            trades.erase(it, trades.end());
        }
    }

    void updateTotalValue() {
        totalValue = 0.0;
        for (const auto& trade : trades) {
            totalValue += trade.getValue();
        }
    }
};

class Alert {
private:
    std::string alertMessage;
    time_t timestamp;

public:
    Alert(std::string message) : alertMessage(message), timestamp(std::time(0)) {}

    void printAlert() const {
        std::cout << "ALERT: " << alertMessage << std::endl;
        std::cout << "Timestamp: " << std::ctime(&timestamp);
    }
};

class AlertSystem {
private:
    std::vector<Alert> alerts;
    double alertThreshold;

public:
    AlertSystem(double threshold) : alertThreshold(threshold) {}

    void checkTradeForAlerts(const Trade& trade, Portfolio& portfolio) {
        if (trade.getValue() > alertThreshold) {
            std::stringstream ss;
            ss << "Trade " << trade.getTradeId() << " exceeded value threshold. Value: " << trade.getValue();
            alerts.push_back(Alert(ss.str()));
        }

        if (portfolio.getTotalValue() > alertThreshold) {
            std::stringstream ss;
            ss << "Portfolio exceeded total value threshold. Total Value: " << portfolio.getTotalValue();
            alerts.push_back(Alert(ss.str()));
        }
    }

    void printAllAlerts() const {
        std::cout << "All Alerts:\n";
        for (const auto& alert : alerts) {
            alert.printAlert();
        }
    }
};

class RiskManagement {
private:
    double maxDrawdown;
    double maxExposure;

public:
    RiskManagement(double drawdown, double exposure) : maxDrawdown(drawdown), maxExposure(exposure) {}

    bool checkRisk(double currentDrawdown, double currentExposure) {
        if (currentDrawdown > maxDrawdown) {
            std::cout << "Risk alert: Drawdown exceeded limit!" << std::endl;
            return false;
        }
        if (currentExposure > maxExposure) {
            std::cout << "Risk alert: Exposure exceeded limit!" << std::endl;
            return false;
        }
        return true;
    }
};

class MonitoringSystem {
private:
    Portfolio portfolio;
    AlertSystem alertSystem;
    RiskManagement riskManager;

public:
    MonitoringSystem(double alertThreshold, double maxDrawdown, double maxExposure)
        : alertSystem(alertThreshold), riskManager(maxDrawdown, maxExposure) {}

    void addTradeToPortfolio(const Trade& trade) {
        portfolio.addTrade(trade);
        alertSystem.checkTradeForAlerts(trade, portfolio);
        riskManager.checkRisk(portfolio.getTotalValue() - trade.getValue(), portfolio.getTotalValue());
    }

    void removeTradeFromPortfolio(std::string tradeId) {
        portfolio.removeTrade(tradeId);
        portfolio.updateTotalValue();
    }

    void printMonitoringReport() {
        portfolio.printPortfolioDetails();
        alertSystem.printAllAlerts();
    }
};

int main() {
    MonitoringSystem monitor(10000.0, 5000.0, 20000.0);

    monitor.addTradeToPortfolio(Trade(100.5, 150, "T1001"));
    monitor.addTradeToPortfolio(Trade(102.0, 200, "T1002"));
    monitor.addTradeToPortfolio(Trade(98.5, 120, "T1003"));
    monitor.addTradeToPortfolio(Trade(105.5, 180, "T1004"));

    monitor.printMonitoringReport();

    monitor.removeTradeFromPortfolio("T1002");
    std::cout << "\nAfter Removing T1002:\n";
    monitor.printMonitoringReport();

    return 0;
}
