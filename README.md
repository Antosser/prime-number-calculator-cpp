# Antosser's really fast "Prime Number Calculator"
## Usage
1. Execute the app
1. Enter how many prime numbers you wish to calculate
1. Enter the name of the file where you want to store the results (It will restore the pervious state if you type in an existing file)
1. Press enter
1. Wait for the calculations to end
----- or ------
1. Open commandline in the directory containing the executable
1. Execute the app with the following syntax:
```batch
prime <filename> <amount>
```
1. Wait for the calculations to end



## How it works
```cpp
std::vector<int> primes = {}; // Create a vector class
for (int i = 2; primes.size() < n; i++) { // Loop trough every integer between 2 and n
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
