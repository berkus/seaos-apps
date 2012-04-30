  #undef TARGET_OS_CPP_BUILTINS
  #define TARGET_OS_CPP_BUILTINS()      \
    do {                                \
      builtin_define_std ("myos");      \
      builtin_define_std ("unix");      \
      builtin_assert ("system=myos");   \
      builtin_assert ("system=unix");   \
    } while(0);
