#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>

void trd(std::vector<int>& primes);

int main(int argc, char** argv) {
	int n;
	std::cout << "Numbers: ";
	std::cin >> n;
	std::string ifilename;
	std::cout << "Input: ";
	std::cin >> ifilename;
	std::string ofilename;
	std::cout << "Output: ";
	std::cin >> ofilename;

	unsigned long long i = 2;
	std::vector<int> primes = {};
	if (ifilename != "0") {
		std::cout << "Restoring vector...\n";
		std::ifstream ifile(ifilename);
		if (ifile.is_open()) {
			std::string line;
			while (std::getline(ifile, line)) {
				primes.push_back(std::stoi(line));
			}
			i = primes[primes.size() - 1];
			ifile.close();
		}
		else std::cout << "Unable to open file";
	}
	std::ofstream file(ofilename);

	std::cout << "Calculating...\n";
	std::thread t1(trd, std::ref(primes));
	for (; primes.size() < n; i++) {
		int root = sqrt(i) + 1;
		for (int ci : primes) {
			if (i % ci == 0)
				goto brk;
			if (ci > root)
				break;
		}
		primes.push_back(i);
		file << i << std::endl;
	brk:;
	}
	file.close();
	std::cout << "Done!\n";
	system("pause");
}

void trd(std::vector<int>& primes) {
	while (true) {
		std::cout << primes.size() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}