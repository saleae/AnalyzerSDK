#ifndef ANALYZERCHANNELDATA
#define ANALYZERCHANNELDATA

#include <LogicPublicTypes.h>

struct AnalyzerChannelDataData;
class ChannelData;
class LOGICAPI AnalyzerChannelData
{
public:
	AnalyzerChannelData( ChannelData* channel_data );
	~AnalyzerChannelData();

	//State
	U64 GetSampleNumber();
	BitState GetBitState();
	
	//Basic:
	U32 Advance( U32 num_samples );  //move forward the specified number of samples.  Returns the number of times the bit changed state during the move.
	U32 AdvanceToAbsPosition( U64 sample_number );  //move forward to the specified sample number.  Returns the number of times the bit changed state during the move.
	void AdvanceToNextEdge();  //move forward until the bit state changes from what it is now.
	
	//Fancier
	U64 GetSampleOfNextEdge();	//without moving, get the sample of the next transition. 
	bool WouldAdvancingCauseTransition( U32 num_samples );  //if we advanced, would we encounter any transitions?
	bool WouldAdvancingToAbsPositionCauseTransition( U64 sample_number );  //if we advanced, would we encounter any transitions?
	
	//minimum pulse tracking.  The serial analyzer uses this for auto-baud
	void TrackMinimumPulseWidth();	//normally this is not enabled.
	U64 GetMinimumPulseWidthSoFar();

	//Fancier, part II
	bool DoMoreTransitionsExistInCurrentData(); //use this when you have a situation where you have multiple lines, and you need to handle the case where one or the other of them may never change again, and you don't know which.
	

protected:
	AnalyzerChannelDataData* mData;
};


#endif //ANALYZERCHANNELDATA
