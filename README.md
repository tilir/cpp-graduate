# cpp-graduate
Code samples for C++ graduate course (iLab at MIPT)

Slides can be found here: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/

Follow the lectures on [Telegram](https://t.me/cpp_lects_rus) and the
[YouTube channel](https://www.youtube.com/@tilir). Course recordings are available
in the [C++ graduate course playlist](https://www.youtube.com/playlist?list=PL3BR09unfgciJ1_K_E914nohpiOiHnpsK).

Build with CMake 3.20+, Ninja, GCC 15 and Clang 21. Install Bison, Flex,
Ruby and Python 3 for the parser and LRU tests. Flex C++ development headers
are also required (`libfl-dev` on Debian/Ubuntu provides `FlexLexer.h`).
LLVM 21.1 development files
enable the ParaCL compiler/interpreter. Google Benchmark, OpenCL C/C++ headers,
Vulkan, GLM and GLFW enable optional examples; CMake reports missing dependencies.
The three examples requiring the unvendored GLAD loader are skipped.

Run either compiler preset (CI runs both):

```sh
cmake --preset gcc15
cmake --build --preset gcc15 --parallel
ctest --preset gcc15 --parallel 2

cmake --preset clang21
cmake --build --preset clang21 --parallel
ctest --preset clang21 --parallel 2
```

Pass `-DLLVM_DIR=/usr/lib/llvm-21/lib/cmake/llvm` when CMake cannot locate LLVM.
CI uses `-DREQUIRE_LLVM=ON` to prevent silently skipping ParaCL.
Standalone lecture examples compile as object libraries. Intentional compile-error
demos and `archive/` are excluded. C variants of LRU build in the binary directory,
and their existing Python and component tests run directly through CTest.

Install **clang-format-21** for formatting targets and the CTest style check:

```sh
cmake --build --preset gcc15 --target format        # fix formatting
cmake --build --preset gcc15 --target format-check  # check only
ctest --test-dir build/gcc15 -L style --output-on-failure
```

Reconfigure after installing the formatter. `-DENABLE_FORMAT_CHECK=ON` requires
it at configuration time (used in CI). The shared file selection excludes
archives and build files; formatting also excludes intentional compile-error
demos. Quoted includes precede system includes. Include groups stay separate
to preserve OpenCL macros and generated headers that require declarations first.

For include suggestions, use CMake 3.27+ and IWYU 0.25 built against Clang 21, following the
[upstream compatibility table](https://github.com/include-what-you-use/include-what-you-use#clang-compatibility):

```sh
cmake -S . -B build-iwyu -DCMAKE_C_COMPILER=clang-21 \
  -DCMAKE_CXX_COMPILER=clang++-21 -DENABLE_IWYU=ON \
  -DIWYU_EXECUTABLE=/path/to/include-what-you-use
cmake --build build-iwyu --parallel 2
```

IWYU checks lecture sources and their project headers. Generated Bison/Flex
sources are excluded with `SKIP_LINTING`; `ci/iwyu*.imp` maps implementation
headers to public library APIs. The parser wrapper exports its generated header,
which requires `YYSTYPE` to be declared first.

IWYU suggestions are advisory and uploaded as `iwyu.log` in CI. Build failures,
runtime tests, include order and clang-format checks fail the workflow.

Some build warnings are intentional lecture demonstrations: mismatched `new`/`delete`
in `02-lrefs/newdelete.cc`, hidden overloads in `06-inheritance/namehide.cc`,
the incomplete shape switch in `09-solid/ocp-bad.cc`, catch ordering in
`08-exceptions/throws.cc`, deprecated implicit `this` capture in
`17-algorithms/capture-body*.cc`, and ambiguous expressions in
`07-parsers/numgrammar/numgrammar-sr.y`. Keep these examples and their warnings.
