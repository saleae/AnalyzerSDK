#include <Analyzer.h>

class TrivialAnalyzer : public Analyzer2
{
public:
    TrivialAnalyzer()
    {

    }

    U32 GetMinimumSampleRateHz() override;

    const char* GetAnalyzerName() const override;
    
    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, 
    SimulationChannelDescriptor** simulation_channels ) override
    {
        return 0;
    }

    void SetupResults() override
    {

    }

    void WorkerThread() override
    {

    }  

    bool NeedsRerun() override
    {
        return false;
    }
private: 
};

U32 TrivialAnalyzer::GetMinimumSampleRateHz()
{
    return 12 * 100000; // 12Mhz minimum sample rate
}

const char* TrivialAnalyzer::GetAnalyzerName() const
{
    return "Trivial Analyzer";
}

extern "C"
{

const char* GetAnalyzerName()
{
    return "Trivial Analyzer";
}

Analyzer* CreateAnalyzer()
{
    return new TrivialAnalyzer;
}

void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}

} // of extern-C
