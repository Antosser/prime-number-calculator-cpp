# Antosser's really fast "Prime Number Calculator"
## Usage
1. Open commandline in the directory containing the executable  
1. Execute the app with the following syntax:  
```prime <filename> (<amount> [-log] [-binin] [-binout] [-allout]) || <-todec> || <-tobin>```
1. Wait for the calculations to end  

## Parameters
* `-log` - Saves the progress and speed at any time (basically the console output) to "<filename>.log.csv" so it can be futherly opened in Google Sheets or Exel
* `-binin` - Takes a compressed binary file as input (`{filename}.prime`)
* `-binout` - Outputs binary compressed file instead of decimal (`{filename}.prime`)
* `-allout` - Outputs both the compressed binary file and the decimal file
* `-tobin` - Converts the decimal file to compressed binary
* `-todec` - Converts the binary file to compressed decimal

## How it works
```cpp
std::vector<int> primes = {}; // Create a vector class
for (int i = 2; primes.size() < n; i++) { // Loop until vector's size is n
  int root = sqrt(i) + 1;
  for (int ci : primes) { // Loop trough every know prime
    if (i % ci == 0)
      goto brk; // Number is not prime
    if (ci > root)
      break; // Number is prime
  }
  primes.push_back(i);
  brk:;
}

// Print primes
for (auto i : primes)
  std::cout << i << std::endl;
```
