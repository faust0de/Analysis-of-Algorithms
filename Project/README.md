# Lee 3-D Thinning — C++ Experimental Project

This project implements an educational version of Lee-style 3-D thinning and produces a compact result table for the Analysis of Algorithms report.

## Shapes
- `tube`
- `branch`
- `loop`
- `vascular_tree`
- `cube` (single-test utility)

## Reported results
The CSV contains only:

`Shape, Size, Foreground, Iterations, Runtime_ms, Components, Euler, Endpoints`

Definitions:
- **Shape**: synthetic geometry used as input.
- **Size**: 3-D volume dimensions, e.g. `33x33x33`.
- **Foreground**: number of foreground voxels before thinning.
- **Iterations**: complete Lee iterations until convergence; one complete iteration contains U, B, N, S, W, E subiterations.
- **Runtime_ms**: execution time in milliseconds. For `--experiment`, this is the average of the requested repetitions.
- **Components**: 26-connected components before and after thinning, e.g. `1->1`.
- **Euler**: Euler characteristic before and after thinning, e.g. `0->0`.
- **Endpoints**: number of final skeleton voxels with fewer than two foreground neighbors in N26.

Internal counters for C1-C4 and scanning are still available in the implementation for debugging, but are not exported as report results.

## Compile
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp LeeThinning.cpp -o lee_thinning
```

## One test
```bash
./lee_thinning --single loop 33 5 full
```

## Full experiment
```bash
./lee_thinning --experiment results.csv 5 full
```

This runs each shape at several volume sizes. Each Shape + Size produces one CSV row; runtime is averaged across repetitions.
