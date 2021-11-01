# Antosser's really fast "Prime Number Calculator"
## Usage
1. Execute the app
2. Enter how many prime numbers you wish to calculate
3. Enter the name of the file where you want to store the results (It will restore the pervious state if you type in an existing file)
4. Press enter
5. Wait for the calculations to end  
----- or ------
1. Open commandline in the directory containing the executable
2. Execute the app with the following syntax:
```batch
prime <filename> <amount>
```
3. Wait for the calculations to end



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
