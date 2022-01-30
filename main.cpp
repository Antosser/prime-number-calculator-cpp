#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>

#define logval(varname) std::cout << #varname": " << varname << "\n"

void trd(const std::vector<uint64_t>& primes, bool& runthread, bool log, const std::string& ifilename);
inline bool fileExists(const std::string& name);
inline uint64_t fast_sqrt(uint64_t num);

enum class readOperation {read_zero_count, read_number};

int main(int argc, char** argv) {
	uint64_t n;
	std::string ifilename;
	bool log = false;
	bool binin = false;
	bool binout = false;
	bool decout = true;
	bool convert = false;

	bool defn = false;
	bool defifilename = false;

	for (uint64_t i = 1; i < argc; i++) {
		std::string argstring = argv[i];
		if (std::string(argstring) == "-log")
			log = true;
		else if (argstring == "-binin") {
			binin = true;
		}
		else if (argstring == "-binout") {
			binout = true;
			decout = false;
		}
		else if (argstring == "-allout") {
			binout = true;
			decout = true;
		}
		else if (argstring == "-convert") {
			binin = true;
			binout = true;
			convert = true;
		}
		else if (!defifilename) {
			ifilename = argv[i];
			defifilename = true;
		}
		else if (!defn) {
			n = atoi(argv[i]);
			defn = true;
		}
		else {
			std::cout << "Unexpected argument: " << argv[i] << std::endl;
			exit(1);
		}

	}
	if (!defn && !convert) {
		std::cout << "Numbers: ";
		std::cin >> n;
	}
	if (!defifilename) {
		std::cout << "File: ";
		std::cin >> ifilename;
	}
	
	uint64_t i = 2;
	std::vector<uint64_t> primes = {};
	if (!binin) {
		if (fileExists(ifilename)) {
			std::cout << "Restoring vector from decimal...\n";
			std::ifstream ifile(ifilename);
			if (ifile.is_open()) {
				std::string line;
				uint64_t j = 0;
				uint64_t total = 0;
				while (std::getline(ifile, line)) {
					++j;
					total += line.length() + 1;
					if (!(j & 0xfffff))
						std::cout << total / 0x100000 << "mb\n";
					primes.push_back(std::stoi(line));
				}
				i = primes[primes.size() - 1] + 1;
				ifile.close();
			}
			else {
				std::cout << "Unable to open file";
				exit(1);
			}
		}
	}
	else {
		if (fileExists(ifilename + ".prime")) {
			std::cout << "Restoring vector from binary...\n";
			std::ifstream ifile(ifilename + ".prime", std::ios::binary);
			if (ifile.is_open()) {
				uint64_t number = 0;
				int readbytes = 1;
				int length = 0;
				char* read = new char[8];
				readOperation operation = readOperation::read_zero_count;

				int count_debug = 0;
				int bytes_debug = 0;
				int last_readbytes_debug = 999;

				while (ifile.read(read, readbytes)) {
					count_debug++;
					bytes_debug += readbytes;

					if (operation == readOperation::read_zero_count) {
						length = *read;
						operation = readOperation::read_number;
						readbytes = length;
						delete[8] read;
						read = new char[8];
						memset(read, 0x00, 8);
					}
					else if (operation == readOperation::read_number) {
						number = *((uint64_t*)read);
						operation = readOperation::read_zero_count;
						readbytes = 1;
						primes.push_back(number);
					}

					last_readbytes_debug = readbytes;
				}

				i = primes[primes.size() - 1] + 1;
			}
			else {
				std::cout << "Unable to open file";
				exit(1);
			}
		}
	}
	if (!convert)
		std::cout << "Calculating...\n";
	bool runthread = true;
	auto starttime = std::chrono::high_resolution_clock::now();
	if (!convert) {
		std::thread t1(trd, std::ref(primes), std::ref(runthread), log, std::ref(ifilename));

		for (; primes.size() < n; i++) {
			uint32_t root = fast_sqrt(i);
			for (uint64_t ci : primes) {
				if (i % ci == 0)
					goto brk;
				if (ci > root)
					break;
			}
			primes.push_back(i);
		brk:;
		}
	}
	runthread = false;
	if (decout) {
		std::cout << "Writing decemal...\n";
		{
			std::ofstream file(ifilename);
			std::string data;
			uint64_t count = 0;
			for (uint64_t i = 0; i < primes.size(); i++) {
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
	}
	if (binout) {
		std::cout << "Writing binary...\n";
		{
			std::ofstream file(ifilename + ".prime", std::ios::binary);

			for (uint64_t j : primes) {
				int length = 8;
				const char* jchar = (const char*)&j;
				for (int i = 7; i >= 0; i--) {
					if (jchar[i] == 0) length--;
					else break;
				}

				file.write((char*)&length, 1);
				file.write((char*)&j, length);
			}

			file.close();
		}
	}
	auto duration = std::chrono::high_resolution_clock::now() - starttime;
	std::cout << "Done in " << double(duration.count()) / 1000000000 << "s\n";
}

void trd(const std::vector<uint64_t>& primes, bool& runthread, bool log, const std::string& ifilename) {
	std::ofstream logfile;
	if (log) {
		logfile.open(std::string(ifilename) + ".log.csv", std::ios::app);
	}
	long long before = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	while (runthread) {
		std::cout << floor(primes.size() / 1000) / 1000 << "m | " << float(primes.size() - before) / 50 << "k" << std::endl;
		if (log) {
			logfile << primes.size() << "," << float(primes.size() - before) * 20 << std::endl;
		}
		before = primes.size();
	
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

inline bool fileExists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}

inline uint64_t fast_sqrt(uint64_t num) {
	uint64_t root = 0;

	for (int32_t i = 15; i >= 0; i--) {
		uint64_t temp = root | ((uint64_t)1 << i);
		if (temp * temp <= num) {
			root = temp;
		}
	}

	return root;
}