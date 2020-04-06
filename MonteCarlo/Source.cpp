#include<iostream>
#include<thread>
#include<vector>
#include<utility>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <functional>
double f(size_t N, std::vector<std::pair<double, double>> myVector, double& k) {
	k = 0;
	for (int i = 0; i < N; i++) {
		if ((myVector[i].first - 50) * (myVector[i].first - 50) + (myVector[i].second - 50) * (myVector[i].second - 50) <= 2500) {
			k += 1;
		}
	}
	return k;
	//std::cout << 4*k / N << std::endl;
}
void consistently(size_t N, std::vector<std::pair<double, double>> myVector) {
	double result = 0;
	std::cout << 4 * f(N, myVector, result) / N << std::endl;
}
void parallel(size_t N) {
	const std::size_t length = N;
	const std::size_t min_per_thread = length / 4;
	const std::size_t max_threads = (length + min_per_thread - 1) / min_per_thread;
	const std::size_t hardware_threads = std::thread::hardware_concurrency();
	const std::size_t num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	std::size_t block_size = length / num_threads;
	std::vector < double > results(num_threads);
	std::vector < std::thread > threads;
	for (std::size_t i = 0; i < num_threads - 1; ++i) {
		threads.push_back(std::thread(f, block_size, std::ref(results[i])));
		block_size += block_size;
	}
	std::for_each(threads.begin(), threads.end(),std::mem_fn(&std::thread::join));
	std::cout << 4 * std::accumulate(results.begin(), results.end(), 0.) / N << std::endl;
}
int main() {
	std::vector<std::pair<double, double>> myVector;
	std::srand(unsigned(std::time(0)));
	for (int i = 0; i <100; i++) {
		myVector.push_back(std::make_pair(std::rand() % 100, std::rand() % 100));
	}
	std::thread thread(consistently,100, myVector);
	thread.join();
	//parallel(1000);
	return 0;
}