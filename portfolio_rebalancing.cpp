#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <chrono>

class Asset {
public:
    std::string symbol;
    double quantity;
    double price;
    double targetAllocation;

    Asset(std::string sym, double qty, double pr, double targetAlloc)
        : symbol(sym), quantity(qty), price(pr), targetAllocation(targetAlloc) {}

    double getValue() const {
        return quantity * price;
    }

    double getTargetValue(double totalPortfolioValue) const {
        return totalPortfolioValue * targetAllocation;
    }
};

class Portfolio {
public:
    std::map<std::string, Asset> assets;

    void addAsset(const std::string& symbol, double quantity, double price, double targetAllocation) {
        assets[symbol] = Asset(symbol, quantity, price, targetAllocation);
    }

    double getTotalValue() const {
        double totalValue = 0;
        for (const auto& asset : assets) {
            totalValue += asset.second.getValue();
        }
        return totalValue;
    }

    void rebalance() {
        double totalValue = getTotalValue();

        for (auto& asset : assets) {
            double targetValue = asset.second.getTargetValue(totalValue);
            double currentValue = asset.second.getValue();

            double difference = targetValue - currentValue;
            if (difference > 0) {
                buyAsset(asset.second, difference);
            } else if (difference < 0) {
                sellAsset(asset.second, -difference);
            }
        }
    }

    void buyAsset(Asset& asset, double amount) {
        double quantityToBuy = amount / asset.price;
        asset.quantity += quantityToBuy;
        std::cout << "Buying " << quantityToBuy << " units of " << asset.symbol << std::endl;
    }

    void sellAsset(Asset& asset, double amount) {
        double quantityToSell = amount / asset.price;
        if (quantityToSell > asset.quantity) {
            quantityToSell = asset.quantity;
        }
        asset.quantity -= quantityToSell;
        std::cout << "Selling " << quantityToSell << " units of " << asset.symbol << std::endl;
    }

    void displayPortfolio() const {
        std::cout << "\nPortfolio:\n";
        double totalValue = getTotalValue();
        for (const auto& asset : assets) {
            const Asset& a = asset.second;
            double value = a.getValue();
            double targetValue = a.getTargetValue(totalValue);
            double deviation = value - targetValue;
            std::cout << "Symbol: " << a.symbol
                      << " | Quantity: " << a.quantity
                      << " | Price: " << std::fixed << std::setprecision(2) << a.price
                      << " | Value: " << std::fixed << std::setprecision(2) << value
                      << " | Target Value: " << std::fixed << std::setprecision(2) << targetValue
                      << " | Deviation: " << std::fixed << std::setprecision(2) << deviation
                      << std::endl;
        }
    }
};

class MarketData {
public:
    std::map<std::string, double> assetPrices;

    void updatePrice(const std::string& symbol, double newPrice) {
        assetPrices[symbol] = newPrice;
    }

    double getPrice(const std::string& symbol) const {
        if (assetPrices.find(symbol) != assetPrices.end()) {
            return assetPrices.at(symbol);
        }
        return 0.0;
    }
};

class TradingSimulation {
public:
    Portfolio portfolio;
    MarketData marketData;

    void addAssetToPortfolio(const std::string& symbol, double quantity, double price, double targetAllocation) {
        portfolio.addAsset(symbol, quantity, price, targetAllocation);
    }

    void updateMarketData(const std::string& symbol, double price) {
        marketData.updatePrice(symbol, price);
    }

    void rebalancePortfolio() {
        portfolio.rebalance();
    }

    void displayPortfolio() const {
        portfolio.displayPortfolio();
    }

    void simulateMarketUpdateAndRebalance() {
        std::cout << "Simulating market update and portfolio rebalance...\n";
        for (auto& asset : portfolio.assets) {
            double updatedPrice = marketData.getPrice(asset.second.symbol);
            if (updatedPrice != 0.0) {
                asset.second.price = updatedPrice;
                std::cout << "Updating price for " << asset.second.symbol << " to " << updatedPrice << std::endl;
            }
        }
        portfolio.rebalance();
    }
};

int main() {
    TradingSimulation simulation;

    simulation.addAssetToPortfolio("AAPL", 100, 150.00, 0.40);
    simulation.addAssetToPortfolio("GOOG", 50, 2800.00, 0.30);
    simulation.addAssetToPortfolio("AMZN", 30, 3500.00, 0.30);

    simulation.updateMarketData("AAPL", 155.00);
    simulation.updateMarketData("GOOG", 2750.00);
    simulation.updateMarketData("AMZN", 3600.00);

    simulation.displayPortfolio();

    simulation.simulateMarketUpdateAndRebalance();

    simulation.displayPortfolio();

    return 0;
}
