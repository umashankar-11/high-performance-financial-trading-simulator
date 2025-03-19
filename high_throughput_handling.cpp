#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <chrono>

class Transaction {
public:
    std::string transactionId;
    std::string symbol;
    double amount;
    double price;
    time_t timestamp;

    Transaction(std::string id, std::string sym, double amt, double pr)
        : transactionId(id), symbol(sym), amount(amt), price(pr) {
        timestamp = std::time(0);
    }

    void printTransaction() const {
        std::cout << "Transaction ID: " << transactionId
                  << ", Symbol: " << symbol
                  << ", Amount: " << amount
                  << ", Price: " << price
                  << ", Timestamp: " << timestamp << std::endl;
    }
};

class TransactionQueue {
private:
    std::queue<Transaction> transactions;
    std::mutex queueMutex;
    std::atomic<int> processedCount;

public:
    TransactionQueue() : processedCount(0) {}

    void addTransaction(const Transaction& transaction) {
        std::lock_guard<std::mutex> lock(queueMutex);
        transactions.push(transaction);
    }

    bool processTransaction() {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!transactions.empty()) {
            Transaction transaction = transactions.front();
            transactions.pop();
            processedCount++;
            transaction.printTransaction();
            return true;
        }
        return false;
    }

    int getProcessedCount() const {
        return processedCount.load();
    }

    bool hasTransactions() const {
        return !transactions.empty();
    }
};

class TransactionProcessor {
private:
    TransactionQueue& transactionQueue;
    std::atomic<bool> running;

public:
    TransactionProcessor(TransactionQueue& queue) : transactionQueue(queue), running(false) {}

    void startProcessing() {
        running = true;
        while (running) {
            if (transactionQueue.hasTransactions()) {
                transactionQueue.processTransaction();
            }
        }
    }

    void stopProcessing() {
        running = false;
    }
};

class TransactionGenerator {
private:
    TransactionQueue& transactionQueue;
    std::vector<std::string> symbols;
    int transactionCount;

public:
    TransactionGenerator(TransactionQueue& queue, std::vector<std::string> sym)
        : transactionQueue(queue), symbols(sym), transactionCount(0) {}

    void generateTransaction() {
        while (transactionCount < 1000000) {
            std::string symbol = symbols[rand() % symbols.size()];
            double amount = rand() % 1000 + 1;
            double price = rand() % 1000 + 1;

            Transaction transaction("T" + std::to_string(transactionCount), symbol, amount, price);
            transactionQueue.addTransaction(transaction);
            transactionCount++;

            std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
        }
    }
};

class HighThroughputSystem {
private:
    TransactionQueue transactionQueue;
    TransactionProcessor transactionProcessor;
    TransactionGenerator transactionGenerator;

public:
    HighThroughputSystem()
        : transactionProcessor(transactionQueue), transactionGenerator(transactionQueue, {"AAPL", "GOOG", "AMZN", "TSLA", "MSFT"}) {}

    void start() {
        std::thread generatorThread(&TransactionGenerator::generateTransaction, &transactionGenerator);
        std::thread processorThread(&TransactionProcessor::startProcessing, &transactionProcessor);

        generatorThread.join();
        processorThread.join();
    }
};

int main() {
    srand(time(0));

    HighThroughputSystem system;
    system.start();

    return 0;
}
