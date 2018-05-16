#ifndef ANALYZER_TEST_INSTANCE_H
#define ANALYZER_TEST_INSTANCE_H

#include "AnalyzerChannelData.h"
#include "Analyzer.h"

namespace AnalyzerTest
{

// forward decls
class MockChannelData;
class SimulatedChannel;
class MockSettings;

// signalling exceptions used to exit the worker thread in various
// siutations.

class OutOfDataException : public std::exception
{
public:
    OutOfDataException(); // non-inline method to pin the class
};

class CancellationException : public std::exception
{
public:
    CancellationException();
};

class Instance
{
public:
    Instance(const std::string& name = {});

    ~Instance();

    void CreatePlugin(const std::string &name);

    void SetChannelData(const Channel& chan, MockChannelData* mockData);

    void SetSampleRate(U64 sample_rate_hz);
    U64 GetSampleRate() const;

    enum RunResult {
        WorkerRanOutOfData = 0,
        WorkerTimeout,
        WorkerError
    };

    RunResult RunAnalyzerWorker(int timeoutSec = 0);

    AnalyzerResults* GetResults();

    void RunSimulation(U64 num_samples, U32 sample_rate_hz);

    SimulatedChannel* GetSimulationChannel(const Channel& chan);

    AnalyzerSettings* GetSettings();

    void GenerateBubbleText( U64 frame_index, Channel channel, DisplayBase display_base );

    void GenerateTabularText(U64 frame_index, DisplayBase display_base);

    /**
     * @brief CheckCancellation - hook for MockChannelData to check if
     * processing should be abandoned
     * @return
     */
    bool CheckCancellation() const;
private:
    std::unique_ptr<Analyzer> mAnalyzerInstance;
    std::vector<SimulationChannelDescriptor*> mSimulatedChannels;
};


} // of namespace AnalyzerTest


#endif // of ANALYZER_TEST_INSTANCE_H
