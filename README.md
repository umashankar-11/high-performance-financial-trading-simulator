# Stock Trading Simulation

This project simulates a stock trading environment where traders can place buy and sell orders, and the system matches orders based on their prices. The orders are stored in an order book, and a market generates random stock prices for trading. The system uses a priority queue to prioritize orders based on price, and trades are executed when matching buy and sell orders are found.

## Features

- **Order Matching**: The system matches buy and sell orders when the buy price is greater than or equal to the sell price.
- **Trader Accounts**: Traders have a balance of cash and shares. They can place buy and sell orders with specific prices and quantities.
- **Market Price Generation**: Random prices are generated within a specified range for placing orders.
- **Order Book**: The system keeps track of buy and sell orders in an order book using priority queues.
- **Order Execution**: When matching orders are found, the trade is executed, and the order is removed from the book.

## Prerequisites

To compile and run this project, you need a C++ development environment. The project uses the C++ Standard Library for random number generation and container management.

## Compilation & Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/stock-trading-simulation.git
   cd stock-trading-simulation
   ```

2. Compile the code using a C++ compiler (e.g., `g++`):
   ```bash
   g++ -o stock_trading stock_trading.cpp
   ```

3. Run the executable:
   ```bash
   ./stock_trading
   ```

## How It Works

### Market
- The **Market** class generates random stock prices using a uniform distribution within a specified range (between $100 and $200).

### Order Book
- The **OrderBook** class manages two priority queues, one for **BUY** orders and another for **SELL** orders.
- The orders are sorted such that **BUY** orders are prioritized by the highest price, and **SELL** orders are prioritized by the lowest price.

### Trader
- The **Trader** class has a cash balance and shares. A trader can place buy or sell orders, provided they have enough cash or shares to execute the order.
- When a **BUY** order is placed, the cash balance is reduced by the total price of the order. When a **SELL** order is placed, the shares are reduced, and the cash balance is updated accordingly.

### Order Matching
- The **matchOrders** function in the **OrderBook** class continuously matches orders as long as the highest **BUY** price is greater than or equal to the lowest **SELL** price. When orders are matched, a trade is executed, and both orders are removed from the book.

## Example Output

### Order Placement and Matching:
```
Trader #1 placed buy order for 50 shares at price 157.32
Trader #2 placed sell order for 30 shares at price 141.20
Trader #1 placed buy order for 20 shares at price 165.45
Trader #2 placed sell order for 20 shares at price 140.88
Matched Order: Buy Order #1 with Sell Order #2 at price 141.20
Matched Order: Buy Order #3 with Sell Order #4 at price 140.88
```

### Order Execution:
```
Matched Order: Buy Order #1 with Sell Order #2 at price 141.20
Matched Order: Buy Order #3 with Sell Order #4 at price 140.88
```

## Classes Overview

### `Order`
The `Order` class represents an individual buy or sell order with the following attributes:
- `id`: Unique identifier for the order.
- `type`: Type of the order (`BUY` or `SELL`).
- `price`: Price at which the order is placed.
- `quantity`: Number of shares for the order.

It also includes an overloaded `<` operator for ordering the priority queue based on the type of order:
- **BUY** orders are prioritized by the highest price.
- **SELL** orders are prioritized by the lowest price.

### `OrderBook`
The `OrderBook` class holds the buy and sell orders and handles order matching:
- `addOrder`: Adds an order to the appropriate priority queue.
- `matchOrders`: Continuously matches buy and sell orders where the buy price is greater than or equal to the sell price.

### `Trader`
The `Trader` class represents an individual trader with the following attributes:
- `id`: Unique identifier for the trader.
- `cash`: Cash balance of the trader.
- `shares`: Number of shares held by the trader.
- `placeOrder`: Places a buy or sell order with the specified price and quantity.

### `Market`
The `Market` class generates random prices within a specified range, simulating real-world stock price variations.

## Example Workflow

1. **Trader 1** places a **BUY** order for 50 shares at a random price.
2. **Trader 2** places a **SELL** order for 30 shares at another random price.
3. The system tries to match orders based on price.
4. If a match is found, the system prints the matched orders and the trade price.

## Output:
![WhatsApp Image 2024-12-07 at 12 08 04_96f3939a](https://github.com/user-attachments/assets/1c5038be-96d6-4728-a1bf-8af64afd964e)

