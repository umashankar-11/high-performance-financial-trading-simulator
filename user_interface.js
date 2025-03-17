let accounts = [];
let marketData = [];
let tradeHistory = [];

class Account {
    constructor(username, balance) {
        this.username = username;
        this.balance = balance;
        this.openPositions = [];
    }

    placeTrade(symbol, volume, price) {
        let tradeValue = volume * price;
        if (this.balance >= tradeValue) {
            this.balance -= tradeValue;
            this.openPositions.push({ symbol, volume, price });
            tradeHistory.push({ username: this.username, symbol, volume, price, type: "BUY" });
            updateTradeHistory();
            updateAccountList();
            alert(`Trade executed: ${volume} shares of ${symbol} at ${price} each.`);
        } else {
            alert("Insufficient funds for this trade!");
        }
    }

    closeTrade(symbol, volume, price) {
        let position = this.openPositions.find(pos => pos.symbol === symbol && pos.volume === volume);
        if (position) {
            let tradeValue = volume * price;
            this.balance += tradeValue;
            this.openPositions = this.openPositions.filter(pos => !(pos.symbol === symbol && pos.volume === volume));
            tradeHistory.push({ username: this.username, symbol, volume, price, type: "SELL" });
            updateTradeHistory();
            updateAccountList();
            alert(`Trade closed: ${volume} shares of ${symbol} at ${price} each.`);
        } else {
            alert("No matching open positions found.");
        }
    }
}

function createAccount(username, initialBalance) {
    accounts.push(new Account(username, initialBalance));
    updateAccountList();
}

function findAccount(username) {
    return accounts.find(account => account.username === username);
}

function simulateMarketData() {
    const symbols = ["AAPL", "GOOG", "AMZN", "MSFT"];
    marketData = symbols.map(symbol => {
        return { symbol, price: (Math.random() * 1000).toFixed(2) };
    });
    updateMarketData();
}

function updateMarketData() {
    let marketList = document.getElementById("marketData");
    marketList.innerHTML = "";
    marketData.forEach(data => {
        let marketItem = document.createElement("li");
        marketItem.innerHTML = `${data.symbol}: $${data.price}`;
        marketList.appendChild(marketItem);
    });
}

function updateAccountList() {
    let accountList = document.getElementById("accountList");
    accountList.innerHTML = "";
    accounts.forEach(account => {
        let accountItem = document.createElement("li");
        accountItem.innerHTML = `${account.username} - Balance: $${account.balance}`;
        accountList.appendChild(accountItem);
    });
}

function updateTradeHistory() {
    let tradeHistoryList = document.getElementById("tradeHistory");
    tradeHistoryList.innerHTML = "";
    tradeHistory.forEach(trade => {
        let tradeItem = document.createElement("li");
        tradeItem.innerHTML = `${trade.username} ${trade.type} ${trade.volume} shares of ${trade.symbol} at $${trade.price}`;
        tradeHistoryList.appendChild(tradeItem);
    });
}

document.getElementById("createAccountBtn").addEventListener("click", function () {
    let username = document.getElementById("username").value;
    let initialBalance = parseFloat(document.getElementById("initialBalance").value);
    if (username && !isNaN(initialBalance)) {
        createAccount(username, initialBalance);
        alert(`Account created for ${username}`);
    } else {
        alert("Please enter valid details.");
    }
});

document.getElementById("placeTradeBtn").addEventListener("click", function () {
    let username = document.getElementById("tradeUsername").value;
    let symbol = document.getElementById("tradeSymbol").value;
    let volume = parseInt(document.getElementById("tradeVolume").value);
    let price = parseFloat(document.getElementById("tradePrice").value);

    let account = findAccount(username);
    if (account && symbol && volume && price) {
        account.placeTrade(symbol, volume, price);
    } else {
        alert("Invalid trade details.");
    }
});

document.getElementById("closeTradeBtn").addEventListener("click", function () {
    let username = document.getElementById("closeUsername").value;
    let symbol = document.getElementById("closeSymbol").value;
    let volume = parseInt(document.getElementById("closeVolume").value);
    let price = parseFloat(document.getElementById("closePrice").value);

    let account = findAccount(username);
    if (account && symbol && volume && price) {
        account.closeTrade(symbol, volume, price);
    } else {
        alert("Invalid trade details.");
    }
});

function setup() {
    let createForm = `
        <h2>Create Account</h2>
        <input type="text" id="username" placeholder="Username">
        <input type="number" id="initialBalance" placeholder="Initial Balance">
        <button id="createAccountBtn">Create Account</button>
    `;

    let placeTradeForm = `
        <h2>Place Trade</h2>
        <input type="text" id="tradeUsername" placeholder="Username">
        <input type="text" id="tradeSymbol" placeholder="Stock Symbol (e.g., AAPL)">
        <input type="number" id="tradeVolume" placeholder="Volume">
        <input type="number" id="tradePrice" placeholder="Price">
        <button id="placeTradeBtn">Place Trade</button>
    `;

    let closeTradeForm = `
        <h2>Close Trade</h2>
        <input type="text" id="closeUsername" placeholder="Username">
        <input type="text" id="closeSymbol" placeholder="Stock Symbol">
        <input type="number" id="closeVolume" placeholder="Volume">
        <input type="number" id="closePrice" placeholder="Price">
        <button id="closeTradeBtn">Close Trade</button>
    `;

    let marketDataContainer = `
        <h2>Market Data</h2>
        <ul id="marketData"></ul>
    `;

    let accountListContainer = `
        <h2>Account List</h2>
        <ul id="accountList"></ul>
    `;

    let tradeHistoryContainer = `
        <h2>Trade History</h2>
        <ul id="tradeHistory"></ul>
    `;

    let container = document.getElementById("container");
    container.innerHTML = createForm + placeTradeForm + closeTradeForm + marketDataContainer + accountListContainer + tradeHistoryContainer;
}

document.addEventListener("DOMContentLoaded", function () {
    setup();
    simulateMarketData();
    setInterval(simulateMarketData, 5000);
});

document.write(`
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
        }
        #container {
            width: 70%;
            margin: 0 auto;
            background-color: white;
            padding: 20px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        h2 {
            color: #333;
        }
        input, button {
            padding: 10px;
            margin: 10px 0;
            width: 100%;
            box-sizing: border-box;
        }
        button {
            background-color: #4CAF50;
            color: white;
            border: none;
            cursor: pointer;
        }
        button:hover {
            background-color: #45a049;
        }
        ul {
            padding-left: 20px;
        }
        li {
            margin-bottom: 5px;
        }
    </style>
`);
