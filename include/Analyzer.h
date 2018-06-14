#ifndef ANALYZER
#define ANALYZER

#include <LogicPublicTypes.h>
#include <AnalyzerTypes.h>
#include <SimulationChannelDescriptor.h>

#include "AnalyzerSettings.h"

class AnalyzerChannelData;
class DeviceCollection;
class ConditionManager;
class ProgressManager;
struct AnalyzerData;
class AnalyzerResults;

class LOGICAPI Analyzer
{
public:
	Analyzer();
	virtual ~Analyzer() = 0;
	virtual void WorkerThread() = 0;
	
	//sample_rate: if there are multiple devices attached, and one is faster than the other,
	//we can sample at the speed of the faster one; and pretend the slower one is the same speed.
	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels ) = 0;
	virtual U32 GetMinimumSampleRateHz() = 0;  //provide the sample rate required to generate good simulation data
	virtual const char* GetAnalyzerName() const = 0;
	virtual bool NeedsRerun() = 0;

	//use, but don't override:
	void SetAnalyzerSettings( AnalyzerSettings* settings );
	void KillThread();
	AnalyzerChannelData* GetAnalyzerChannelData( Channel& channel );  //don't delete this pointer
	void ReportProgress( U64 sample_number );
	void SetAnalyzerResults( AnalyzerResults* results );
	U32 GetSimulationSampleRate();
	U32 GetSampleRate();
	U64 GetTriggerSample();

	//don't override, don't use:
	void Init( DeviceCollection* device_collection, ConditionManager* condition_manager, ProgressManager* progress_manager );
	void StartProcessing();
	void StopWorkerThread();
	AnalyzerSettings* GetAnalyzerSettings();
	bool DoesAnalyzerUseDevice( U64 device_id );
	bool IsValid( Channel* channel_array, U32 count );
	void InitialWorkerThread();
	bool GetAnalyzerResults( AnalyzerResults** analyzer_results );

	//functions added after initial release -- added at the bottom for binary compatibilty
	void CheckIfThreadShouldExit();
	double GetAnalyzerProgress();
	void SetThreadMustExit();

	void StartProcessing( U64 starting_sample );

	
	
	/*
	bool DoesChannelHaveBubbles( Channel& channel );
	bool DoesChannelHaveMarkers( Channel& channel );
	bool GetBubblesInRange( Channel& channel, S64 starting_sample_inclusive, S64 ending_sample_inclusive, U32* first_bubble, U32* last_bubble );
	bool GetMarkersInRange( Channel& channel, S64 starting_sample_inclusive, S64 ending_sample_inclusive, U32* first_marker, U32* last_marker );
	void GetBubblePosition( Channel& channel, U32 index, U64& starting_position_inclusive, U64& ending_position_inclusive );
	ResultMarker GetMarker( Channel& channel, U32 index );
	U64 GetNumBubbles( Channel& channel );
	U64 GetNumMarkers( Channel& channel );
*/

	

protected: 
	struct AnalyzerData* mData;
};

class LOGICAPI Analyzer2 : public Analyzer
{
public:
	Analyzer2();
	virtual void SetupResults();
};

#endif  //ANALYZER
