#ifndef ANALYZER_TEST_MOCK_SIMULATED_CHANNEL_DESCRIPTOR_H
#define ANALYZER_TEST_MOCK_SIMULATED_CHANNEL_DESCRIPTOR_H

#include "SimulationChannelDescriptor.h"

namespace AnalyzerTest
{

class SimulatedChannel
{
public:
    SimulatedChannel();

    Channel GetChannel() const {
        return mChannel;
    }

    static SimulatedChannel* FromSimulatedChannelDescriptor(SimulationChannelDescriptor* sim);

    U64 GetSampleCount() const;

    void ResetToStart();

    BitState GetCurrentState() const;

    double GetDurationToNextTransition() const;

    bool AdvanceToNextTransition();

    double GetSampleDuration() const;

    U64 GetCurrentSample() const;
private:
    friend ::SimulationChannelDescriptor;

    Channel mChannel;
    U32 mSampleRateHz = 12000000;
    BitState mInitialBitState = BIT_LOW;

    U64 mCurrentSample = 0;
    BitState mCurrentState = BIT_LOW;
    U32 mTransitionIndex = 0;
    std::vector<U64> mTransitions; // absolute sample numbers of transitions
};

class SimulatedChannelGroup
{
public:
    SimulatedChannelGroup();

 private:
    friend ::SimulationChannelDescriptorGroup;

    U32 mSampleRateHz = 12000000;

    std::vector<SimulationChannelDescriptor> mChannels;
};

} // of namespace AnalyzerTest

#endif // of ANALYZER_TEST_MOCK_SIMULATED_CHANNEL_DESCRIPTOR_H
