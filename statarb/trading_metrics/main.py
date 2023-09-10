import pandas as pd
import numpy as np
import math
from collections import deque
import matplotlib.pyplot as plt
import quantstats as qs
from collections import deque


def readCSV(filename):
    df = pd.read_csv(filename)
    prices = df['Adj Close'].values  # Assuming "Adj Close" is the column name
    return prices

gs_prices = readCSV("GS.csv")
ms_prices = readCSV("MS.csv")

N = 10
spread = deque(maxlen=N)

cash = 1000000
gs_shares = 0
ms_shares = 0
base_num_shares = 6000  # The number of shares to long or short

gs_position = 0  # positive for long, negative for short
ms_position = 0  # positive for long, negative for short

position_open = False

positions_closed = 0

longs = []
shorts = []
portfolio_values = []

# Data collections for the new graphs
spreads_over_time = []
z_scores_over_time = []
cash_balance_over_time = []
# Initialize lists for tracking
portfolio_balance_no_trades = [cash]  # Starts with initial cash balance
trade_profits = []  # List to hold the P&L for each closed trade


for i in range(N, len(gs_prices)):

    spread.append(gs_prices[i] - ms_prices[i])

    mean = np.mean(spread)
    stddev = np.std(spread)

    current_spread = gs_prices[i] - ms_prices[i]
    z_score = (current_spread - mean) / stddev

    num_shares = base_num_shares * abs(z_score)

    # Append data for new graphs
    spreads_over_time.append(current_spread)
    z_scores_over_time.append(z_score)
    cash_balance_over_time.append(cash)


    if z_score > 1 and position_open == False:
        potential_cost = gs_prices[i] * num_shares - ms_prices[i] * num_shares
        if potential_cost > cash:
            num_shares = int((cash / (gs_prices[i] - ms_prices[i])) * 0.9)
        if gs_shares == 0 and ms_shares == 0:
            gs_shares += num_shares
            print(gs_shares)
            ms_shares -= num_shares
            cash -= gs_prices[i] * num_shares
            cash += ms_prices[i] * num_shares
            gs_position = 1
            ms_position = -1
            position_open = True
            longs.append(i)
        else:   
            print(f"Day {i}: Positions already open, waiting to close.")
    elif z_score < -1 and position_open == False:
        potential_cost = ms_prices[i] * num_shares - gs_prices[i] * num_shares
        if potential_cost > cash:
            num_shares = int((cash / (ms_prices[i] - gs_prices[i])) * 0.9)
        if gs_shares == 0 and ms_shares == 0:
            gs_shares  -= num_shares
            ms_shares += num_shares
            cash += gs_prices[i] * num_shares
            cash -= ms_prices[i] * num_shares
            gs_position = -1
            ms_position = 1
            position_open = True
            shorts.append(i)
        else:
            print(f"Day {i}: Positions already open, waiting to close.")
    elif abs(z_score) < 0.8 and position_open == True:
        if gs_position == 1 and ms_position == -1:
            pnl = (gs_prices[i] - gs_prices[longs[-1]]) * gs_shares + \
                  (ms_prices[i] - ms_prices[longs[-1]]) * ms_shares
            trade_profits.append(pnl)
            cash += gs_prices[i] * gs_shares
            cash -= ms_prices[i] * abs(ms_shares)
            gs_shares = 0
            ms_shares = 0
            positions_closed += 1
            gs_position = 0
            ms_position = 0
            position_open = False
        elif gs_position == -1 and ms_position == 1:
               # Calculate the P&L for this trade and append to the list
            pnl = (gs_prices[shorts[-1]] - gs_prices[i]) * gs_shares + \
                  (ms_prices[shorts[-1]] - ms_prices[i]) * ms_shares
            trade_profits.append(pnl)
            cash -= gs_prices[i] * abs(gs_shares)
            cash += ms_prices[i] * ms_shares
            gs_shares = 0
            ms_shares = 0
            positions_closed += 1
            gs_position = 0
            ms_position = 0
            position_open = False
    else:
        print(f"Day {i}: No positions open, waiting to buy.")


    portfolio_value = cash + (gs_shares * gs_prices[i]) + (ms_shares * ms_prices[i])
    portfolio_values.append(portfolio_value)


    if position_open == False:
        portfolio_balance_no_trades.append(cash)

portfolio_values = pd.Series(portfolio_values)
daily_returns = portfolio_values.pct_change().dropna()
sharpe_ratio = np.sqrt(252) * (daily_returns.mean() / daily_returns.std())

print(f"Portfolio value: {portfolio_value}")
print(f"Positions closed: {positions_closed}")
print(f"Sharpe Ratio: {sharpe_ratio}")


gs_df = pd.read_csv("GS.csv")
dates = gs_df['Date'][N:]  # Adjust the index for the starting point N

portfolio_values = pd.DataFrame(portfolio_values, columns=["Portfolio Value"])
portfolio_values['Date'] = dates.values
portfolio_values['Date'] = pd.to_datetime(portfolio_values['Date'])  # Convert 'Date' to datetime format
portfolio_values.set_index('Date', inplace=True)

# Calculate daily returns
daily_returns = portfolio_values.pct_change().dropna()

# Convert the DataFrame to a Series
# This assumes that your DataFrame daily_returns has only one column. 
# If it has more, select one column explicitly.
daily_returns_series = daily_returns.squeeze()  # Squeezing a one-column DataFrame returns a Series

# Generate the HTML report
qs.reports.html(daily_returns_series, title='Queen\'s Gate Pairs Trading', output='trading_strategy_report.html')



# Existing graph for longs and shorts
plt.figure(figsize=(12, 6))
plt.plot(gs_prices, label='GS')
plt.plot(ms_prices, label='MS')
plt.scatter(longs, [gs_prices[i] for i in longs], color='g', marker='^', label='Long')
plt.scatter(shorts, [gs_prices[i] for i in shorts], color='r', marker='v', label='Short')
plt.xlabel('Days')
plt.ylabel('Price')
plt.title('Stock prices over time with trading actions')
plt.legend()
plt.show()

# Spreads against days
plt.figure(figsize=(12, 6))
plt.plot(spreads_over_time, label='Spread')
plt.xlabel('Days')
plt.ylabel('Spread')
plt.title('Spreads over Time')
plt.legend()
plt.show()

# Z-scores against days
plt.figure(figsize=(12, 6))
plt.plot(z_scores_over_time, label='Z-Score')
plt.axhline(1, color='g', linestyle='--')
plt.axhline(-1, color='g', linestyle='--', label='Signal to Open Position')
plt.axhline(0.8, color='r', linestyle='--', label='Signal to Close Position')
plt.axhline(-0.8, color='r', linestyle='--',)
plt.xlabel('Days')
plt.ylabel('Z-Score')
plt.title('Z-Scores over Time')
plt.legend()
plt.show()


import matplotlib.ticker as ticker
# Cash balance against days
plt.figure(figsize=(12, 6))
plt.plot(cash_balance_over_time, label='Portfolio Balance')
plt.xlabel('Days')
plt.ylabel('Portfolio Balance')
plt.title('Portfolio Balance over Time')

# Disable scientific notation
ax = plt.gca()  # get current axes
ax.yaxis.set_major_formatter(ticker.StrMethodFormatter('{x:,.0f}'))

plt.legend()
plt.show()


# Portfolio balance with no trades open
plt.figure(figsize=(12, 6))
plt.plot(portfolio_balance_no_trades, label='Portfolio Balance')
plt.xlabel('Days')
plt.ylabel('Portfolio Balance')
plt.title('Portfolio Balance Over Time')

# Disable scientific notation for y-axis
ax = plt.gca()
ax.yaxis.set_major_formatter(ticker.StrMethodFormatter('{x:,.0f}'))

plt.legend()
plt.show()


plt.figure(figsize=(12, 6))
plt.bar(range(len(trade_profits)), trade_profits, color='g', alpha=0.6, label='Profit')
plt.bar([i for i, profit in enumerate(trade_profits) if profit < 0], 
        [profit for profit in trade_profits if profit < 0], color='r', alpha=0.6, label='Loss')
plt.xlabel('Trade Number')
plt.ylabel('P&L')
plt.title('P&L for Each Closed Trade')
plt.legend()
plt.show()

