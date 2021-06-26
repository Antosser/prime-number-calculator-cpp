#include <iostream>
#include <vector>
#include <string>
//#include <thread>
//#include <Windows.h>

int main(int argc, char** argv) {
	if (argc < 2 || argc > 2)
		return -1;

	int n = std::stoi(argv[1]);

	std::vector<int> primes = {};
	for (unsigned long long i = 2; primes.size() < n; i++) {
		for (int ci : primes) {
			if (i % ci == 0)
				goto brk;
		}
		primes.push_back(i);
		std::cout << i << std::endl;
		brk:;
	}
}