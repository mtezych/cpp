
#ifndef CXX_CONTRACTS
#define CXX_CONTRACTS


#include <cassert>


#if   defined(_MSC_VER)

    // [MSVC] - __assume
    // - https://docs.microsoft.com/en-us/cpp/intrinsics/assume
    #define cxx_assume(axiom) __assume(axiom)

#elif defined(__clang__)

    // [Clang] - Language Extensions
    // - https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume
    #define cxx_assume(axiom) __builtin_assume(axiom)

#elif defined(__GNUC__)

    // [GCC] - Other Built-in Functions
    // - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
    #define cxx_assume(axiom) ((axiom) ? void() : __builtin_unreachable())

#else
    static_assert(false, "C++ compiler is not supported!");
#endif


#ifdef NDEBUG
    #define cxx_expects(axiom) cxx_assume(axiom)
#else
    #define cxx_expects(axiom)     assert(axiom)
#endif


#endif
