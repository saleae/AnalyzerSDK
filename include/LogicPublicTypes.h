#ifndef LOGICPUBLICTYPES
#define LOGICPUBLICTYPES

#include <vector>

#ifndef _WIN32
	#define __cdecl
#endif

//If we discover that Analyzer needs to be compiled statically, and that __declspec(dllexport) causes an error with that build,
//then we need to add a define to turn this on or off for that project. i.e. Device SDK build. Not tested yet.
#ifdef _WIN32
	#define LOGICAPI __declspec(dllexport)
#else
	#define LOGICAPI __attribute__ ((visibility("default")))
#endif

#ifdef _WIN32
	#define ANALYZER_EXPORT __declspec(dllexport)
#else
	#define ANALYZER_EXPORT __attribute__ ((visibility("default")))
#endif

typedef char S8;
typedef short S16;
typedef int S32;
typedef long long int S64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long int U64;
//typedef signed long long int S64;

#ifndef NULL
	#define NULL 0
#endif

enum DisplayBase { Binary, Decimal, Hexadecimal, ASCII, AsciiHex };
enum BitState { BIT_LOW, BIT_HIGH };

enum ChannelDataType { 
	ANALOG_CHANNEL = 0,
	DIGITAL_CHANNEL = 1};

#define Toggle(x) ( x == BIT_LOW ? BIT_HIGH : BIT_LOW )
#define Invert(x) ( x == BIT_LOW ? BIT_HIGH : BIT_LOW )

#define UNDEFINED_CHANNEL Channel( 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF, DIGITAL_CHANNEL )

//Saleae: Note this class can not add, remove or reorder member variables without breaking binary compatibilty
//Saleae: This function can add non-virtual functions as long as they are listed after existing functions.
class LOGICAPI Channel
{
public:
	Channel();
	Channel( const Channel& channel );
	Channel( U64 device_id, U32 channel_index, ChannelDataType data_type );
	~Channel();

	Channel& operator=( const Channel& channel );
	bool operator==( const Channel& channel ) const;
	bool operator!=( const Channel& channel ) const;
	bool operator>( const Channel& channel ) const;
	bool operator<( const Channel& channel ) const;

	U64 mDeviceId;
	U32 mChannelIndex;
	ChannelDataType mDataType;
	//static std::vector<Channel> mFilteredChannels; // TODO: Messy idea, need a better way to fix Debug Assertion Failed error (_pfirstblock == phead )
	static std::vector<Channel>FilterChannelType( std::vector<Channel> all_channels, ChannelDataType data_type );
	static std::vector<U32> GetChannelIndexes( std::vector<Channel> channels );

};


#include <memory> //required by some compilers for std::auto_ptr

#endif //LOGICPUBLICTYPES
