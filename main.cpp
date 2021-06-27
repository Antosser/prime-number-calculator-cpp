#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
//#include <thread>
//#include <Windows.h>

int main() {
	std::vector<int> primes = {};
	for (unsigned long long i = 2; primes.size() < 1000; i++) {
		for (int ci : primes) {
			if (i % 10000 == 0)
				std::cout << primes.size() << std::endl;
			if (i % ci == 0)
				goto brk;
		}
	}
}