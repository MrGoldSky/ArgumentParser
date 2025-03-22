# ArgumentParser

A C++ library for parsing command-line arguments. This library is designed to simplify the process of handling command-line options in your C++ programs by supporting various argument types, automatic help generation, and positional arguments.

## Features

- Add short and long named arguments
- Supports `int`, `bool`, and `string` types
- Assign default values
- Retrieve parsed values by type


- **Type-safe arguments:** Supports arguments of type `int`, `bool`, and `std::string`.
- **Short and long argument names:** Define arguments with both short (e.g. `-n`) and long names (e.g. `--number`).
- **Default values and required arguments:** 
  - Arguments without a default value are required; if not provided, parsing will fail.
  - Optional arguments can have default values using `.Default(...)`.
- **Value storage:** 
  - Retrieve parsed values using getter methods like `GetIntValue`, `GetStringValue`, and `GetFlag`.
  - Store values directly into external variables with `StoreValue()` (for single values) and `StoreValues()` (for multiple values).
- **Multi-value arguments:** 
  - Support for arguments that can be specified multiple times (e.g. `--param=1 --param=2`).
  - Specify a minimum number of required values using `MultiValue(min_count)`. Default - unlimited
- **Positional arguments:** 
  - Define arguments that are matched by their position on the command line rather than by a flag.
- **Combined flags:** 
  - Supports grouping of short flag arguments (e.g. `-ac` to enable both `-a` and `-c`).
- **Help functionality:** 
  - Add a help flag (e.g. `-h`/`--help`) to automatically generate a help message detailing usage, argument types, default values, and requirements.
- **Dynamic configuration:** 
  - The parser supports repeated parsing. It allows modifying the configuration (e.g., adding new arguments based on previous flags) and parsing again.


## Tests and CMake

The project includes a suite of tests (using [Google Test](https://github.com/google/googletest)) to verify the functionality of the library. The tests cover various aspects such as:

- Parsing of string, integer, and flag arguments.
- Handling of default values and required arguments.
- Multi-value and positional arguments.
- Combined flags and dynamic parsing behavior.
- Automatic help message generation.

### Building and Running Tests with CMake

A `CMakeLists.txt` file is provided for building the project and its tests. To build the project and run tests using CMake, follow these steps:

1. **Create a build directory and navigate into it:**

    ```bash
    mkdir build && cd build
    ```

2. **Configure the project:**

    ```bash
    cmake ..
    ```

3. **Build the project:**

    ```bash
    cmake --build .
    ```

4. **Run the tests:**

    ```bash
    ctest
    ```

    Alternatively, you can run the test executable directly (usually located in the `build/tests` directory).


## Example Code (`main.cpp`)

```cpp
#include <lib/ArgParser.h>

#include <iostream>
#include <numeric>

struct Options {
    bool sum = false;
    bool mult = false;
};

using namespace ArgumentParser;

int main(int argc, char** argv) {
    Options opt;
    std::vector<int> values;

    ArgumentParser::ArgParser parser("Program");
    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
    parser.AddFlag('s', "sum", "add args").StoreValue(opt.sum);
    parser.AddFlag("mult", "multiply args").StoreValue(opt.mult);
    parser.AddHelp('h', "help", "Program accumulate arguments");

    if (!parser.Parse(argc, argv)) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if (parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    if (opt.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if (opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        std::cout << parser.HelpDescription();
        return 1;
    }

    return 0;
}

```

## Build

```bash
g++ main.cpp lib/ArgParser.cpp -o app
```

## License

MIT License
