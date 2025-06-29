# SIMD vs SISD Benchmark

This project compares the performance of SIMD (Single Instruction Multiple Data) and SISD (Single Instruction Single Data) arithmetic operations on large floating-point vectors. The purpose is to measure performance differences in typical vectorized vs. scalar computations.

## Project Structure

* **SIMD.c**

  * Uses SSE instructions with inline assembly to perform operations (+, -, \*, /) on vectors of floats, processing 4 elements at a time.
  * Measures average execution time over 10 trials.
  * Saves results to `wynik_SIMD.txt`.

* **SISD.c**

  * Uses x87 FPU instructions with inline assembly for the same operations on a scalar basis.
  * Measures average execution time over 10 trials.
  * Saves results to `wynik_SISD.txt`.

* **Makefile**

  * Contains commands to build both programs using GCC with correct compiler flags.

## Features

* Tests vector sizes of 2048, 4096, and 8192 elements.
* Measures four arithmetic operations:

  * addition
  * subtraction
  * multiplication
  * division
* Stores results to text files for easy comparison.

## Requirements

* GCC compiler supporting SSE instructions (e.g., `gcc` on Linux)
* `make`
* Standard C library

## Build Instructions

```bash
make
```

This will build both the `SIMD` and `SISD` executables.

## Usage

```bash
./SIMD
./SISD
```

The results will be stored in:

* `wynik_SIMD.txt`
* `wynik_SISD.txt`

## Example output (SIMD)

```
Typ obliczen: SIMD
Liczba liczb: 2048
Sredni czas [s]:
+ 0.000123
- 0.000124
* 0.000131
/ 0.000145
```

## License

This benchmark project is provided for educational and comparative purposes.
