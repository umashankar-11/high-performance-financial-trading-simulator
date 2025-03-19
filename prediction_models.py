import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
import matplotlib.pyplot as plt
import datetime
import random

def generate_price_data(start_date, end_date, symbol):
    dates = pd.date_range(start=start_date, end=end_date, freq='B')
    data = []
    price = 100
    for date in dates:
        price += random.uniform(-5, 5)
        data.append([date, price])
    df = pd.DataFrame(data, columns=["Date", "Price"])
    df["Symbol"] = symbol
    return df

def generate_multiple_stock_data():
    symbols = ['AAPL', 'GOOG', 'AMZN', 'TSLA']
    all_data = pd.DataFrame()
    for symbol in symbols:
        data = generate_price_data("2020-01-01", "2023-01-01", symbol)
        all_data = pd.concat([all_data, data], ignore_index=True)
    return all_data

def prepare_data(df):
    df['Date'] = pd.to_datetime(df['Date'])
    df['Date'] = df['Date'].apply(lambda x: x.toordinal())
    X = df[['Date']]
    y = df['Price']
    return X, y

def train_model(X, y):
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    scaler = StandardScaler()
    X_train = scaler.fit_transform(X_train)
    X_test = scaler.transform(X_test)
    
    model = LinearRegression()
    model.fit(X_train, y_train)
    
    y_pred = model.predict(X_test)
    
    mse = mean_squared_error(y_test, y_pred)
    r2 = r2_score(y_test, y_pred)
    
    return model, scaler, mse, r2

def predict_price(model, scaler, date, last_date):
    date_ordinal = pd.to_datetime(date).toordinal()
    last_date_ordinal = pd.to_datetime(last_date).toordinal()
    days_diff = date_ordinal - last_date_ordinal
    prediction = model.predict(scaler.transform([[date_ordinal]]))
    return prediction[0]

def plot_predictions(df, model, scaler):
    plt.figure(figsize=(10, 6))
    plt.scatter(df['Date'], df['Price'], color='blue', label='Actual Price')
    future_dates = pd.date_range(df['Date'].max(), pd.to_datetime(df['Date'].max()) + pd.DateOffset(days=30), freq='B')
    future_ordinal = future_dates.toordinal()
    future_prices = model.predict(scaler.transform(future_ordinal.reshape(-1, 1)))
    plt.plot(future_dates, future_prices, color='red', label='Predicted Price')
    plt.title('Stock Price Prediction')
    plt.xlabel('Date')
    plt.ylabel('Price')
    plt.legend()
    plt.show()

def main():
    df = generate_multiple_stock_data()
    X, y = prepare_data(df)
    
    model, scaler, mse, r2 = train_model(X, y)
    print(f"Mean Squared Error: {mse}")
    print(f"R2 Score: {r2}")
    
    last_date = df['Date'].max()
    future_date = '2023-12-01'
    predicted_price = predict_price(model, scaler, future_date, last_date)
    print(f"Predicted price for {future_date}: {predicted_price}")
    
    plot_predictions(df, model, scaler)

if __name__ == "__main__":
    main()

