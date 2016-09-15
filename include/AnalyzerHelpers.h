#ifndef ANALYZER_HELPERS_H
#define ANALYZER_HELPERS_H

#include "Analyzer.h"
#include <string>
#include <cstring>

class LOGICAPI AnalyzerHelpers
{
public:
	static bool IsEven( U64 value );
	static bool IsOdd( U64 value );
	static U32 GetOnesCount( U64 value );
	static U32 Diff32( U32 a, U32 b );

	//static void GetNumberString( U64 number, DisplayBase display_base, U32 num_data_bits, char* result_string, U32 result_string_max_length, bool csv_safe = false );
	static void GetNumberString( U64 number, DisplayBase display_base, U32 num_data_bits, char* result_string, U32 result_string_max_length );
	static void GetTimeString( U64 sample, U64 trigger_sample, U32 sample_rate_hz, char* result_string, U32 result_string_max_length );

	static void Assert( const char* message );
	static U64 AdjustSimulationTargetSample( U64 target_sample, U32 sample_rate, U32 simulation_sample_rate );

	static bool DoChannelsOverlap( const Channel* channel_array, U32 num_channels );
	static void SaveFile( const char* file_name, const U8* data, U32 data_length, bool is_binary = false ); 

	static S64 ConvertToSignedNumber( U64 number, U32 num_bits );

	//These save functions should not be used with SaveFile, above.  They are a better way to export data (don't waste memory), and should be used from now on.
	static void* StartFile( const char* file_name, bool is_binary = false );
	static void	AppendToFile( const U8* data, U32 data_length, void* file );
	static void	EndFile( void* file );
};




struct ClockGeneratorData;
class LOGICAPI ClockGenerator
{
public:
	ClockGenerator();
	~ClockGenerator();
	void Init( double target_frequency, U32 sample_rate_hz );
	U32 AdvanceByHalfPeriod( double multiple = 1.0 );
	U32 AdvanceByTimeS( double time_s );

protected:
	struct ClockGeneratorData* mData;
};



struct BitExtractorData;
class LOGICAPI BitExtractor
{
public:
	BitExtractor( U64 data, AnalyzerEnums::ShiftOrder shift_order, U32 num_bits );
	~BitExtractor();

	BitState GetNextBit();

protected:
	struct BitExtractorData* mData;
};


struct DataBuilderData;
class LOGICAPI DataBuilder
{
public:
	DataBuilder();
	~DataBuilder();
	
	void Reset( U64* data, AnalyzerEnums::ShiftOrder shift_order, U32 num_bits );
	void AddBit( BitState bit );

protected:
	struct DataBuilderData* mData;
};




struct SimpleArchiveData;
class LOGICAPI SimpleArchive
{
public:
	SimpleArchive();
	~SimpleArchive();

	void SetString( const char* archive_string );
	const char* GetString();

	bool operator<<( U64 data );
	bool operator<<( U32 data );
	bool operator<<( S64 data );
	bool operator<<( S32 data );
	bool operator<<( double data );
	bool operator<<( bool data );
	bool operator<<( const char* data );
	bool operator<<( Channel& data );

	bool operator>>( U64& data );
	bool operator>>( U32& data );
	bool operator>>( S64& data );
	bool operator>>( S32& data );
	bool operator>>( double& data );
	bool operator>>( bool& data );
	bool operator>>( char const ** data );
	bool operator>>( Channel& data );
	
protected:
	struct SimpleArchiveData* mData;
};

#endif //ANALYZER_HELPERS_H
