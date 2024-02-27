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

- **String-to-big-integer conversion and vice versa**

- **Basic arithmetic operations:** 
    - Addition `+`
    - Subtraction `-`
    - Multiplication `*`
    - Integer division `/`
    - Modulo division `%`

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
- GCC (or another C compiler)

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

4. Build a library:

    ```sh
    cmake --build . --clean-first
    ```

## Usage

TODO

## Examples

TODO

## Licenses

The project is licensed under an [MIT License][license-url].

<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[license-shield]: https://img.shields.io/github/license/artem-burashnikov/big-int-lib.svg?style=for-the-badge&color=blue
[license-url]: LICENSE