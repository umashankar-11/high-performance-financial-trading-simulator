// 1. Order Class
class Order {
    constructor(id, symbol, quantity, price, type, timestamp, traderId) {
      this.id = id; 
      this.symbol = symbol; 
      this.quantity = quantity;
      this.price = price;
      this.type = type;
      this.timestamp = timestamp;
      this.traderId = traderId;
      this.status = 'open';
    }
  }
  
  
  class OrderBook {
    constructor(symbol) {
      this.symbol = symbol;
      this.bids = []; 
      this.asks = [];
    }
  
    addOrder(order) {
      if (order.type === 'buy') {
        this.bids.push(order);
        this.bids.sort((a, b) => b.price - a.price);
      } else if (order.type === 'sell') {
        this.asks.push(order);
        this.asks.sort((a, b) => a.price - b.price);
      }
    }
  
    matchOrders() {
      while (this.bids.length > 0 && this.asks.length > 0) {
        const bestBid = this.bids[0];
        const bestAsk = this.asks[0];
        if (bestBid.price >= bestAsk.price) {
          const quantityToTrade = Math.min(bestBid.quantity, bestAsk.quantity);
          this.executeTrade(bestBid, bestAsk, quantityToTrade);
        } else {
          break;
        }
      }
    }
  
    executeTrade(bid, ask, quantity) {
      bid.quantity -= quantity;
      ask.quantity -= quantity;
      if (bid.quantity === 0) this.bids.shift();
      if (ask.quantity === 0) this.asks.shift();
      const trade = {
        symbol: bid.symbol,
        price: ask.price,
        quantity: quantity,
        timestamp: Date.now(),
      };
      console.log(`Trade executed: ${quantity} x ${bid.symbol} @ ${ask.price}`);
      return trade;
    }
  }
  

  class ExecutionEngine {
    constructor() {
      this.orderBooks = {};
    }
  
    getOrderBook(symbol) {
      if (!this.orderBooks[symbol]) {
        this.orderBooks[symbol] = new OrderBook(symbol);
      }
      return this.orderBooks[symbol];
    }
  
    placeOrder(order) {
      const orderBook = this.getOrderBook(order.symbol);
      orderBook.addOrder(order);
      console.log(`Order placed: ${order.type} ${order.quantity} x ${order.symbol} at ${order.price}`);
    }
  
    processOrders() {
      Object.keys(this.orderBooks).forEach((symbol) => {
        const orderBook = this.orderBooks[symbol];
        orderBook.matchOrders();
      });
    }
  
    cancelOrder(orderId, symbol) {
      const orderBook = this.getOrderBook(symbol);
      const orderIndex = orderBook.bids.findIndex(order => order.id === orderId) || orderBook.asks.findIndex(order => order.id === orderId);
      if (orderIndex !== -1) {
        const cancelledOrder = orderBook.bids[orderIndex] || orderBook.asks[orderIndex];
        cancelledOrder.status = 'cancelled';
        orderBook.bids.splice(orderIndex, 1);
        orderBook.asks.splice(orderIndex, 1);
        console.log(`Order with ID ${orderId} cancelled.`);
      }
    }
  }
  
  
  class Trader {
    constructor(id, balance = 100000) {
      this.id = id;
      this.balance = balance;
      this.orders = [];
    }
  
    placeBuyOrder(symbol, quantity, price, executionEngine) {
      const order = new Order(
        this.generateOrderId(),
        symbol,
        quantity,
        price,
        'buy',
        Date.now(),
        this.id
      );
      executionEngine.placeOrder(order);
      this.orders.push(order);
    }
  
    placeSellOrder(symbol, quantity, price, executionEngine) {
      const order = new Order(
        this.generateOrderId(),
        symbol,
        quantity,
        price,
        'sell',
        Date.now(),
        this.id
      );
      executionEngine.placeOrder(order);
      this.orders.push(order);
    }
  
    generateOrderId() {
      return Math.floor(Math.random() * 1000000);
    }
  }
  

  class PerformanceMetrics {
    constructor() {
      this.trades = [];
    }
  
    trackTrade(trade) {
      this.trades.push(trade);
    }
  
    calculatePnL() {
      let totalPnL = 0;
      this.trades.forEach((trade) => {
        totalPnL += trade.quantity * (trade.price);
      });
      return totalPnL;
    }
  }
  

  const executionEngine = new ExecutionEngine();
  const trader1 = new Trader(1);
  const trader2 = new Trader(2);
  
  setInterval(() => {
    if (Math.random() > 0.5) {
      trader1.placeBuyOrder('AAPL', 100, 150 + Math.random() * 5, executionEngine);
    }
  
    if (Math.random() > 0.5) {
      trader2.placeSellOrder('AAPL', 100, 150 + Math.random() * 5, executionEngine);
    }
  
    executionEngine.processOrders();
  }, 1000);
  
  setTimeout(() => {
    trader1.cancelOrder(1, 'AAPL');
  }, 5000);
  
  
  class TransactionCost {
    constructor(feePercentage = 0.001) {
      this.feePercentage = feePercentage;
    }
  
    calculateFee(price, quantity) {
      return price * quantity * this.feePercentage;
    }
  }
  
  
  class EnhancedExecutionEngine extends ExecutionEngine {
    constructor() {
      super();
      this.transactionCost = new TransactionCost();
    }
  
    executeTrade(bid, ask, quantity) {
      const fee = this.transactionCost.calculateFee(ask.price, quantity);
      const tradeValue = ask.price * quantity;
      const netTradeValue = tradeValue - fee;
  
      bid.quantity -= quantity;
      ask.quantity -= quantity;
      if (bid.quantity === 0) this.bids.shift();
      if (ask.quantity === 0) this.asks.shift();
  
      const trade = {
        symbol: bid.symbol,
        price: ask.price,
        quantity: quantity,
        fee: fee,
        netTradeValue: netTradeValue,
        timestamp: Date.now(),
      };
      console.log(`Trade executed: ${quantity} x ${bid.symbol} @ ${ask.price} (Fee: ${fee})`);
      return trade;
    }
  }
  
  
  class Logger {
    static logOrder(order) {
      console.log(`Order Log: ${order.type} ${order.quantity} x ${order.symbol} @ ${order.price}`);
    }
  
    static logTrade(trade) {
      console.log(`Trade Log: ${trade.quantity} x ${trade.symbol} @ ${trade.price} (Net: ${trade.netTradeValue})`);
    }
  
    static logCancel(orderId) {
      console.log(`Cancel Log: Order ID ${orderId} cancelled.`);
    }
  }
  
  
  class StopLimitOrder extends Order {
    constructor(id, symbol, quantity, price, stopPrice, limitPrice, type, timestamp, traderId) {
      super(id, symbol, quantity, price, type, timestamp, traderId);
      this.stopPrice = stopPrice;
      this.limitPrice = limitPrice;
    }
  
    isTriggered(currentPrice) {
      return this.type === 'buy'
        ? currentPrice <= this.stopPrice
        : currentPrice >= this.stopPrice;
    }
  }
  
  class ExecutionEngineWithStopLimit extends ExecutionEngine {
    constructor() {
      super();
    }
  
    placeStopLimitOrder(order) {
      const orderBook = this.getOrderBook(order.symbol);
      if (order instanceof StopLimitOrder) {
        orderBook.addOrder(order);
        console.log(`Stop-limit order placed: ${order.type} ${order.quantity} x ${order.symbol} at ${order.price}`);
      }
    }
  
    matchOrders() {
      Object.keys(this.orderBooks).forEach((symbol) => {
        const orderBook = this.orderBooks[symbol];
        orderBook.matchOrders();
      });
    }
  }
  
  
  const stopLimitOrder = new StopLimitOrder(
    1234, 'AAPL', 100, 160, 155, 158, 'buy', Date.now(), 3
  );
  const advancedExecutionEngine = new ExecutionEngineWithStopLimit();
  advancedExecutionEngine.placeStopLimitOrder(stopLimitOrder);
  

  class MultiOrderExecutionEngine extends ExecutionEngine {
    constructor() {
      super();
    }
  
    placeMultipleOrders(orders) {
      orders.forEach(order => this.placeOrder(order));
    }
  }
  
  const multiOrderExecutionEngine = new MultiOrderExecutionEngine();
  multiOrderExecutionEngine.placeMultipleOrders([
    new Order(123, 'AAPL', 50, 150, 'buy', Date.now(), 1),
    new Order(124, 'GOOG', 20, 2700, 'sell', Date.now(), 2)
  ]);
  
 
  class RiskManager {
    constructor(maxPosition = 1000) {
      this.maxPosition = maxPosition;
    }
  
    checkPosition(position, quantity) {
      return position + quantity <= this.maxPosition;
    }
  }
  
  const riskManager = new RiskManager();
  if (riskManager.checkPosition(500, 300)) {
    console.log('Position accepted.');
  } else {
    console.log('Risk management check failed.');
  }
  