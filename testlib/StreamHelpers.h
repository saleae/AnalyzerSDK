#ifndef SALEAE_ANALYZER_TEST_STREAM_HELPERS_H
#define SALEAE_ANALYZER_TEST_STREAM_HELPERS_H

#include <iostream>

class Channel;

std::ostream& operator<<(std::ostream& out, const Channel& chan);

#endif // of SALEAE_ANALYZER_TEST_STREAM_HELPERS_H
