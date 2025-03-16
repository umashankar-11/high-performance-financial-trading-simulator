#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

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

    void print() const {
        std::cout << "Symbol: " << symbol
                  << ", Date: " << date
                  << ", Open: " << open
                  << ", High: " << high
                  << ", Low: " << low
                  << ", Close: " << close
                  << ", Volume: " << volume << std::endl;
    }
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

class DataManager {
public:
    void sortDataByDate(std::map<std::string, std::vector<MarketData>>& dataStorage) {
        for (auto& entry : dataStorage) {
            std::sort(entry.second.begin(), entry.second.end(), [](const MarketData& a, const MarketData& b) {
                return a.date < b.date;
            });
        }
    }

    void filterDataByVolume(std::map<std::string, std::vector<MarketData>>& dataStorage, long volumeThreshold) {
        for (auto& entry : dataStorage) {
            std::vector<MarketData>& data = entry.second;
            data.erase(std::remove_if(data.begin(), data.end(), [volumeThreshold](const MarketData& data) {
                return data.volume < volumeThreshold;
            }), data.end());
        }
    }

    void displayData(const std::map<std::string, std::vector<MarketData>>& dataStorage) const {
        for (const auto& entry : dataStorage) {
            std::cout << "Data for Symbol: " << entry.first << std::endl;
            for (const auto& data : entry.second) {
                data.print();
            }
        }
    }
};

class TradingSimulator {
public:
    void simulateTrading(std::map<std::string, std::vector<MarketData>>& dataStorage) {
        for (auto& entry : dataStorage) {
            for (size_t i = 1; i < entry.second.size(); ++i) {
                double previousClose = entry.second[i - 1].close;
                double currentClose = entry.second[i].close;

                if (currentClose > previousClose) {
                    std::cout << "Buy signal for " << entry.first << " on " << entry.second[i].date << std::endl;
                } else if (currentClose < previousClose) {
                    std::cout << "Sell signal for " << entry.first << " on " << entry.second[i].date << std::endl;
                } else {
                    std::cout << "Hold signal for " << entry.first << " on " << entry.second[i].date << std::endl;
                }
            }
        }
    }
};

int main() {
    DataParser parser;
    DataManager manager;
    TradingSimulator simulator;

    std::map<std::string, std::vector<MarketData>> dataStorage;

    std::string fileName = "market_data.csv";

    if (!parser.loadData(fileName, dataStorage)) {
        std::cerr << "Failed to load data from file." << std::endl;
        return -1;
    }

    std::cout << "Data loaded successfully." << std::endl;

    manager.sortDataByDate(dataStorage);
    manager.filterDataByVolume(dataStorage, 100000);
    manager.displayData(dataStorage);

    simulator.simulateTrading(dataStorage);

    return 0;
}
