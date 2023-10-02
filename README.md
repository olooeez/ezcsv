[![Test](https://github.com/olooeez/ezcsv/actions/workflows/test.yml/badge.svg)](https://github.com/olooeez/ezcsv/actions/workflows/test.yml)

# ezcsv

ezcsv is a lightweight and easy-to-use CSV parsing library written in C. It provides simple functions to parse CSV data, manipulate CSV rows, and handle CSV errors. Whether you need to read CSV files, generate CSV strings, or work with CSV data in your C application, ezcsv has you covered.

## Features

- Parse CSV data from files or strings.
- Create and manipulate CSV rows.
- Write CSV data to files or strings.
- Retrieve CSV field values by index.
- Handle common CSV errors with ease.

## Prerequisites

Before using ezcsv, make sure you have the following requirements installed on your system:

1. **GCC Compiler**: You need GCC compiler to build and use the library.
2. **Makefile**: You need Makefile in order to build and install the library.
3. **glibc**: The project uses some functions that are in glibc. 

## Installation

To use ezcsv in your C project, follow these steps:

### Step 1: Clone the Repository

Clone the ezcsv repository to your project's directory:

```
git clone https://github.com/olooeez/ezcsv.git
cd ezcsv
```

### Step 2: Build the Library

Compile the ezcsv library by running the following command:

```
make
```

This command will create a shared library (`libezcsv.so`) in the `build` directory.

### Step 3: Install the Library

To use ezcsv in your C project, install the library by running the following command:

```
sudo make install
```

### Step 4: Compile Your Project

When compiling your project, make sure to link it with the ezcsv library:

```
gcc -o your_program your_program.c -lezcsv
```

## Usage

Here's a brief example of how to use ezcsv to parse a CSV file and access its data:

```c
#include <stdio.h>
#include <ezcsv.h>

int main() {
    // Create a CSVParser to read a CSV file
    CSVParser *parser = csv_parser_create("example.csv", ',');

    if (!parser) {
        fprintf(stderr, "Error: Unable to create CSV parser\n");
        return 1;
    }

    // Read rows from the CSV file
    CSVRow *row;

    while ((row = csv_read_row(parser)) != NULL) {
        for (int i = 0; i < row->field_count; i++) {
            printf("Field %d: %s\n", i, csv_get_field(row, i));
        }

        // Don't forget to free the row
        csv_row_destroy(row);
    }

    // Clean up the parser
    csv_parser_destroy(parser);

    return 0;
}
```

## Documentation

For detailed documentation and usage examples, please refer to the [ezcsv GitHub repository](https://github.com/olooeez/ezcsv).

## Contributing

Contributions to ezcsv are welcome! If you find a bug or have an idea for improvement, please open an issue or submit a pull request.

## License

This project is licensed under the [Apache License 2.0](https://github.com/olooeez/ezcsv/blob/main/LICENSE). See the LICENSE file for more details.