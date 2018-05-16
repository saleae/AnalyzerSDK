#include "MockSimulatedChannelDescriptor.h"

namespace {
    class DataExtractor : public SimulationChannelDescriptor {
    public:
        AnalyzerTest::SimulatedChannel* testData() {
            return reinterpret_cast<AnalyzerTest::SimulatedChannel*>(mData);
        }
    };

    BitState InvertBitState(BitState b)
    {
        if (b == BIT_HIGH) return BIT_LOW;
        return BIT_HIGH;
    }

}

namespace AnalyzerTest
{

SimulatedChannel::SimulatedChannel()
{

}

SimulatedChannel *SimulatedChannel::FromSimulatedChannelDescriptor(SimulationChannelDescriptor *sim)
{
    return static_cast<DataExtractor*>(sim)->testData();
}

U64 SimulatedChannel::GetSampleCount() const
{
    return mCurrentSample;
}

void SimulatedChannel::ResetToStart()
{
    mCurrentState = mInitialBitState;
    mCurrentSample = 0;
    mTransitionIndex = 0;
}

BitState SimulatedChannel::GetCurrentState() const
{
    return mCurrentState;
}

double SimulatedChannel::GetDurationToNextTransition() const
{
    U32 samples = mTransitions.at(mTransitionIndex) - mCurrentSample;
    if (samples == 0)
        return 0.0;

    return samples * GetSampleDuration();
}

bool SimulatedChannel::AdvanceToNextTransition()
{
    if (mTransitionIndex >= (mTransitions.size() - 1)) {
        return false;
    }

    mCurrentSample = mTransitions.at(mTransitionIndex++);
    mCurrentState = InvertBitState(mCurrentState);
    return true;
}

double SimulatedChannel::GetSampleDuration() const
{
    return 1.0 / mSampleRateHz;
}

U64 SimulatedChannel::GetCurrentSample() const
{
    return mCurrentSample;
}

SimulatedChannelGroup::SimulatedChannelGroup()
{
}

} // of namespace AnalyzerTest

#define D_PTR() \
    auto d = reinterpret_cast<AnalyzerTest::SimulatedChannel*>(mData);


SimulationChannelDescriptor::SimulationChannelDescriptor()
{
    auto d = new AnalyzerTest::SimulatedChannel();
    mData = reinterpret_cast<struct SimulationChannelDescriptorData*>(d);
}

SimulationChannelDescriptor::SimulationChannelDescriptor(const SimulationChannelDescriptor &other)
{
    auto d = new AnalyzerTest::SimulatedChannel();
    mData = reinterpret_cast<struct SimulationChannelDescriptorData*>(d);

    auto otherD = reinterpret_cast<AnalyzerTest::SimulatedChannel*>(other.mData);
    d->mChannel = otherD->mChannel;
    d->mSampleRateHz = otherD->mSampleRateHz;
    d->mInitialBitState = otherD->mInitialBitState;
}

SimulationChannelDescriptor::~SimulationChannelDescriptor()
{
    D_PTR();
    delete d; // ensure we delete as the correct type
}

void SimulationChannelDescriptor::SetChannel(Channel &channel)
{
    D_PTR();
    d->mChannel = channel;
}

void SimulationChannelDescriptor::SetSampleRate(U32 sample_rate_hz)
{
    D_PTR();
    d->mSampleRateHz = sample_rate_hz;
}

void SimulationChannelDescriptor::SetInitialBitState(BitState initial_bit_state)
{
    D_PTR();
    d->mInitialBitState = initial_bit_state;
    d->mCurrentState = initial_bit_state;
}

Channel SimulationChannelDescriptor::GetChannel()
{
    D_PTR();
    return d->mChannel;
}

void SimulationChannelDescriptor::Transition()
{
    D_PTR();
    d->mTransitions.push_back(GetCurrentSampleNumber());
    d->mCurrentState = InvertBitState(d->mCurrentState);
}

void SimulationChannelDescriptor::TransitionIfNeeded(BitState bit_state)
{
    D_PTR();
    if (d->mCurrentState != bit_state) {
        Transition();
    }
}

void SimulationChannelDescriptor::Advance(U32 num_samples_to_advance)
{
    D_PTR();
    d->mCurrentSample += num_samples_to_advance;
}

BitState SimulationChannelDescriptor::GetCurrentBitState()
{
    D_PTR();
    return BIT_LOW;
}

U64 SimulationChannelDescriptor::GetCurrentSampleNumber()
{
    D_PTR();
    return d->mCurrentSample;
}

////////////////////////////////////////////////////////////////////////////////

#undef D_PTR
#define D_PTR() \
    auto d = reinterpret_cast<AnalyzerTest::SimulatedChannelGroup*>(mData);

SimulationChannelDescriptorGroup::SimulationChannelDescriptorGroup()
{
    auto d = new AnalyzerTest::SimulatedChannelGroup();
    mData = reinterpret_cast<struct SimulationChannelDescriptorGroupData*>(d);
}

SimulationChannelDescriptorGroup::~SimulationChannelDescriptorGroup()
{
    D_PTR();
    delete d;
}

SimulationChannelDescriptor *SimulationChannelDescriptorGroup::Add(Channel &channel, U32 sample_rate, BitState intial_bit_state)
{
    D_PTR();
    SimulationChannelDescriptor simChan;
    simChan.SetChannel(channel);
    simChan.SetInitialBitState(intial_bit_state);
    simChan.SetSampleRate(sample_rate);
    d->mChannels.push_back(simChan);
    return &d->mChannels.back();
}

void SimulationChannelDescriptorGroup::AdvanceAll(U32 num_samples_to_advance)
{
    // implement me
}

SimulationChannelDescriptor *SimulationChannelDescriptorGroup::GetArray()
{
    D_PTR();
    return d->mChannels.data();
}

U32 SimulationChannelDescriptorGroup::GetCount()
{
    D_PTR();
    return d->mChannels.size();
}


