# big-int-lib

[![MIT License][license-shield]][license-url]

## Overview

The repository provides a C library which implements a set of basic arithmetic operations on arbitrary-precision integers.

### Project structure

Below is the default project structure.

```ignorelang
big-int-lib/
├── big-int-lib
│   └── (source files)
│   
├── tests
│   └── (test files)
│
├── CMakeLists.txt
├── LICENSE
└── README.md
```

Source files have self explanatory names.

## Features

Biginteger structure can be initialized from and converted to a string and integers.

Digits are stored in little-endian order.

Make sure the integer type can hold a given bigint before converting to integer.

- **Basic arithmetic operations:** 
    - Addition `+`
    - Subtraction `-`
    - Multiplication `*`
    - Integer division `/`
    - Modulo division `%`

All operations return a new bigint structure.

## Table of contents

- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building](#building)
- [Usage](#usage)
- [Examples](#examples)
- [Licenses](#licenses)

## Getting Started

### Prerequisites

- CMake (version 3.22.1 or higher)
- GCC

### Building

Open the terminal and follow these steps:

1. Clone the repository:

    ```sh
    git clone git@github.com:artem-burashnikov/big-int-lib.git
    ```

2. Navigate to the project root:

    ```sh
    cd big-int-lib
    ```

3. Generate Makefile using CMake:

    ```sh
    mkdir build && cd build;
    cmake ..
    ```

4. Build a static library:

    ```sh
    cmake --build . --clean-first
    ```

5. Now you can include a `bigint.h` header file in your source files and link a static library `libbigint.a` during a compilation.

## Usage
Below are ways you can utilize functions in this library.

### `Initialize and free a bigint`
```c
#include "bigint.h"
/* Initialization. */
bigint_t *a = bigint_from_str("2236913125512124108572352");
bigint_t *b = bigint_from_int(3236);

/* Clean up. */
bifree(a);
bifree(b);
```

### `Perform arithmetic operations`
```c
#include "bigint.h"

bigint_t *a_sum_b = bigint_sum(a, b);
bigint_t *a_sub_b = bigint_sub(a, b);
bigint_t *a_mul_b = bigint_mul(a, b);
bigint_t *a_div_b = bigint_div(a, b);
bigint_t *a_mod_b = bigint_mod(a, b);

/* Clean up. */
bifree(a);
bifree(b);
bifree(a_sum_b);
bifree(a_sub_b);
bifree(a_mul_b);
bifree(a_div_b);
bifree(a_mod_b);
```

## Examples

For example, you can calculate `1000!` using functions defined in this library.

```c
bigint_t *factorial(const unsigned int n) {
  bigint_t *resp, *tmp, *factor;
  unsigned int p;

  resp = tmp = factor = NULL;

  resp = bigint_from_int(1);

  if ((n == 0) || (n == 1)) {
    return resp;
  }

  for (p = 2; p <= n; ++p) {
    factor = bigint_from_int(p);
    tmp = bigint_mul(resp, factor);
    bifree(factor);
    bifree(resp);
    resp = tmp;
  }

  return resp;
}

bigint_t factorial_1000 = factorial(1000);
char *str = bigint_to_str(factorial_1000);

/* str now hold all digits of 1000! in little-endian order. */

bifree(factorial_1000);
free(str);
```

## Licenses

The project is licensed under an [MIT License][license-url].

<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[license-shield]: https://img.shields.io/github/license/artem-burashnikov/big-int-lib.svg?style=for-the-badge&color=blue
[license-url]: LICENSE