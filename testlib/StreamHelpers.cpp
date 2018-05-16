#include "StreamHelpers.h"

#include <Analyzer.h>

std::ostream &operator<<(std::ostream &out, const Channel &chan)
{
    out << "Channel{" << chan.mDeviceId << "/" << chan.mChannelIndex << " ";
    if (chan.mDataType == ANALOG_CHANNEL) {
        out << "A}";
    } else {
        out << "D}";
    }
    return out;
}
