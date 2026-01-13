# ExpressionEvaluator Codebase Analysis

## Overview
- Reverse Polish Notation (RPN) expression evaluator written in C++14-ish style. Core logic lives in [include/ExpressionEvaluator.h](include/ExpressionEvaluator.h), with tokenization in [include/Tokenizer.h](include/Tokenizer.h) and user-defined function wrappers in [include/DefinedFunction.h](include/DefinedFunction.h).
- Tests are Catch2-based via [ExpressionEvaluator_TestRunner.cpp](ExpressionEvaluator_TestRunner.cpp) and suites under [tests/](tests). A small CLI example sits in [src/example_app.cpp](src/example_app.cpp). A Qt stub exists in [QtFrontend/](QtFrontend), not wired into CMake.

## Architecture & Behavior
- **Tokenization**: `Tokenizer` converts an input stream into `Token` objects, recognizing numbers, names, built-in operators, parentheses, comma, and `=`. It distinguishes function names via a lookup provided by `ExpressionEvaluator`.
- **Parsing**: `ExpressionEvaluator::transformToRPN` implements a Shunting-Yard algorithm to produce RPN, handling operator precedence and associativity and detecting some syntax errors through `checkTransition`.
- **Evaluation**: `evaluateRPN` consumes the RPN stack to compute a `double`, executing arithmetic ops and dispatching user-defined functions registered in `_defFunc`. Built-ins (trig, exp/log, hyperbolic, gamma) are registered at construction time.
- **API surface**: Primary entry points are `driver` (returns string or error text) and `evaluate` overloads (return `double` and set `CalculatorStatus`). Users can add functions via `addUserDefinedFunction` by providing `DefinedFunctionOneParam`/`TwoParam`.

## Test Coverage Snapshot
- Arithmetic precedence, parentheses, power operator, built-in functions: [tests/BasicExpressionsTests.cpp](tests/BasicExpressionsTests.cpp).
- Tokenization edge cases: [tests/TokenizerTests.cpp](tests/TokenizerTests.cpp).
- Unary minus handling: [tests/UnaryMinusTests.cpp](tests/UnaryMinusTests.cpp).
- User-defined functions (1-param): [tests/AddingUserDefFunctionsTests.cpp](tests/AddingUserDefFunctionsTests.cpp); (2-param): [tests/MultiParameterFunctionsTests.cpp](tests/MultiParameterFunctionsTests.cpp).
- Error reporting for syntax/parentheses: [tests/ErrorsTests.cpp](tests/ErrorsTests.cpp).

## Strengths
- Shunting-Yard approach keeps parsing deterministic and is easy to extend with more operators.
- Built-in library initialization covers a wide math subset, and extensibility for user functions exists.
- Test suite is reasonably broad for arithmetic, unary minus, function calls, and some error paths; CMake integrates tests and example app out of the box.

## Issues & Risks
- **Memory management**: Built-in and user-defined functions are allocated with `new` and never released; `ExpressionEvaluator` lacks a destructor/ownership policy. Repeated calls to `addUserDefinedFunction` leak and may overwrite entries without cleanup ([include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
- **Error handling gaps**:
  - `transformToRPN` reads `stack.top()` for comma handling without empty-check; malformed input could dereference an empty stack ([include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
  - `evaluateRPN` assumes sufficient operands before unary minus/function application and does not set an error if a function name is unknown; it silently skips evaluation, leaving stack artifacts ([include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
  - Division by zero, domain errors (e.g., `sqrt(-1)`), and overflow are not trapped; `CalculatorStatus` never reports runtime math faults ([include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
- **Syntax validation limitations**: `checkTransition` accepts any bare `name` as a value but there is no variable binding or assignment handling, even though tokens and statuses imply equation support. Names are pushed to the eval stack and never resolved, yielding undefined behavior for expressions containing variables ([include/Tokenizer.h](include/Tokenizer.h), [include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
- **Function arity**: Only 1- and 2-parameter functions are implemented; 3-parameter path is stubbed and `_numParam` is not validated against call-site commas ([include/DefinedFunction.h](include/DefinedFunction.h), [include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
- **Numeric formats**: Tokenizer lacks scientific notation (E/e) support, called out in TODO.txt; numeric parsing stops at whitespace but not at trailing invalid characters ([include/Tokenizer.h](include/Tokenizer.h), [TODO.txt](TODO.txt)).
- **API ergonomics**: `driver` converts results with `std::to_string`, producing many trailing zeros; no locale/precision control. Status messages are fixed English strings without context (positioning, offending token).
- **Thread safety**: Mutable internal maps and raw pointers make instances non-thread-safe; concurrent use could race on `_defFunc` without guards ([include/ExpressionEvaluator.h](include/ExpressionEvaluator.h)).
- **Qt frontend**: Exists but is not included in the CMake build; likely stale ([QtFrontend/untitled](QtFrontend/untitled)).

## Opportunities / Next Steps
1. Add ownership semantics (smart pointers or value wrappers) for `_defFunc` and provide cleanup in a destructor; guard duplicate registrations.
2. Harden parsing: validate stack sizes before access, handle unknown functions as errors, and extend `CalculatorStatus` to cover runtime math faults (div-by-zero, domain errors).
3. Implement variable binding or remove `name` token acceptance from expressions that are not function calls; alternatively, add a symbol table and assignment handling to align with existing token types/status enums.
4. Support scientific notation and tighter numeric parsing; expand tests accordingly (TODO item).
5. Enforce function arity during RPN conversion (track argument counts) and implement 3+ parameter support if needed.
6. Improve diagnostics with position info and clearer messages; expose precision control for `driver` output.
7. Integrate Qt frontend or remove it from the tree; consider packaging a single-header-only distribution from [single_header/ExpressionEvaluator.h](single_header/ExpressionEvaluator.h) if that is intended for consumers.
8. Add tests for runtime faults (division by zero), unknown identifiers, malformed commas, and function arity mismatches.

## Build & Run
- CMake targets: `ExpressionEvaluator_ExampleApp` (CLI), `ExpressionEvaluatorTests` (Catch2). Include path is `include/`; Catch header is vendored in [catch/catch.hpp](catch/catch.hpp).
- Run tests: `ctest` after configuring, or execute `ExpressionEvaluatorTests` directly.

## Open TODOs from repo
- Expand test suite; implement scientific notation; broaden math function coverage; strengthen error handling and user-facing diagnostics; explore Qt-based UI (see [TODO.txt](TODO.txt)).
