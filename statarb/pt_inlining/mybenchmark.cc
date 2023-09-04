#include <benchmark/benchmark.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <cmath>
#include <iostream>

using namespace std;


vector<double> readCSV(const string& filename){
  std::vector<double> prices;
  std::ifstream file(filename);
  std::string line;

  std::getline(file, line);  // Skip the header line

  while (std::getline(file, line)) {
	std::stringstream ss(line);
	std::string value;
	std::vector<std::string> row;

	while (std::getline(ss, value, ',')) {  // Split the line by commas
  	row.push_back(value);
	}

	double adjClose = std::stod(row[5]);  // Assuming "Adj Close" is at index 5
	prices.push_back(adjClose);
  }


  return prices;
}

__attribute__((always_inline)) double calc_mean(const std::vector<double>& v, double sum) {
  // implementation...
  //return std::accumulate(v.begin(), v.end(), 0.0) / v.size();

    return (sum / v.size()); 
  
}

__attribute__((always_inline)) double calc_stddev(const std::vector<double>& v, double mean, double sq_sum) {
  // implementation...
  //double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
  //return std::sqrt(sq_sum / v.size() - mean * mean);


  return std::sqrt(sq_sum / v.size() - mean * mean);
}

void pairs_trading_strategy(const std::vector<double>& stock1_prices, const std::vector<double>& stock2_prices, size_t N) {
  // implementation...

  //int counter = 0;
  
  for(size_t i = N; i < stock1_prices.size(); ++i) {

    std::vector<double> spread(stock1_prices.begin() + i - N, stock1_prices.begin() + i);

    for(size_t j = 0; j < N; ++j) {
      spread[j] -= stock2_prices[i - N + j];
    }

    double sum = 0.0;
    double sq_sum = 0.0;
    for(auto& s : spread) {
      sum += s;
      sq_sum += s * s;
    }

    double mean = calc_mean(spread, sum);
    double stddev = calc_stddev(spread, mean, sq_sum);

    double current_spread = stock1_prices[i] - stock2_prices[i];
    double z_score = (current_spread - mean) / stddev;  // calculate z-score
	

    if(z_score > 1.0) {
      //counter++;
      //cout << z_score << endl;
      // Short signal 
      //cout << "short" << endl;
    } else if(z_score < -1.0) {
      //counter++;
      // Long signal
      //cout << "long" << endl;
    } else if (abs(z_score) < 0.8) {
      // close positions
    } else{
      //counter++;
      // No signal
    }

  }
  
  //cout << counter << endl;
}

std::vector<double> stock1_prices;
std::vector<double> stock2_prices;

void read_prices() {
 
  string gs_file = "GS.csv";
  string ms_file = "MS.csv";

  stock1_prices = readCSV(gs_file);
  stock2_prices = readCSV(ms_file);
 
}


void BM_PairsTradingStrategy(benchmark::State& state) {
  if (stock1_prices.empty() || stock2_prices.empty()) {
	read_prices();
  }
  for (auto _ : state) {
	pairs_trading_strategy(stock1_prices, stock2_prices, 8);
  }
}

BENCHMARK(BM_PairsTradingStrategy);

BENCHMARK_MAIN();
