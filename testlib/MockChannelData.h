#ifndef ANALYZER_TEST_MOCK_CHANNEL_DATA
#define ANALYZER_TEST_MOCK_CHANNEL_DATA

#include <exception>

#include "AnalyzerChannelData.h"
#include "TestInstance.h"

namespace AnalyzerTest
{

class MockChannelData : public AnalyzerChannelData
{
public:
    MockChannelData(Instance* pluginInstance);

    // test interface

    void TestSetInitialBitState(BitState bs);

    void TestAppendTransitionAfterSamples(U64 sampleCount);

    void TestAppendTransitionAtSamples(U64 sampleCount);

    void TestAppendTransitions(const std::vector<U64>& transitions);

    // base case ending with std::vec<double>
    double TestAppendIntervals(U64 sampleRateHz, double startingError, const std::vector<double>& intervals)
    {
        return InnerAppendIntervals(sampleRateHz, startingError, intervals);
    }

    // base case ending with a single double
    double TestAppendIntervals(U64 sampleRateHz, double startingError, double interval)
    {
        std::vector<double> i({interval,});
        return InnerAppendIntervals(sampleRateHz, startingError, i);
    }

    template<typename... Targs>
    double TestAppendIntervals(U64 sampleRateHz, double startingError, const std::vector<double>& intervals, Targs... Fargs)
    {
        const double err = InnerAppendIntervals(sampleRateHz, startingError, intervals); // base version
        return TestAppendIntervals(sampleRateHz, err, Fargs...); // expand the pack and recurse
    }

    template<typename... Targs>
    double TestAppendIntervals(U64 sampleRateHz, double startingError, double interval, Targs... Fargs)
    {
        std::vector<double> i({interval,});
        const double err = InnerAppendIntervals(sampleRateHz, startingError, i); // base version
        return TestAppendIntervals(sampleRateHz, err, Fargs...); // expand the pack and recurse
    }

    double TestAppendClockedState(U64 sampleRateHz, double startingError, double clockPeriodSec, BitState bs);

    void TestTransitionToState(BitState bs);

    void TestAdvance(U32 samples);

    void TestAdvanceTo(U64 exactSample);

    void ResetCurrentSample(U64 sampleNumber = 0);

    U32 AdvanceToSample(U64 sample); // returns number of times the value changed

    void DumpTestData(double sampleRateMhz);

    U64 GetCurrentSample() const
    { return mCurrentSample; }

    /**
     * @brief AdvanceNTransitions - advance a number of transitions in the data
     * (or run off the end)
     * @param transitionCount
     */
    void AdvanceNTransitions(U32 transitionCount);

    double TestAdvanceTime(U64 sampleRateHz, double currentError, double clockPeriodSec);
private:
    friend class ::AnalyzerChannelData;
    /**
     * @brief TestAppendIntervals - specify transition intervals in seconds (
     * or fractions thereof)
     * @param samleRateHz - time basis for conversion to samples
     * @param intervals
     */
    double InnerAppendIntervals(U64 sampleRateHz, double startingError, const std::vector<double>& intervals);

    /**
     * @brief CheckForCancellation - check the plugin instance for the cancellation
     *  flag, and throw CancellationException in that situation ,to get us out
     * out of analysis worker thread
     */
    void CheckForCancellation() const;

    BitState mCurrentState = BIT_LOW;
    U64 mCurrentSample = 0;

    BitState mInitialState = BIT_LOW
            ;
    // absolute sample numbers of transitions
    std::vector<U64> mTransitions;

    const Instance* mInstance = nullptr;
};


} // of namespace AnalyzerTest


#endif // of ANALYZER_TEST_MOCK_CHANNEL_DATA
