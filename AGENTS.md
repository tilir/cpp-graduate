# AGENTS.md

Code samples for the C++ graduate course (iLab at MIPT). Each `NN-topic/`
directory contains standalone examples for one lecture; some directories
(`01-basics/lru-*`, `07-parsers/*`, `15-llvm/naive-pcl`) are subprojects with
their own build logic.

## Toolchain

The repository is built with:

- `g++-15` / `gcc-15` and `clang++-21` / `clang-21` (both must pass)
- cmake, bison, flex and its C++ headers (`libfl-dev` on Debian/Ubuntu), ruby (test drivers)
- LLVM 21.1 dev package (for `15-llvm/naive-pcl`)
- OpenCL headers, Vulkan/GLM/GLFW headers, google benchmark (optional
  examples are skipped by cmake if headers/libraries are missing)

## Build and test

Everything is driven by the root `CMakeLists.txt`:

```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-15 -DCMAKE_C_COMPILER=gcc-15
cmake --build build -j
ctest --test-dir build --output-on-failure
```

`CMakePresets.json` defines `gcc15` and `clang21` configure/build/test
presets. GitHub Actions runs them in separate matrix jobs
(see `.github/workflows/ci.yml`). Test drivers live in `ci/*.rb` — write new
ones in Ruby, not bash, for portability.

Standalone examples are compiled as `OBJECT` library targets: they must
compile, but are not required to link or run. The default standard is
C++20 (`CMAKE_CXX_STANDARD`).

## Style rules

- Include order: project headers (`#include "..."`) come **before**
  standard/system headers (`#include <...>`). Enforced by the ctest test
  `include-order` (`ci/check_include_order.rb`). If an include genuinely
  must stay late (e.g. a generated bison header requiring `YYSTYPE`),
  annotate the line with `// include-order: late`.
- When the CL headers require configuration macros
  (`CL_HPP_TARGET_OPENCL_VERSION` etc.), the macro block travels together
  with the quoted CL include to the top of the include list.

## IWYU

`include-what-you-use` is wired into the build:

```
cmake -B build-iwyu -DENABLE_IWYU=ON
cmake --build build-iwyu -j
```

This runs IWYU (with `--quoted_includes_first`, matching the include-order
rule above) on every translation unit; its diagnostics are advisory and part of the build
log. Use IWYU 0.25 with Clang/LLVM 21. See `.github/workflows/ci.yml` for the CI job.

## Intentional compile-error demos

Some examples demonstrate compile errors on purpose and are excluded from
the build (`INTENTIONAL_ERRORS` in `CMakeLists.txt`), e.g.
`03-concrete/ctors-ambig.cc` (most vexing parse), `18-constexpr/unruh.cc`
(compile-time primes in error messages). Do not "fix" them, and do not add
them back to the build.

## Notes

- `05-opers/binaryop-resolved.cc` is built with `-DNOFAIL` (the only
  configuration that is meant to compile).
- `15-llvm/tests-pcl/fib/file.in` (250) is a benchmark input; `file.out`
  corresponds to input 25 — the ctest test uses 25.
- `archive/` is legacy material and is not built.

## Formatting

Use clang-format 21 with the root `.clang-format` configuration:

```
cmake --build build --target format        # fix formatting
cmake --build build --target format-check  # check only
ctest --test-dir build -L style --output-on-failure
```

The targets and CTest check are available when Ruby and clang-format-21 are
installed. `-DENABLE_FORMAT_CHECK=ON` makes them mandatory (used in CI).
Archive, generated/build files and intentional compile-error demos are excluded
from formatting. Quoted includes stay first, with include groups preserved for
OpenCL configuration macros and late generated headers.
