#ifndef LOGICPUBLICTYPES
#define LOGICPUBLICTYPES

#ifndef WIN32
	#define __cdecl
#endif

#ifdef ANALYZER_EXPORTS
	#ifdef WIN32
		#define LOGICAPI __declspec(dllexport)
	#else
		#define LOGICAPI __attribute__ ((visibility("default")))
	#endif
#else
	#define LOGICAPI
#endif

#ifdef WIN32
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
typedef signed long long int S64;

#ifndef NULL
	#define NULL 0
#endif

enum DisplayBase { Binary, Decimal, Hexadecimal, ASCII, AsciiHex };
enum BitState { BIT_LOW, BIT_HIGH };
#define Toggle(x) ( x == BIT_LOW ? BIT_HIGH : BIT_LOW )
#define Invert(x) ( x == BIT_LOW ? BIT_HIGH : BIT_LOW )

#define UNDEFINED_CHANNEL Channel( 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF )

//Saleae: Note this class can not add, remove or reorder member variables without breaking binary compatibilty
//Saleae: This function can add non-virtual functions as long as they are listed after existing functions.
class LOGICAPI Channel
{
public:
	Channel();
	Channel( const Channel& channel );
	Channel( U64 device_id, U32 channel_index );
	~Channel();

	Channel& operator=( const Channel& channel );
	bool operator==( const Channel& channel ) const;
	bool operator!=( const Channel& channel ) const;
	bool operator>( const Channel& channel ) const;
	bool operator<( const Channel& channel ) const;

	U64 mDeviceId;
	U32 mChannelIndex;
};

#include <memory> //required by some compilers for std::auto_ptr

#endif //LOGICPUBLICTYPES
