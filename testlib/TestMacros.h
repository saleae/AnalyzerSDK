#ifndef SALEAE_ANALYZER_TEST_MACROS_H
#define SALEAE_ANALYZER_TEST_MACROS_H

#include <iostream>
#include <cassert>
#include <cstring> // for stcmp

#include "StreamHelpers.h"

#define TEST_VERIFY(a) \
    if ( !(a) )  { \
        std::cerr << "failed: " << #a << std::endl; \
        exit(1); \
    }

#define TEST_VERIFY_EQ(a, b) \
    if ( !((a) == (b)) )  { \
        std::cerr << "failed: " << #a << " == " << #b << std::endl; \
       std::cerr << "\tgot '" << (a) << "' and '" << (b) << "'" << std::endl; \
        std::cerr << "\tat " << __FILE__ << ":" << __LINE__ << std::endl; \
        exit(1); \
    }

#define TEST_EQ_EPSILON(a, b, ep) \
    if ( std::fabs((a) - (b)) >= ep )  { \
        std::cerr << "failed: " << #a << " == " << #b << " with epilson " << ep << std::endl; \
       std::cerr << "\tgot '" << (a) << "' and '" << (b) << "'" << std::endl; \
        std::cerr << "\tat " << __FILE__ << ":" << __LINE__ << std::endl; \
        exit(1); \
    }

#define TEST_VERIFY_EQ_CHARS(a, b) \
    if (strcmp(a, b)) { \
        std::cerr << "failed: " << #a << " == " << #b << std::endl; \
        std::cerr << "\tgot '" << (a) << "' and '" << (b) << "'" << std::endl; \
        std::cerr << "\tat " << __FILE__ << ":" << __LINE__ << std::endl; \
        exit(1); \
    }


#endif // of SALEAE_ANALYZER_TEST_MACROS_H
