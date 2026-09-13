# Electricity Bill Calculator

This project is a portable C11 refactor of the original Turbo C electricity-billing program. It supports allocating a known electricity amount across multiple meters, calculating multiple meters from a supplied per-unit cost, and calculating one meter using the historical Medak household tariff table.

## Build

On Linux, macOS, or another system with a C11 compiler:

```sh
make
```

The `.C` filename is retained for compatibility with the original repository. The Makefile explicitly tells the compiler to treat it as C rather than C++.

To build manually:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -x c ELECTRIC.C -o electricity_bills
```

## Run

```sh
./electricity_bills
```

A copy of each completed calculation is written to `current.txt` in the working directory. Existing output is replaced for each completed calculation.

## Refactor highlights

The application no longer depends on `conio.h`, `clrscr()`, `getch()`, Turbo C, or a hard-coded DOS path. Input is line-based and validated, meter readings cannot produce negative consumption, meter count is bounded, calculation state is local to each operation, totals are accumulated once, and the tariff bands correctly distinguish 401–800 units from above 800 units.

The tariff behavior remains a flat per-unit rate selected by the total consumption band, matching the behavior intended by the original single-meter mode. Rates are preserved from the original source and should be reviewed before use for current utility billing.
