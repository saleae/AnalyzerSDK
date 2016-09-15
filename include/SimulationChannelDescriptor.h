#ifndef SIMULATION_CHANNEL_DESCRIPTOR
#define SIMULATION_CHANNEL_DESCRIPTOR

#include <LogicPublicTypes.h>

//Saleae: PIMPL implementation can be changed, non-virtual functions can be added at the end, no vars can be added/removed/reordered/changed.
struct SimulationChannelDescriptorData;
class LOGICAPI SimulationChannelDescriptor
{
public:
	void Transition();
	void TransitionIfNeeded( BitState bit_state );
	void Advance( U32 num_samples_to_advance );

	BitState GetCurrentBitState();
	U64 GetCurrentSampleNumber();

public:  //don't use
	SimulationChannelDescriptor();
	SimulationChannelDescriptor( const SimulationChannelDescriptor& other );
	~SimulationChannelDescriptor();
	SimulationChannelDescriptor& operator=( const SimulationChannelDescriptor& other );

	void SetChannel( Channel& channel );
	void SetSampleRate( U32 sample_rate_hz );
	void SetInitialBitState( BitState intial_bit_state );

	Channel GetChannel();
	U32 GetSampleRate();
	BitState GetInitialBitState();
	void* GetData();

protected:
	struct SimulationChannelDescriptorData* mData;


};


struct SimulationChannelDescriptorGroupData;
class LOGICAPI SimulationChannelDescriptorGroup
{
public:
	SimulationChannelDescriptorGroup();
	~SimulationChannelDescriptorGroup();

	SimulationChannelDescriptor* Add( Channel& channel, U32 sample_rate, BitState intial_bit_state ); //do not delete this pointer

	void AdvanceAll( U32 num_samples_to_advance );

public:  
	SimulationChannelDescriptor* GetArray();
	U32 GetCount();

protected:
	struct SimulationChannelDescriptorGroupData* mData;
};



#endif //SIMULATION_CHANNEL_DESCRIPTOR
