#include "TestInstance.h"

#include "AnalyzerResults.h"
#include "TestAnalyzerData.h"
#include "MockSimulatedChannelDescriptor.h"

#include <cassert>
#include <iostream>
#include <cstring>

extern "C" {
    Analyzer* __cdecl CreateAnalyzer( );
    const char* __cdecl GetAnalyzerName();
}

namespace AnalyzerTest
{


AnalyzerResults *Instance::GetResults()
{
    return GetResultsFromAnalyzer(mAnalyzerInstance.get());
}

Instance::Instance(const std::string& name)
{
    if (!name.empty()) {
        CreatePlugin(name);
    }
}

Instance::~Instance()
{

}

void Instance::CreatePlugin(const std::string& name)
{
    assert(name == GetAnalyzerName());
    mAnalyzerInstance.reset(CreateAnalyzer());
    assert(mAnalyzerInstance.get());
    assert(mAnalyzerInstance->GetAnalyzerName() == name);
}

void Instance::SetChannelData(const Channel& chan, MockChannelData *mockData)
{
    GetDataFromAnalyzer(mAnalyzerInstance.get())->channelData[chan] = mockData;
}

void Instance::SetSampleRate(U64 sample_rate_hz)
{
    GetDataFromAnalyzer(mAnalyzerInstance.get())->sampleRateHz = sample_rate_hz;
}

U64 Instance::GetSampleRate() const
{
    return GetDataFromAnalyzer(mAnalyzerInstance.get())->sampleRateHz;
}

auto Instance::RunAnalyzerWorker(int timeoutSec) -> RunResult
{
    assert(mAnalyzerInstance);

    // setup a watch-dog to timeout this

    // TODO - test harness should be told if the analyser is using
    // the Analyser2 interface (maybe they all do in practice?)
    auto analyzer2 = dynamic_cast<Analyzer2*>(mAnalyzerInstance.get());
    if (analyzer2) {
        analyzer2->SetupResults();
    }

    RunResult result = WorkerError;

    try {
        mAnalyzerInstance->WorkerThread();
    } catch (OutOfDataException& ode) {
        // this is 'normal' termination for a worker thread
        result = WorkerRanOutOfData;
    } catch (CancellationException& ce) {
        result = WorkerTimeout;
    } catch (std::exception& e) {
        std::cerr << "Worker thread raised exception:" << e.what() << std::endl;
        result = WorkerError;
    }

    return result;
}


void Instance::RunSimulation(U64 num_samples, U32 sample_rate_hz)
{
    mSimulatedChannels.clear();
    SimulationChannelDescriptor* channels[16];

    GetDataFromAnalyzer(mAnalyzerInstance.get())->simulationRateHz = sample_rate_hz;

    U32 count = mAnalyzerInstance->GenerateSimulationData(num_samples, sample_rate_hz, channels);
    if (count == 0) {
        return;
    }

    mSimulatedChannels.resize(count);
    memcpy(mSimulatedChannels.data(), channels, count * sizeof(SimulationChannelDescriptor*));
}

SimulatedChannel *Instance::GetSimulationChannel(const Channel &chan)
{
    if (mSimulatedChannels.empty()) {
        std::cerr << "no simualted channels exist" << std::endl;
        return nullptr;
    }

    for (auto sc : mSimulatedChannels) {
        if (sc->GetChannel() == chan) {
            return SimulatedChannel::FromSimulatedChannelDescriptor(sc);
        }
    }

    std::cerr << "GetSimulationChannel: couldn't find data for channel" << std::endl;
    return nullptr;
}

AnalyzerSettings *Instance::GetSettings()
{
    return GetDataFromAnalyzer(mAnalyzerInstance.get())->settings;
}

void Instance::GenerateBubbleText(U64 frame_index, Channel channel, DisplayBase display_base)
{
    auto results = GetResults();
    results->ClearResultStrings();
    results->GenerateBubbleText(frame_index, channel, display_base);
}

void Instance::GenerateTabularText(U64 frame_index, DisplayBase display_base)
{
    auto results = GetResults();
    results->GenerateFrameTabularText(frame_index, display_base);
}

bool Instance::CheckCancellation() const
{

    return false;
}

OutOfDataException::OutOfDataException()
{

}

CancellationException::CancellationException()
{

}

} // of namespace AnalyzerTest

