#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>
#include <Windows.h>

void trd(std::vector<int>& primes, bool& runthread);
inline bool fileExists(const std::string& name);
void write_file(int n, std::ofstream& file);

int main(int argc, char** argv) {
	long long int n;
	std::cout << "Numbers: ";
	std::cin >> n;
	std::string ifilename;
	std::cout << "File: ";
	std::cin >> ifilename;
	//std::string ofilename;
	//std::cout << "Output: ";
	//std::cin >> ofilename;

	unsigned long long i = 2;
	std::vector<int> primes = {};
	if (fileExists(ifilename)) {
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

	std::cout << "Calculating...\n";
	bool runthread = true;
	std::thread t1(trd, std::ref(primes), std::ref(runthread));
	for (; primes.size() < n; i++) {
		int root = sqrt(i) + 1;
		for (int ci : primes) {
			if (i % ci == 0)
				goto brk;
			if (ci > root)
				break;
		}
		primes.push_back(i);
		brk:;
		if (GetKeyState(0x1B) & 0x8000)
			goto exit;
	}
	exit:;
	runthread = false;
	std::cout << "Writing...\n";
	std::ofstream file(ifilename);
	std::string data;
	for (auto el : primes)
		data += std::to_string(el) + '\n';
	file.write(data.data(), data.size());
	file.close();
	std::cout << "Done!\n";
}

void trd(std::vector<int>& primes, bool& runthread) {
	while (runthread) {
		std::cout << primes.size() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

inline bool fileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}