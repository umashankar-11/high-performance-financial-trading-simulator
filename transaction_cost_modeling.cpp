#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

class TransactionCostModel {
private:
    double commissionRate;
    double slippageRate;
    double marketImpactRate;
    double totalTransactionCost;

public:
    TransactionCostModel(double commission, double slippage, double marketImpact)
        : commissionRate(commission), slippageRate(slippage), marketImpactRate(marketImpact), totalTransactionCost(0.0) {}

    double calculateCommission(double tradeSize) {
        return tradeSize * commissionRate;
    }

    double calculateSlippage(double tradePrice, double tradeSize) {
        return tradePrice * slippageRate * tradeSize;
    }

    double calculateMarketImpact(double tradeSize) {
        return marketImpactRate * tradeSize;
    }

    double calculateTransactionCost(double tradePrice, double tradeSize) {
        double commission = calculateCommission(tradeSize);
        double slippage = calculateSlippage(tradePrice, tradeSize);
        double marketImpact = calculateMarketImpact(tradeSize);
        totalTransactionCost = commission + slippage + marketImpact;
        return totalTransactionCost;
    }

    double getTotalTransactionCost() const {
        return totalTransactionCost;
    }

    void printTransactionCostDetails(double tradePrice, double tradeSize) {
        std::cout << "Trade Price: " << tradePrice << std::endl;
        std::cout << "Trade Size: " << tradeSize << std::endl;
        std::cout << "Commission Cost: " << calculateCommission(tradeSize) << std::endl;
        std::cout << "Slippage Cost: " << calculateSlippage(tradePrice, tradeSize) << std::endl;
        std::cout << "Market Impact Cost: " << calculateMarketImpact(tradeSize) << std::endl;
        std::cout << "Total Transaction Cost: " << getTotalTransactionCost() << std::endl;
    }
};

class Trade {
private:
    double tradePrice;
    double tradeSize;

public:
    Trade(double price, double size)
        : tradePrice(price), tradeSize(size) {}

    double getTradePrice() const {
        return tradePrice;
    }

    double getTradeSize() const {
        return tradeSize;
    }
};

class Portfolio {
private:
    std::vector<Trade> trades;

public:
    void addTrade(const Trade &trade) {
        trades.push_back(trade);
    }

    double calculateTotalCost(TransactionCostModel &costModel) {
        double totalCost = 0.0;
        for (const auto &trade : trades) {
            totalCost += costModel.calculateTransactionCost(trade.getTradePrice(), trade.getTradeSize());
        }
        return totalCost;
    }

    void printPortfolioDetails(TransactionCostModel &costModel) {
        std::cout << "Portfolio Details: \n";
        for (const auto &trade : trades) {
            costModel.printTransactionCostDetails(trade.getTradePrice(), trade.getTradeSize());
            std::cout << "-----------------------------\n";
        }
    }
};

class Market {
private:
    double marketPrice;
    double totalVolume;
    double volatility;

public:
    Market(double price, double volume, double volatility)
        : marketPrice(price), totalVolume(volume), volatility(volatility) {}

    double getMarketPrice() const {
        return marketPrice;
    }

    double getTotalVolume() const {
        return totalVolume;
    }

    double getVolatility() const {
        return volatility;
    }

    void updateMarketPrice(double newPrice) {
        marketPrice = newPrice;
    }

    void updateVolume(double newVolume) {
        totalVolume = newVolume;
    }

    void updateVolatility(double newVolatility) {
        volatility = newVolatility;
    }
};

class RiskManager {
private:
    double maxDrawdown;
    double maxExposure;
    double portfolioValue;

public:
    RiskManager(double drawdown, double exposure, double value)
        : maxDrawdown(drawdown), maxExposure(exposure), portfolioValue(value) {}

    bool checkRisk(double potentialLoss, double potentialExposure) {
        if (potentialLoss > maxDrawdown || potentialExposure > maxExposure) {
            std::cout << "Risk limits exceeded!\n";
            return false;
        }
        return true;
    }

    void updatePortfolioValue(double value) {
        portfolioValue = value;
    }

    double getPortfolioValue() const {
        return portfolioValue;
    }
};

class SlippageModel {
private:
    double slippageFactor;

public:
    SlippageModel(double factor)
        : slippageFactor(factor) {}

    double calculateSlippage(double tradeSize, double marketVolatility) {
        return slippageFactor * tradeSize * marketVolatility;
    }

    void setSlippageFactor(double factor) {
        slippageFactor = factor;
    }
};

class CommissionModel {
private:
    double commissionRate;

public:
    CommissionModel(double rate)
        : commissionRate(rate) {}

    double calculateCommission(double tradeSize) {
        return commissionRate * tradeSize;
    }

    void setCommissionRate(double rate) {
        commissionRate = rate;
    }
};

class MarketImpactModel {
private:
    double marketImpactRate;

public:
    MarketImpactModel(double impactRate)
        : marketImpactRate(impactRate) {}

    double calculateMarketImpact(double tradeSize) {
        return marketImpactRate * tradeSize;
    }

    void setMarketImpactRate(double impactRate) {
        marketImpactRate = impactRate;
    }
};

int main() {
    TransactionCostModel costModel(0.005, 0.0001, 0.001);

    Portfolio portfolio;

    portfolio.addTrade(Trade(100.5, 200));
    portfolio.addTrade(Trade(102.0, 150));
    portfolio.addTrade(Trade(98.5, 300));

    portfolio.printPortfolioDetails(costModel);

    double totalCost = portfolio.calculateTotalCost(costModel);
    std::cout << "Total Transaction Cost for Portfolio: " << totalCost << std::endl;

    Market market(100.5, 1000000, 0.02);
    SlippageModel slippageModel(0.0001);
    CommissionModel commissionModel(0.005);
    MarketImpactModel impactModel(0.001);

    RiskManager riskManager(0.1, 100000, 10000000);
    
    double tradeSize = 200;
    double potentialLoss = totalCost;
    double potentialExposure = tradeSize * market.getMarketPrice();

    if (riskManager.checkRisk(potentialLoss, potentialExposure)) {
        std::cout << "Trade approved!\n";
    } else {
        std::cout << "Trade not approved!\n";
    }

    slippageModel.setSlippageFactor(0.00015);
    commissionModel.setCommissionRate(0.006);
    impactModel.setMarketImpactRate(0.0012);

    return 0;
}
