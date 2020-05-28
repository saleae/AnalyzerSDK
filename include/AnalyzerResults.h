#ifndef ANALYZER_RESULTS
#define ANALYZER_RESULTS

#include <LogicPublicTypes.h>
#include <string>

#define DISPLAY_AS_ERROR_FLAG ( 1 << 7 )
#define DISPLAY_AS_WARNING_FLAG ( 1 << 6 )

#define SUPPORTS_PROTOCOL_SEARCH

#ifdef LOGIC2
class FrameV2Data;
class LOGICAPI FrameV2
{
  public:
    FrameV2();
    ~FrameV2();

    void AddString( const char* key, const char* value );
    void AddDouble( const char* key, double value );
    void AddInteger( const char* key, S64 value );
    void AddBoolean( const char* key, bool value );
    void AddByte( const char* key, U8 value );
    void AddByteArray( const char* key, const U8* data, U64 length );

    FrameV2Data* mInternals;
};
#endif

class LOGICAPI Frame
{
  public:
    Frame();
    Frame( const Frame& frame );
    ~Frame();

    S64 mStartingSampleInclusive;
    S64 mEndingSampleInclusive;
    U64 mData1;
    U64 mData2;
    U8 mType;
    U8 mFlags;

    bool HasFlag( U8 flag );
};

#define INVALID_RESULT_INDEX 0xFFFFFFFFFFFFFFFFull


struct AnalyzerResultsData;


class LOGICAPI AnalyzerResults
{
  public:
    enum MarkerType
    {
        Dot,
        ErrorDot,
        Square,
        ErrorSquare,
        UpArrow,
        DownArrow,
        X,
        ErrorX,
        Start,
        Stop,
        One,
        Zero
    };
    AnalyzerResults(); // you must call the base class contructor in your constructor
    virtual ~AnalyzerResults();

    // override:
    virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base ) = 0;
    virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id ) = 0;
    virtual void GenerateFrameTabularText( U64 frame_index, DisplayBase display_base ) = 0;
    virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base ) = 0;
    virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base ) = 0;

  public: // adding/setting data
#ifdef LOGIC2
    void AddFrameV2( const FrameV2& frame, const char* type, U64 starting_sample, U64 ending_sample );
#endif

    void AddMarker( U64 sample_number, MarkerType marker_type, Channel& channel );
    U64 AddFrame( const Frame& frame );
    U64 CommitPacketAndStartNewPacket();
    void CancelPacketAndStartNewPacket();
    void AddPacketToTransaction( U64 transaction_id, U64 packet_id );
    void AddChannelBubblesWillAppearOn( const Channel& channel );

    void CommitResults();

  public: // data access
    U64 GetNumFrames();
    U64 GetNumPackets();
    Frame GetFrame( U64 frame_id );

    U64 GetPacketContainingFrame( U64 frame_id );
    U64 GetPacketContainingFrameSequential( U64 frame_id );
    void GetFramesContainedInPacket( U64 packet_id, U64* first_frame_id, U64* last_frame_id );

    U64 GetTransactionContainingPacket( U64 packet_id );
    void GetPacketsContainedInTransaction( U64 transaction_id, U64** packet_id_array, U64* packet_id_count );


  public: // text results setting and access:
    void ClearResultStrings();
    void AddResultString( const char* str1, const char* str2 = NULL, const char* str3 = NULL, const char* str4 = NULL,
                          const char* str5 = NULL, const char* str6 = NULL ); // multiple strings will be concatinated

    void GetResultStrings( char const*** result_string_array, U32* num_strings );

  protected: // use these when exporting data.
    bool UpdateExportProgressAndCheckForCancel( U64 completed_frames, U64 total_frames );

  public: // don't use
    bool DoBubblesAppearOnChannel( Channel& channel );
    bool DoMarkersAppearOnChannel( Channel& channel );
    bool GetFramesInRange( S64 starting_sample_inclusive, S64 ending_sample_inclusive, U64* first_frame_index, U64* last_frame_index );
    bool GetMarkersInRange( Channel& channel, S64 starting_sample_inclusive, S64 ending_sample_inclusive, U64* first_marker_index,
                            U64* last_marker_index );
    void GetMarker( Channel& channel, U64 marker_index, MarkerType* marker_type, U64* marker_sample );
    U64 GetNumMarkers( Channel& channel );
    bool GetFrameBeforeSample( S64 sample_number, U64* frame_index );
    bool GetFrameAfterSample( S64 sample_number, U64* frame_index );

    void CancelExport();
    double GetProgress();
    void StartExportThread( const char* file, DisplayBase display_base, U32 export_type_user_id );
    AnalyzerResultsData* GetAnalyzerResultsData();

  protected:
    struct AnalyzerResultsData* mData;

  public:
    void AddTabularText( const char* str1, const char* str2 = NULL, const char* str3 = NULL, const char* str4 = NULL,
                         const char* str5 = NULL, const char* str6 = NULL );
    void GetSearchData( const char*** result_string_array, U32* num_strings );
    void ClearTabularText();
};


#endif // ANALYZER_RESULTS
