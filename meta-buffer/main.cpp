#include <cstdio>
#if (__GNUC__ == 4 && __GNUC_MINOR__ > 7) || (__GNUC__ > 4) || \
    (__clang_major__ > 2 && __clang_minor__ > 3) || (__clang_major__ > 3) ||\
    (_MSC_VER > 1912)
void run_all_tests();
#else
#define __GCC4_7_GOD_FORBID
#warning Your C++ compiler is not fully c++11 complaint it thinks
#endif
void cpp_ver_out();
void probs();

int main(int, char**)
{
#ifdef __GCC4_7_GOD_FORBID
#warning Nothing can be done using it
#else
    probs();
#endif
    cpp_ver_out();
}
void cpp_ver_out()
{
    printf(
#ifdef CPP_14
    "\nC++14, "
#elif defined(CPP_11)
    "\nC++11, "
#elif _MSC_VER
    "\nMS VC++, "
#endif
    "%ld %s\n", __cplusplus,
#ifdef  __clang__
     "CLANG"
#elif _MSC_VER
    "MS VC++, "
#else
     "GXX"
#endif
            );
}
