#ifndef HAVE_TREEBEARD_LIB_COMMON_H
#define HAVE_TREEBEARD_LIB_COMMON_H

#ifdef NDEBUG
  #define TREEBEARD_ASSERT(x)
#else
  #include <cassert>
  #define TREEBEARD_ASSERT(x) assert(x)
#endif

#define TREEBEARD_STATIC_ASSERT(x) static_assert(x, #x)

#endif // inclusion guard
