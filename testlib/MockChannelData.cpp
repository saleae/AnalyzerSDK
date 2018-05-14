#include "MockChannelData.h"

#include <cassert>
#include <algorithm>
#include <exception>
#include <cmath>

// REMOVE ME
#include <iostream>

namespace AnalyzerTest
{

BitState InvertBitState(BitState b)
{
    if (b == BIT_HIGH) return BIT_LOW;
    return BIT_HIGH;
}

MockChannelData::MockChannelData(Instance* plugin) :
    AnalyzerChannelData(nullptr),
    mInstance(plugin)
{
    assert(plugin);
}

void MockChannelData::TestSetInitialBitState(BitState bs)
{
    assert(mTransitions.empty());
    mInitialState = bs;
    mCurrentState = mInitialState;
    // insert a dummy sample before the real data, so that
    // the logic in AdvanceToSample is simpler
    mTransitions.push_back(0);
}

void MockChannelData::TestAppendTransitionAfterSamples(U64 sampleCount)
{
    assert(sampleCount > 0);
    TestAppendTransitionAtSamples(mCurrentSample + sampleCount);
}

void MockChannelData::TestAppendTransitionAtSamples(U64 sample)
{
    assert(sample > mCurrentSample);
    mTransitions.push_back(sample);
    mCurrentSample = mTransitions.back();
    mCurrentState = InvertBitState(mCurrentState);
}

void MockChannelData::TestAppendTransitions(const std::vector<U64> &transitions)
{
    for (auto t : transitions) {
        TestAppendTransitionAfterSamples(t);
    }
}

double MockChannelData::TestAppendClockedState(U64 sampleRateHz, double currentError, double clockPeriodSec, BitState bs)
{
    if (mCurrentState != bs) {
        if (mTransitions.back() == mCurrentSample) {
            // squash together, we actually remove a transition here
            mTransitions.pop_back();
        } else {
            mTransitions.push_back(mCurrentSample);
        }
        mCurrentState = InvertBitState(mCurrentState);
    }

    return TestAdvanceTime(sampleRateHz, currentError, clockPeriodSec);
}

double MockChannelData::TestAdvanceTime(U64 sampleRateHz, double currentError, double clockPeriodSec)
{
    const double sampleDuration = 1.0 / sampleRateHz;
    double nominalSamples = (clockPeriodSec / sampleDuration) + currentError;
    const U32 samplesToAdvance = std::lround(nominalSamples);
    currentError = nominalSamples - static_cast<double>(samplesToAdvance);
    mCurrentSample += samplesToAdvance;
    return currentError;
}

void MockChannelData::TestTransitionToState(BitState bs)
{
    if (bs == mCurrentState)
        return;

    if (mTransitions.back() == mCurrentSample) {
        std::cerr << "TestTransitionToState without advancing from last transition" << std::endl;
    }
    mTransitions.push_back(mCurrentSample);
    mCurrentState = bs;
}

void MockChannelData::TestAdvance(U32 samples)
{
    mCurrentSample += samples;
}

void MockChannelData::TestAdvanceTo(U64 exactSample)
{
    assert(mCurrentSample <= exactSample);
    mCurrentSample = exactSample;
}

void MockChannelData::ResetCurrentSample(U64 sampleNumber)
{
    mCurrentState = mInitialState;
    mCurrentSample = 0;
    AdvanceToSample(sampleNumber);
}

U32 MockChannelData::AdvanceToSample(U64 sample)
{
    assert(sample >= mCurrentSample);
    if (sample == mCurrentSample)
        return 0;

    auto cur = std::upper_bound(mTransitions.begin(), mTransitions.end(), mCurrentSample) - 1;
    if (cur == mTransitions.end()) {
        throw OutOfDataException();
    }

    assert (*cur <= mCurrentSample);

    auto it = std::upper_bound(mTransitions.begin(), mTransitions.end(), sample) - 1;
    if (it == mTransitions.end()) {
        throw OutOfDataException();
    }

    assert (*it <= sample);

    if (it == cur) {
        // no transitions between current and requested sample
        // short circuit to avoud complicating the logic below
        mCurrentSample = sample;
        return 0;
    }

    // count the distance between the two
    U32 transitionCount = std::distance(cur, it);
    bool oddTransitionCount = transitionCount % 2;
    if (oddTransitionCount) {
        mCurrentState = InvertBitState(mCurrentState);
    }

    mCurrentSample = sample;
    return transitionCount;
}

double MockChannelData::InnerAppendIntervals(U64 sampleRateHz, double startingError, const std::vector<double> &intervals)
{
    // same logic as built-in ClockGenerator, track accumulated error to ensure
    // we don't consistently under-sample by rounding down

    const double sampleDuration = 1.0 / sampleRateHz;
    double currentError = startingError;

    for (auto i : intervals) {
        double nominalSamples = (i / sampleDuration) + currentError;
        // testing using lround here rather that simple cast to int
     //   const U32 samplesToAdvance = static_cast<U32>(nominalSamples);
        const U32 samplesToAdvance = std::lround(nominalSamples);
        currentError = nominalSamples - static_cast<double>(samplesToAdvance);
        TestAppendTransitionAfterSamples(samplesToAdvance);
    }

    return currentError;
}

void MockChannelData::CheckForCancellation() const
{
    if (mInstance->CheckCancellation()) {
        throw CancellationException{};
    }
}

void MockChannelData::DumpTestData(double sampleRateMhz)
{
    double sampleDuration = 1.0 / sampleRateMhz;
    int frame = 0;
    U64 previous = 0;
    for (int i=0; i<mTransitions.size(); ++i) {
        if ((i % 48 )== 0) {
            std::cout << "==" << frame++ << "===================" << std::endl;
        }

        U64 interval = mTransitions.at(i) - previous;
        std::cout << interval << " " << interval * sampleDuration << std::endl;
        previous = mTransitions.at(i);
    }
}

void MockChannelData::AdvanceNTransitions(U32 transitionCount)
{
    while (transitionCount-- > 0) {
        AdvanceToNextEdge();
    }
}

} // of namespace AnalyzerTest


/////////////////////////////////////////////////////////////////////////////

#define D_PTR() \
    auto * d = reinterpret_cast<AnalyzerTest::MockChannelData*>(this);

AnalyzerChannelData::AnalyzerChannelData(ChannelData *channel_data)
{
}

AnalyzerChannelData::~AnalyzerChannelData()
{

}

U64 AnalyzerChannelData::GetSampleNumber()
{
    D_PTR();
    return d->mCurrentSample;
}

BitState AnalyzerChannelData::GetBitState()
{
    D_PTR();
    return d->mCurrentState;
}

U32 AnalyzerChannelData::Advance(U32 num_samples)
{
    D_PTR();
    return d->AdvanceToSample(d->mCurrentSample + num_samples);
}

U32 AnalyzerChannelData::AdvanceToAbsPosition(U64 sample_number)
{
    D_PTR();
    return d->AdvanceToSample(sample_number);
}

void AnalyzerChannelData::AdvanceToNextEdge()
{
    D_PTR();
    U64 nextEdge = GetSampleOfNextEdge();
 //   std::cerr << "AdvNE: advanced to " << nextEdge << " from " << d->mCurrentSample << std::endl;
    d->mCurrentState = AnalyzerTest::InvertBitState(d->mCurrentState);
    d->mCurrentSample = nextEdge;
}

U64 AnalyzerChannelData::GetSampleOfNextEdge()
{
    D_PTR();
    auto next = std::lower_bound(d->mTransitions.begin(), d->mTransitions.end(), d->mCurrentSample);
    if (next == d->mTransitions.end()) {
        throw AnalyzerTest::OutOfDataException();
    }

    if (*next == d->mCurrentSample) {
        // if we're exactly on the sample, advance
        ++next;
        if (next == d->mTransitions.end()) {
            throw AnalyzerTest::OutOfDataException();
        }
    }

    return *next;
}

bool AnalyzerChannelData::WouldAdvancingCauseTransition(U32 num_samples)
{
    D_PTR();
    return WouldAdvancingToAbsPositionCauseTransition(d->mCurrentSample + num_samples);
}

bool AnalyzerChannelData::WouldAdvancingToAbsPositionCauseTransition(U64 sample_number)
{
    D_PTR();
    auto next = std::lower_bound(d->mTransitions.begin(), d->mTransitions.end(), d->mCurrentSample);
    if (next == d->mTransitions.end()) {
        return false;
    }

    if (*next == d->mCurrentSample) {
        ++next;
        if (next == d->mTransitions.end()) {
            return false;
        }
    }

    return (*next <= sample_number);
}
