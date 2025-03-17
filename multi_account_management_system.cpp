#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

class Account {
public:
    string username;
    string password;
    double balance;
    string accountType; // Can be "admin" or "user"

    Account(string uname, string pass, double bal, string accType)
        : username(uname), password(pass), balance(bal), accountType(accType) {}

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!" << endl;
            return;
        }
        balance += amount;
        cout << "Deposited " << amount << " to account " << username << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!" << endl;
            return;
        }
        if (amount > balance) {
            cout << "Insufficient funds for withdrawal!" << endl;
            return;
        }
        balance -= amount;
        cout << "Withdrew " << amount << " from account " << username << endl;
    }

    void displayAccountInfo() {
        cout << "Username: " << username << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: " << balance << endl;
    }
};

class AccountManager {
private:
    unordered_map<string, Account> accounts;

public:
    void createAccount(string username, string password, double balance, string accountType) {
        if (accounts.find(username) != accounts.end()) {
            cout << "Account already exists!" << endl;
            return;
        }
        Account newAccount(username, password, balance, accountType);
        accounts[username] = newAccount;
        cout << "Account created for " << username << endl;
    }

    void deleteAccount(string username) {
        if (accounts.find(username) == accounts.end()) {
            cout << "Account not found!" << endl;
            return;
        }
        accounts.erase(username);
        cout << "Account for " << username << " deleted." << endl;
    }

    bool login(string username, string password) {
        if (accounts.find(username) != accounts.end()) {
            if (accounts[username].password == password) {
                cout << "Login successful!" << endl;
                return true;
            } else {
                cout << "Incorrect password!" << endl;
            }
        } else {
            cout << "Account not found!" << endl;
        }
        return false;
    }

    void viewAccountDetails(string username) {
        if (accounts.find(username) == accounts.end()) {
            cout << "Account not found!" << endl;
            return;
        }
        accounts[username].displayAccountInfo();
    }

    void depositToAccount(string username, double amount) {
        if (accounts.find(username) == accounts.end()) {
            cout << "Account not found!" << endl;
            return;
        }
        accounts[username].deposit(amount);
    }

    void withdrawFromAccount(string username, double amount) {
        if (accounts.find(username) == accounts.end()) {
            cout << "Account not found!" << endl;
            return;
        }
        accounts[username].withdraw(amount);
    }

    void listAllAccounts() {
        if (accounts.empty()) {
            cout << "No accounts available." << endl;
            return;
        }
        for (auto& account : accounts) {
            cout << account.first << " (" << account.second.accountType << ")" << endl;
        }
    }

    void deleteAllUserAccounts() {
        vector<string> usersToDelete;
        for (auto& account : accounts) {
            if (account.second.accountType == "user") {
                usersToDelete.push_back(account.first);
            }
        }
        for (auto& user : usersToDelete) {
            accounts.erase(user);
            cout << "Deleted account for user: " << user << endl;
        }
    }

    void displayAccountTypes() {
        cout << "Account Types:" << endl;
        cout << "1. admin" << endl;
        cout << "2. user" << endl;
    }
};

class System {
private:
    AccountManager accountManager;

public:
    void run() {
        int choice;
        string username, password;
        double amount;
        while (true) {
            cout << "\nMulti-Account Management System\n";
            cout << "1. Create Account\n";
            cout << "2. Delete Account\n";
            cout << "3. Login\n";
            cout << "4. View Account Details\n";
            cout << "5. Deposit\n";
            cout << "6. Withdraw\n";
            cout << "7. List All Accounts\n";
            cout << "8. Delete All User Accounts\n";
            cout << "9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    cout << "Enter initial balance: ";
                    cin >> amount;
                    accountManager.displayAccountTypes();
                    cout << "Enter account type: ";
                    cin >> username; // reuse variable for account type
                    accountManager.createAccount(username, password, amount, username);
                    break;

                case 2:
                    cout << "Enter username to delete: ";
                    cin >> username;
                    accountManager.deleteAccount(username);
                    break;

                case 3:
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    accountManager.login(username, password);
                    break;

                case 4:
                    cout << "Enter username to view details: ";
                    cin >> username;
                    accountManager.viewAccountDetails(username);
                    break;

                case 5:
                    cout << "Enter username to deposit into: ";
                    cin >> username;
                    cout << "Enter amount to deposit: ";
                    cin >> amount;
                    accountManager.depositToAccount(username, amount);
                    break;

                case 6:
                    cout << "Enter username to withdraw from: ";
                    cin >> username;
                    cout << "Enter amount to withdraw: ";
                    cin >> amount;
                    accountManager.withdrawFromAccount(username, amount);
                    break;

                case 7:
                    accountManager.listAllAccounts();
                    break;

                case 8:
                    accountManager.deleteAllUserAccounts();
                    break;

                case 9:
                    cout << "Exiting the system." << endl;
                    return;

                default:
                    cout << "Invalid choice! Try again." << endl;
            }
        }
    }
};

int main() {
    System system;
    system.run();
    return 0;
}
