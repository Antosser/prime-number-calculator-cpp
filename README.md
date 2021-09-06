# Antosser's really fast "Prime Number Calculator"
## Usage
1. Execute the app with a commandline.
2. Enter how many prime numbers you wish to calculate
3. Enter the name of the file where you want to store the results (It will restore the pervious state if you type in an existing file)
4. Press enter
5. Wait for the calculation to end or press **Escape** if you want to stop the calculation and proceed to saving part

## How it works
First a std::vector<int> is being created. (That's like an array that has the ability to expand.)
```cpp
std::vector<int> primes = {};
```
Then you do that
```cpp
for (int i = 2; primes.size() < n; i++) {
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
} // Try understand it yourself. Not that hard
```
