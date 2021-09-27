#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>
#include <Windows.h>

void trd(std::vector<int>& primes, bool& runthread);
bool fileExists(const std::string& name);
unsigned long long isqrt(unsigned long a);

int main(int argc, char** argv) {
	long long int n;
	std::cout << "Numbers: ";
	std::cin >> n;
	std::string ifilename;
	std::cout << "File: ";
	std::cin >> ifilename;

	unsigned long long i = 2;
	std::vector<int> primes = {};
	if (fileExists(ifilename)) {
		std::cout << "Restoring vector...\n";
		std::ifstream ifile(ifilename);
		if (ifile.is_open()) {
			std::string line;
			long long j = 0;
			long long total = 0;
			while (std::getline(ifile, line)) {
				++j;
				total += line.length() + 1;
				if (!(j & 0xfffff))
					std::cout << total / 0x100000 << "mb\n";
				primes.push_back(std::stoi(line));
			}
			i = primes[primes.size() - 1];
			ifile.close();
		}
		else std::cout << "Unable to open file";
	}

	std::cout << "Calculating...\n";
	bool runthread = true;
	std::thread t1(trd, std::ref(primes), std::ref(runthread));
	for (; primes.size() < n; i += 2) {
		//if (i & 0x100000)
		//	primes.(0x100000);
		uint16_t root = 0;
		for (int32_t j = 15; j >= 0; j--) {
			uint16_t temp = root | (1 << j);
			if (temp * temp <= i) {
				root = temp;
			}
		}
		for (int ci : primes) {
			if (i % ci == 0)
				goto brk;
			if (ci > root)
				break;
		}
		primes.push_back(i);
		brk:;
		if (GetKeyState(0x1B) & 0x8000 && GetConsoleWindow() == GetForegroundWindow())
			break;
	}
	runthread = false;
	std::cout << "Writing...\n";
	{
		std::ofstream file(ifilename);
		std::string data;
		int count = 0;
		for (long long int i = 0; i < primes.size(); i++) {
			data += std::to_string(primes[i]) + '\n';
			if (count & 0x1000000) {
				std::cout << float(i) / 0x100000 << "m\n";
				file.write(data.data(), data.size());
				count = 0;
				data.erase();
			}
			count++;
		}
		file.write(data.data(), data.size());
		file.close();
	}
	//
	//for (auto el : primes)
	//	data += std::to_string(el) + '\n';
	//
	std::cout << "Done!\n";
}

void trd(std::vector<int>& primes, bool& runthread) {
	long long before = 0;
	while (runthread) {
		std::cout << floor(primes.size() / 1000) / 1000 << "m | " << float(primes.size() - before) / 100 << "k" << std::endl;
		before = primes.size();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

bool fileExists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

unsigned long long isqrt(unsigned long a) {
	unsigned long rem = 0;
	int root = 0;
	int i;

	for (i = 0; i < 16; i++) {
		root <<= 1;
		rem <<= 2;
		rem += a >> 30;
		a <<= 2;

		if (root < rem) {
			root++;
			rem -= root;
			root++;
		}
	}

	return (unsigned long long)(root >> 1);
}