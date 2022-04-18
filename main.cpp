#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>

// Macro for quick debugging
#define logval(varname) std::cout << #varname": " << varname << "\n"

// Definitions of functions
void trd(const std::vector<uint64_t>& primes, bool& runthread, bool log, const std::string& ifilename);
inline bool fileExists(const std::string& name);
inline uint64_t fast_sqrt(uint64_t num);

enum class readOperation {read_zero_count, read_number};

int main(int argc, char** argv) {
	// n = amount of primes to be calculated
	uint64_t n;
	// Filename
	std::string ifilename;
	// Options
	bool log = false;
	bool binin = false;
	bool binout = false;
	bool decout = true;
	bool convert = false;
	// IsDefined
	bool defn = false;
	bool defifilename = false;

	// Parse arguments
	for (uint64_t i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-log") == 0)
			log = true;
		else if (strcmp(argv[i], "-binin")) {
			binin = true;
		}
		else if (strcmp(argv[i], "-binout")) {
			binout = true;
			decout = false;
		}
		else if (strcmp(argv[i], "-allout")) {
			binout = true;
			decout = true;
		}
		else if (strcmp(argv[i], "-tobin")) {
			binin = false;
			binout = true;
			decout = false;
			convert = true;
		}
		else if (strcmp(argv[i], "-todec")) {
			binin = true;
			binout = false;
			decout = true;
			convert = true;
		}
		else if (strcmp(argv[i], "-help") || strcmp(argv[i], "--help") || strcmp(argv[i], "--?") || strcmp(argv[i], "-?") || strcmp(argv[i], "/?") || strcmp(argv[i], "/help")) {
			std::cout << "Follow this link for help: https://github.com/Antosser/prime-number-calculator#readme\n";
			exit(0);
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
	// If n is not defined, ask
	if (!defn && !convert) {
		std::cout << "Numbers: ";
		std::cin >> n;
	}
	// If ifilename is not defined, ask
	if (!defifilename) {
		std::cout << "File: ";
		std::cin >> ifilename;
	}
	
	// Current calculated
	uint64_t i = 2;
	// Vector with result
	std::vector<uint64_t> primes = {};
	if (!binin) {
		// Parse decimal file
		if (fileExists(ifilename)) {
			// File exists
			std::cout << "Restoring vector from decimal...\n";
			std::ifstream ifile(ifilename);
			if (ifile.is_open()) {
				std::string line;
				uint64_t j = 0;
				uint64_t total = 0;
				// Parse each line
				while (std::getline(ifile, line)) {
					++j;
					total += line.length() + 1;
					// Print state
					if (!(j & 0xfffff))
						std::cout << primes.size() / 1000000 << "m\n";
					primes.push_back(std::stoi(line));
				}
				// Set current calculated to last number in file + 1
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
		// Parse binary file
		if (fileExists(ifilename + ".prime")) {
			// File exists
			std::cout << "Restoring vector from binary...\n";
			std::ifstream ifile(ifilename + ".prime", std::ios::binary);
			if (ifile.is_open()) {
				// number = parsed number
				uint64_t number = 0;
				// How many bytes to read next
				int readbytes = 1;
				// Same
				int length = 0;
				// Buffer
				char* buffer = new char[8];
				// State: read_zero_count OR read_number
				readOperation readOperation = readOperation::read_zero_count;

				// Cycle trough file
				while (ifile.read(buffer, readbytes)) {
					if (readOperation == readOperation::read_zero_count) {
						length = *buffer;
						// Switch operation
						readOperation = readOperation::read_number;
						readbytes = length;
						// Clear read
						memset(buffer, 0x00, 8);
					}
					else if (readOperation == readOperation::read_number) {
						// Cast read (char*) to number (uint64_t)
						number = *((uint64_t*)read);
						// Switch operation
						readOperation = readOperation::read_zero_count;
						readbytes = 1;
						// Add number to list
						primes.push_back(number);
					}
				}

				// Set current calculated to last number in file + 1
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
	// Timer
	auto starttime = std::chrono::high_resolution_clock::now();
	// Create log thread
	std::thread* t1;
	if (!convert) {
		// Init thread
		t1 = new std::thread(trd, std::ref(primes), std::ref(runthread), log, std::ref(ifilename));

		// Explained in GitHub
		for (; primes.size() < n; i++) {
			uint64_t root = fast_sqrt(i);
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
		std::cout << "Writing decimal...\n";
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

			// Loop trough primes
			for (uint64_t j : primes) {
				// Get length of current number
				int length = 8;
				const char* jchar = (const char*)&j;
				for (int i = 7; i >= 0; i--) {
					if (jchar[i] == 0) length--;
					else break;
				}
				
				// Write length and number to file
				file.write((char*)&length, 1);
				file.write((char*)&j, length);
			}

			file.close();
		}
	}

	// Print clock
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
		std::cout << floor(primes.size() / 1000) / 1000 << "m | " << float(primes.size() - before) / 50 << "k" << '\n';
		if (log) {
			logfile << primes.size() << "," << float(primes.size() - before) * 20 << '\n';
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