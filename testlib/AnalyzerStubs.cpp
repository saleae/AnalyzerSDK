#include "Analyzer.h"

#include <vector>
#include <memory>
#include <map>
#include <iostream>

#include "MockChannelData.h"
#include "AnalyzerResults.h"
#include "TestAnalyzerData.h"


#define D_PTR() \
     auto d = reinterpret_cast<AnalyzerTest::AnalyzerData*>(mData);

Analyzer::Analyzer()
{
    auto ourData = new AnalyzerTest::AnalyzerData{};
    mData = reinterpret_cast<struct AnalyzerData*>(ourData);
}

Analyzer::~Analyzer()
{
    D_PTR();
    delete d;
}

Analyzer2::Analyzer2()
{
}

void Analyzer2::SetupResults()
{

}

U64 Analyzer::GetTriggerSample()
{
    return 2;
}

void Analyzer::CheckIfThreadShouldExit()
{
    // record soemthing here?
}

U32 Analyzer::GetSampleRate()
{
    D_PTR();
    return d->sampleRateHz;
}


U32 Analyzer::GetSimulationSampleRate()
{
    D_PTR();
    return d->simulationRateHz;
}


AnalyzerChannelData* Analyzer::GetAnalyzerChannelData(Channel &channel)
{
    D_PTR();
    auto it = d->channelData.find(channel);
    if (it == d->channelData.end()) {
        std::cerr << "Couldn't find channel data for " << channel.mDeviceId << "/" << channel.mChannelIndex << std::endl;
        return nullptr;
    }

    return it->second;
}

void Analyzer::ReportProgress( U64 sample_number )
{
    // store progress value in data? Or report directly?
}

void Analyzer::SetAnalyzerResults( AnalyzerResults* results )
{
    D_PTR();
    d->results = results;
}

void Analyzer::SetAnalyzerSettings(AnalyzerSettings *settings)
{
    D_PTR();
    d->settings = settings;
}

void Analyzer::KillThread()
{

}

////////////////////////////////////////////////////////////////////////////


Channel::Channel()
    : mDeviceId(0)
    , mChannelIndex(0)
    , mDataType(DIGITAL_CHANNEL)
{
}

Channel::Channel(const Channel &channel)
    : mDeviceId(channel.mDeviceId)
    , mChannelIndex(channel.mChannelIndex)
    , mDataType(channel.mDataType)
{

}

Channel::Channel(U64 device_id, U32 channel_index, ChannelDataType data_type)
    : mDeviceId(device_id)
    , mChannelIndex(channel_index)
    , mDataType(data_type)
{
}

Channel::~Channel()
{
}

Channel &Channel::operator=(const Channel &channel)
{
    mDeviceId = channel.mDeviceId;
    mChannelIndex = channel.mChannelIndex;
    mDataType = channel.mDataType;
    return *this;
}

bool Channel::operator==(const Channel &channel) const
{
    return (mDeviceId == channel.mDeviceId) &&
            (mChannelIndex == channel.mChannelIndex) &&
            (mDataType == channel.mDataType);
}

bool Channel::operator!=(const Channel &channel) const
{
    return !(*this == channel);
}

bool Channel::operator<(const Channel &channel) const
{
    if (mDeviceId != channel.mDeviceId)
        return mDeviceId < channel.mDeviceId;

    if (mChannelIndex != channel.mChannelIndex)
        return mChannelIndex < channel.mChannelIndex;

    return mDataType < channel.mDataType;
}

////////////////////////////////////////////////////////////////////////////

namespace AnalyzerTest
{

class DataExtractor : public Analyzer2 {
public:
    AnalyzerData* testData() {
        return reinterpret_cast<AnalyzerData*>(mData);
    }
};

AnalyzerData* GetDataFromAnalyzer(Analyzer* instance)
{
    return static_cast<DataExtractor*>(instance)->testData();
}

AnalyzerResults* GetResultsFromAnalyzer(Analyzer* instance)
{
    return static_cast<DataExtractor*>(instance)->testData()->results;
}

AnalyzerData::~AnalyzerData()
{
}

} // of namespace AnalyzerTest
