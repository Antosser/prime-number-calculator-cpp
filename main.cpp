#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
//#include <thread>
//#include <Windows.h>

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Expected 2 arguments\n";
		return -1;
	}

	int n = std::stoi(argv[1]);
	std::ofstream file(argv[2]);
	std::cout << "Numbers: " << n << std::endl;

	std::vector<int> primes = {};
	for (unsigned long long i = 2; primes.size() < n; i++) {
		for (int ci : primes) {
			if (i % 10000 == 0)
				std::cout << primes.size() << std::endl;
			if (i % ci == 0)
				goto brk;
		}
		primes.push_back(i);
		file << i << std::endl;
		brk:;
	}
	file.close();
}