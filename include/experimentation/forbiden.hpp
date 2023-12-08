#if defined(__GNUC__)
#pragma GCC poison malloc realloc calloc free new
#elif defined(__clang__)
#pragma Clang poison malloc realloc calloc free new
#else
#error "Compiler not supported"
#endif
