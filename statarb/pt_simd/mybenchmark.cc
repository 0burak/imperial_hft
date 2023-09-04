
#include <benchmark/benchmark.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <cmath>
#include <iostream>

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <cmath>
#include <immintrin.h>
#include <iostream>
#include <vector>
#include <deque>
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

double calc_mean(const std::vector<double>& v) {
    assert(v.size() % 4 == 0);  // For simplicity, assume size is a multiple of 4
    
    __m256d sum_vec = _mm256_setzero_pd();

    for (size_t i = 0; i < v.size(); i += 4) {
        __m256d v_vec = _mm256_loadu_pd(&v[i]);
        sum_vec = _mm256_add_pd(sum_vec, v_vec);
    }

    __m256d temp = _mm256_hadd_pd(sum_vec, sum_vec);
    __m256d sum_vec_total = _mm256_add_pd(temp, _mm256_permute2f128_pd(temp, temp, 0x1));

    double sum = _mm_cvtsd_f64(_mm256_castpd256_pd128(sum_vec_total));
    return sum / v.size();
}

double calc_stddev(const std::vector<double>& v, double mean) {
    assert(v.size() % 4 == 0);  // For simplicity, assume size is a multiple of 4
    
    __m256d sum_vec = _mm256_setzero_pd();
    __m256d sq_sum_vec = _mm256_setzero_pd();

    for (size_t i = 0; i < v.size(); i += 4) {
        __m256d v_vec = _mm256_loadu_pd(&v[i]);
        sum_vec = _mm256_add_pd(sum_vec, v_vec);
        sq_sum_vec = _mm256_add_pd(sq_sum_vec, _mm256_mul_pd(v_vec, v_vec));
    }

    __m256d temp1 = _mm256_hadd_pd(sum_vec, sum_vec);
    __m256d sum_vec_total = _mm256_add_pd(temp1, _mm256_permute2f128_pd(temp1, temp1, 0x1));

    __m256d temp2 = _mm256_hadd_pd(sq_sum_vec, sq_sum_vec);
    __m256d sq_sum_vec_total = _mm256_add_pd(temp2, _mm256_permute2f128_pd(temp2, temp2, 0x1));

    double sum = _mm_cvtsd_f64(_mm256_castpd256_pd128(sum_vec_total));
    double sq_sum = _mm_cvtsd_f64(_mm256_castpd256_pd128(sq_sum_vec_total));

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

	double mean = calc_mean(spread);
	double stddev = calc_stddev(spread, mean);

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
