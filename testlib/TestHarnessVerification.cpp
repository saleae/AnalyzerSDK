#include "MockChannelData.h"
#include "TestMacros.h"

#include <iostream>

using namespace AnalyzerTest;

namespace {

double operator "" _ns( unsigned long long x )
{
    return x * 1e-9;
}

double operator "" _us( unsigned long long x )
{
    return x * 1e-6;
}

} // of anonymous namespace

void verifyMockChannelData()
{
    Instance plugin;

    const int sampleRateHz = 20000000; // 20MHz

    MockChannelData channelData(&plugin);
    channelData.TestSetInitialBitState(BIT_LOW);
    channelData.TestAppendIntervals(sampleRateHz, 0.0,
        {150_ns, 200_ns, 250_ns, 170_ns, 225_ns, 275_ns, 300_ns, 120_ns});

    channelData.ResetCurrentSample();

    TEST_VERIFY(channelData.WouldAdvancingCauseTransition(2) == false);

    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 3);
    channelData.AdvanceToNextEdge();
    TEST_VERIFY_EQ(channelData.GetSampleNumber(), 3);

    channelData.AdvanceToAbsPosition(6);
    TEST_VERIFY_EQ(channelData.GetBitState(), BIT_HIGH);

    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 7);
    channelData.AdvanceToAbsPosition(7);
    TEST_VERIFY_EQ(channelData.GetBitState(), BIT_LOW);

    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 12);
    channelData.Advance(5);
    TEST_VERIFY_EQ(channelData.GetBitState(), BIT_HIGH);

    // next pulse is 170ns which doesn't exactly map to 20MHz rate
    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 15);
    channelData.AdvanceToNextEdge();

    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 20);
    channelData.AdvanceToNextEdge();

    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 25);
    channelData.AdvanceToNextEdge();

    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 31);

    // should round up due to accumulated error
    channelData.AdvanceToNextEdge();
    TEST_VERIFY_EQ(channelData.GetSampleOfNextEdge(), 34);
}

void verifyMockChannelData2()
{
    Instance plugin;

    const int sampleRateHz = 12000000; // 12MHz

    MockChannelData channelData(&plugin);
    channelData.TestSetInitialBitState(BIT_HIGH);
    channelData.TestAdvance(5);
    channelData.TestTransitionToState(BIT_HIGH);
    channelData.TestAdvance(6);
    channelData.TestTransitionToState(BIT_LOW);
    channelData.TestAdvance(8);

    channelData.ResetCurrentSample();

    TEST_VERIFY(channelData.WouldAdvancingCauseTransition(2) == false);
    TEST_VERIFY(channelData.GetSampleOfNextEdge() == 11);
    channelData.AdvanceToNextEdge();
    TEST_VERIFY(channelData.GetCurrentSample() == 11);


}

int main(int argc, char* argv[])
{
    verifyMockChannelData();
    verifyMockChannelData2();

    std::cout << "test harness verified ok" << std::endl;
    return EXIT_SUCCESS;
}
