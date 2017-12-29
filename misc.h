#ifndef MISC_H
#define MISC_H

#include <sys/stat.h>
#include <type_traits>
#include <QDebug>
#include <QString>

// logging
#define DEBUG_LOG

// easy functions
#define Len(a) (int)(sizeof(a) / sizeof(*a))
#define For(x, y) for (typename std::remove_cv<typename std::remove_reference<decltype(y)>::type>::type x = 0;  x < (y); ++x)
#define LastChar(str) (str[str.length() - 1])
#define ValidIndex(i, max_i) (i >= 0 && i < max_i)

#endif // MISC_H
