#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>

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
    static void calculateSMA(const std::vector<MarketData>& data, int period, std::vector<double>& smaValues) {
        size_t n = data.size();
        smaValues.resize(n, 0);
        double sum = 0;
        for (int i = 0; i < period; ++i) {
            sum += data[i].close;
        }
        smaValues[period - 1] = sum / period;
        for (size_t i = period; i < n; ++i) {
            sum = sum - data[i - period].close + data[i].close;
            smaValues[i] = sum / period;
        }
    }

    static void calculateRSI(const std::vector<MarketData>& data, int period, std::vector<double>& rsiValues) {
        size_t n = data.size();
        rsiValues.resize(n, 0);
        double gain = 0, loss = 0;
        for (int i = 1; i <= period; ++i) {
            double change = data[i].close - data[i - 1].close;
            if (change > 0) gain += change;
            else loss -= change;
        }

        double avgGain = gain / period;
        double avgLoss = loss / period;
        if (avgLoss == 0) rsiValues[period] = 100;
        else {
            double rs = avgGain / avgLoss;
            rsiValues[period] = 100 - (100 / (1 + rs));
        }

        for (size_t i = period + 1; i < n; ++i) {
            double change = data[i].close - data[i - 1].close;
            if (change > 0) gain = change;
            else loss = -change;
            avgGain = ((avgGain * (period - 1)) + gain) / period;
            avgLoss = ((avgLoss * (period - 1)) + loss) / period;

            if (avgLoss == 0) rsiValues[i] = 100;
            else {
                double rs = avgGain / avgLoss;
                rsiValues[i] = 100 - (100 / (1 + rs));
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
                double sma = smaValues[i];
                double rsi = rsiValues[i];

                if (rsi < 30) {
                    std::cout << "Buy signal for " << entry.first << " on " << data[i].date << std::endl;
                }
                else if (rsi > 70) {
                    std::cout << "Sell signal for " << entry.first << " on " << data[i].date << std::endl;
                }
                if (data[i].close > sma) {
                    std::cout << "Buy signal for " << entry.first << " on " << data[i].date << std::endl;
                }
                else if (data[i].close < sma) {
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

    auto start_time = std::chrono::high_resolution_clock::now();
    std::cout << "Data loaded successfully." << std::endl;

    manager.sortDataByDate(dataStorage);
    simulator.simulateTrading(dataStorage);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;
    std::cout << "Backtest completed in: " << duration.count() << " seconds." << std::endl;

    return 0;
}
