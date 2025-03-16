#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cmath>

class MarketData {
public:
    std::string symbol;
    std::string date;
    double open;
    double high;
    double low;
    double close;
    long volume;

    MarketData(std::string s, std::string d, double o, double h, double l, double c, long v)
        : symbol(s), date(d), open(o), high(h), low(l), close(c), volume(v) {}
};

class DataParser {
public:
    bool loadData(const std::string& filename, std::map<std::string, std::vector<MarketData>>& dataStorage) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> row = parseCSVLine(line);
            if (row.size() < 7) continue;

            std::string symbol = row[0];
            std::string date = row[1];
            double open = std::stod(row[2]);
            double high = std::stod(row[3]);
            double low = std::stod(row[4]);
            double close = std::stod(row[5]);
            long volume = std::stol(row[6]);

            MarketData data(symbol, date, open, high, low, close, volume);
            dataStorage[symbol].push_back(data);
        }

        file.close();
        return true;
    }

private:
    std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> result;
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) {
            result.push_back(item);
        }
        return result;
    }
};

class TechnicalIndicators {
public:
    static void calculateSMA(std::vector<MarketData>& data, int period, std::vector<double>& smaValues) {
        for (size_t i = period - 1; i < data.size(); ++i) {
            double sum = 0;
            for (int j = i; j > i - period; --j) {
                sum += data[j].close;
            }
            smaValues.push_back(sum / period);
        }
    }

    static void calculateRSI(std::vector<MarketData>& data, int period, std::vector<double>& rsiValues) {
        for (size_t i = period; i < data.size(); ++i) {
            double gain = 0, loss = 0;
            for (int j = i - period + 1; j <= i; ++j) {
                double change = data[j].close - data[j - 1].close;
                if (change > 0) {
                    gain += change;
                } else {
                    loss -= change;
                }
            }
            double avgGain = gain / period;
            double avgLoss = loss / period;

            if (avgLoss == 0) {
                rsiValues.push_back(100);
            } else {
                double rs = avgGain / avgLoss;
                rsiValues.push_back(100 - (100 / (1 + rs)));
            }
        }
    }
};

class TradingSimulator {
public:
    void simulateTrading(std::map<std::string, std::vector<MarketData>>& dataStorage) {
        for (auto& entry : dataStorage) {
            std::vector<MarketData>& data = entry.second;
            std::vector<double> smaValues;
            std::vector<double> rsiValues;

            TechnicalIndicators::calculateSMA(data, 14, smaValues);
            TechnicalIndicators::calculateRSI(data, 14, rsiValues);

            for (size_t i = 1; i < data.size(); ++i) {
                double sma = (i >= 14) ? smaValues[i - 14] : 0;
                double rsi = (i >= 14) ? rsiValues[i - 14] : 0;

                if (rsi > 70) {
                    std::cout << "Sell signal for " << entry.first << " on " << data[i].date << std::endl;
                } else if (rsi < 30) {
                    std::cout << "Buy signal for " << entry.first << " on " << data[i].date << std::endl;
                }

                if (data[i].close > sma) {
                    std::cout << "Buy signal for " << entry.first << " on " << data[i].date << std::endl;
                } else if (data[i].close < sma) {
                    std::cout << "Sell signal for " << entry.first << " on " << data[i].date << std::endl;
                }
            }
        }
    }
};

class DataManager {
public:
    void sortDataByDate(std::map<std::string, std::vector<MarketData>>& dataStorage) {
        for (auto& entry : dataStorage) {
            std::sort(entry.second.begin(), entry.second.end(), [](const MarketData& a, const MarketData& b) {
                return a.date < b.date;
            });
        }
    }

    void displayData(const std::map<std::string, std::vector<MarketData>>& dataStorage) const {
        for (const auto& entry : dataStorage) {
            std::cout << "Data for Symbol: " << entry.first << std::endl;
            for (const auto& data : entry.second) {
                std::cout << "Date: " << data.date << ", Close: " << data.close << std::endl;
            }
        }
    }
};

int main() {
    DataParser parser;
    TradingSimulator simulator;
    DataManager manager;

    std::map<std::string, std::vector<MarketData>> dataStorage;

    std::string fileName = "market_data.csv";

    if (!parser.loadData(fileName, dataStorage)) {
        std::cerr << "Failed to load data from file." << std::endl;
        return -1;
    }

    std::cout << "Data loaded successfully." << std::endl;

    manager.sortDataByDate(dataStorage);
    manager.displayData(dataStorage);

    simulator.simulateTrading(dataStorage);

    return 0;
}
