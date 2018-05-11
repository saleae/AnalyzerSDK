#include "Analyzer.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <cassert>

#include "AnalyzerHelpers.h"


struct ClockGeneratorData
{
    double mSamplesPerHalfPeriod;
    double mCurrentError;
    double mSampleTime;
};

ClockGenerator::ClockGenerator()
:	mData( new ClockGeneratorData() )
{
}

ClockGenerator::~ClockGenerator()
{
    delete mData;
    mData = NULL;
}

void ClockGenerator::Init( double target_frequency, U32 sample_rate_hz )
{
    mData->mSamplesPerHalfPeriod = double( sample_rate_hz ) / target_frequency;
    mData->mCurrentError = 0;
    mData->mSampleTime = 1.0 / sample_rate_hz;
}

U32 ClockGenerator::AdvanceByHalfPeriod( double multiple )
{
    double target_half_period =  ( mData->mSamplesPerHalfPeriod * multiple ) + mData->mCurrentError;
    U32 samples_to_advance = U32( target_half_period );
    mData->mCurrentError = target_half_period - double( samples_to_advance );
    return samples_to_advance;
}

U32 ClockGenerator::AdvanceByTimeS( double time_s )
{
    double target_num_samples =  ( time_s / mData->mSampleTime ) + mData->mCurrentError;
    U32 samples_to_advance = U32( target_num_samples );
    mData->mCurrentError = target_num_samples - double( samples_to_advance );
    return samples_to_advance;
}

struct BitExtractorData
{
    U64 mData;
    AnalyzerEnums::ShiftOrder mShiftOrder;
    U64 mMask;
};


BitExtractor::BitExtractor( U64 data,  AnalyzerEnums::ShiftOrder shift_order, U32 num_bits )
:	mData( new BitExtractorData() )
{
    mData->mData = data;
    mData->mShiftOrder = shift_order;

    if( shift_order ==  AnalyzerEnums::LsbFirst )
        mData->mMask = 0x1;
    else
        mData->mMask = 0x1ULL << ( num_bits - 1 );
}

BitExtractor::~BitExtractor()
{
    delete mData;
    mData = NULL;
}

BitState BitExtractor::GetNextBit()
{
    BitState result;

    if( ( mData->mMask & mData->mData ) == 0 )
        result = BIT_LOW;
    else
        result = BIT_HIGH;

    if( mData->mShiftOrder == AnalyzerEnums::LsbFirst )
        mData->mMask <<= 1;
    else
        mData->mMask >>= 1;

    return result;
}

struct DataBuilderData
{
    U64* mData;
    AnalyzerEnums::ShiftOrder mShiftOrder;
    U64 mMask;
};

DataBuilder::DataBuilder()
:	mData( new DataBuilderData() )
{
}

DataBuilder::~DataBuilder()
{
    delete mData;
    mData = NULL;
}

void DataBuilder::Reset( U64* data, AnalyzerEnums::ShiftOrder shift_order, U32 num_bits )
{
    mData->mData = data;
    mData->mShiftOrder = shift_order;

    if( shift_order ==  AnalyzerEnums::LsbFirst )
        mData->mMask = 0x1;
    else
        mData->mMask = 0x1ULL << ( num_bits - 1 );

    *data = 0;
}

void DataBuilder::AddBit( BitState bit )
{
    if( bit == BIT_HIGH )
        *mData->mData |= mData->mMask;

    if( mData->mShiftOrder == AnalyzerEnums::LsbFirst )
        mData->mMask <<= 1;
    else
        mData->mMask >>= 1;
}

void BinaryToStringStream( U64 number, U32 num_data_bits, std::stringstream& ss )
{
    if( num_data_bits == 0 )
    {
        int start;
        ss << "0b  ";

        U64 compairison = 0x0FFFFFFFFFFFFFFFull;
        start = 63;

        for( ; ; )
        {
            if( number > compairison )
                break;

            compairison >>= 4;
            start -=4 ;

            if( start == 3 )
                break;
        }

        U32 space_counter = 0;
        for(int i = start; i >= 0; --i)
        {
            if( space_counter == 4 )
            {
                space_counter = 0;
                ss << "  ";
            }
            space_counter++;
            if(number & ( (U64)1 << i))
                ss << "1";
            else
                ss << "0";
        }
    }
    else
    {
        ss << "0b  ";
        U32 space_counter;
        if( ( num_data_bits % 4 ) == 0 )
            space_counter = 0;
        else
            space_counter = 4 - ( num_data_bits % 4 );  //normally we start at zero, but if this is not a multile of 4, we'll wan't to start a bit advanced.

        for( int i = num_data_bits-1; i >= 0; --i )
        {
            if( space_counter == 4 )
            {
                space_counter = 0;
                ss << "  ";
            }
            space_counter++;
            if( ( number & ( 0x1ULL << i) ) != 0 )
                ss << "1";
            else
                ss << "0";
        }
    }
}

void HexToStringStream( U64 number, U32 num_data_bits, std::stringstream& ss )
{
    if( num_data_bits == 0 )
    {
        ss << "0x" << std::hex << std::uppercase << number;
    }else
    {
        U32 num_places = num_data_bits / 4;
        if( ( num_data_bits % 4 ) != 0 )
            num_places++;

        ss << "0x" << std::hex << std::setw(num_places) << std::setfill('0') << std::uppercase << number;
    }
}

void AsciiToStringStream( U64 number, std::stringstream& ss, bool csv_safe )
{
    if( (number < 0x20) || (number > 0x7e) )
    {
        switch( number )
        {
        case '\t':
            ss << "\\t";
            break;
        case '\r':
            ss << "\\r";
            break;
        case '\n':
            ss << "\\n";
            break;
        default:
            ss << "'" << number << "'";
            break;
        }
    }
    else if( ( number == ',' ) && ( csv_safe == true ) )
    {
        //',' == 0x0x2C
        ss << "COMMA";
    }
    else
    {
        if( number == ' ' )
            ss << "' '";
        else
            ss << char( number );
    }
}

void AnalyzerHelpers::GetNumberString(U64 number, DisplayBase display_base, U32 num_data_bits, char *result_string, U32 result_string_max_length)
{
    std::stringstream ss;
    bool csv_safe = true;

    switch( display_base )
    {
    case Binary:
        BinaryToStringStream( number, num_data_bits, ss );
        break;

    case Decimal:
        ss << number;
        break;

    case Hexadecimal:
        HexToStringStream( number, num_data_bits, ss );
        break;

    case ASCII:
        AsciiToStringStream( number, ss, csv_safe );
        break;

    case AsciiHex:
        AsciiToStringStream( number, ss, csv_safe );
        ss << " (";
        HexToStringStream( number, num_data_bits, ss );
        ss << ")";
        break;
    }

    std::string result = ss.str();
    if( ( result.size() + 1 ) > result_string_max_length )
        result = result.substr( 0, result_string_max_length - 1 );

    strcpy( result_string, result.c_str() );
}

void AnalyzerHelpers::GetTimeString( U64 sample, U64 trigger_sample, U32 sample_rate_hz, char* result_string, U32 result_string_max_length )
{
    std::stringstream ss;
    ss.precision( 15 );
    double time = double( S64(sample) - S64(trigger_sample) );
    time = time / double( sample_rate_hz );
    ss << std::dec << std::fixed << time;

    std::string result = ss.str();
    if( ( result.size() + 1 ) > result_string_max_length )
        result = result.substr( 0, result_string_max_length - 1 );

    strcpy( result_string, result.c_str() );
}

void AnalyzerHelpers::Assert(const char *message)
{
    // record this assert for the test?
    std::cerr << "Helper assert:" << message << std::endl;
    exit(-1);
}

U64 AnalyzerHelpers::AdjustSimulationTargetSample(U64 target_sample, U32 sample_rate, U32 simulation_sample_rate)
{
    //we need to adjust the requested minimum_sample_index to our mSimulationSampleRateHz
    U64 multiplier = sample_rate / simulation_sample_rate;
    return target_sample / multiplier;
}

bool AnalyzerHelpers::DoChannelsOverlap(const Channel *channel_array, U32 num_channels)
{
    return false;
}

struct TestHelperFile
{
    std::string fileName;
    bool isBinary;
    std::string contents;
};

void *AnalyzerHelpers::StartFile(const char *file_name, bool is_binary)
{
    auto p = new TestHelperFile{file_name, is_binary, {}};
    return p;
}

void AnalyzerHelpers::AppendToFile(const U8 *data, U32 data_length, void *file)
{
    assert(file);
    auto p = reinterpret_cast<TestHelperFile*>(file);
    p->contents.append(std::string(reinterpret_cast<const char*>(data), data_length));
}

void AnalyzerHelpers::EndFile(void *file)
{
    assert(file);
    auto p = reinterpret_cast<TestHelperFile*>(file);
    delete p;
    // or save off for later testing
}

/////////////////////////////////////////////////////////////////////////////


struct SimpleArchiveData
{
};

SimpleArchive::SimpleArchive()
:	mData( new SimpleArchiveData() )
{
}

SimpleArchive::~SimpleArchive()
{
    delete mData;
    mData = NULL;
}

void SimpleArchive::SetString( const char* archive_string )
{
 }

const char* SimpleArchive::GetString()
{
    return nullptr;
}

bool SimpleArchive::operator<<( U64 data )
{
    return true;
}
bool SimpleArchive::operator<<( U32 data )
{

    return true;
}
bool SimpleArchive::operator<<( S64 data )
{
    return true;
}
bool SimpleArchive::operator<<( S32 data )
{
    return true;
}
bool SimpleArchive::operator<<( double data )
{

    return true;
}
bool SimpleArchive::operator<<( bool data )
{
    return true;
}
bool SimpleArchive::operator<<( const char* data )
{
    return true;
}
bool SimpleArchive::operator<<( Channel& data )
{
    return true;
}

bool SimpleArchive::operator>>( U64& data )
{
    return true;
}
bool SimpleArchive::operator>>( U32& data )
{
    return true;
}
bool SimpleArchive::operator>>( S64& data )
{
    return true;
}
bool SimpleArchive::operator>>( S32& data )
{
    return true;
}
bool SimpleArchive::operator>>( double& data )
{
    return true;
}
bool SimpleArchive::operator>>( bool& data )
{
    return true;
}
bool SimpleArchive::operator>>( char const ** data )
{

    return true;
}
bool SimpleArchive::operator>>( Channel& data )
{

    return true;
}
