#ifndef ANALYZER_TEST_MOCK_RESULTS
#define ANALYZER_TEST_MOCK_RESULTS

#include <iostream>

#include "AnalyzerResults.h"

namespace AnalyzerTest
{

class MockResultData 
{
public:
    /**
     * @brief extract the result test data from an AnalyzerResults
     * */
    static MockResultData* MockFromResults(AnalyzerResults* results);

    U64 AddFrame(const Frame& f);
    const Frame& GetFrame(U64 index) const;

    struct MarkerInfo
    {
        U64 frame;
        AnalyzerResults::MarkerType type;
        Channel channel;
    };

    U64 CurrentFrame() const;
    U64 NextFrame() const;

    struct StringInfo
    {
        U64 frame;
        std::string string;
    };

    void AddString(const std::string& s);

    void AddMarker(const MarkerInfo& marker);

    void SetCancelled(bool cancelled);

    typedef std::pair<U64, U64> FrameRange;
    FrameRange GetFrameRangeForPacket(U64 packetIndex) const;

    U64 TotalFrameCount() const;
    U64 TotalPacketCount() const;
    U64 TotalCommitCount() const;

    U32 TotalStringCount() const;
    std::string GetString(U32 index) const;

    U32 TotalTabularTextCount() const;
    std::string GetTabularText(U32 index) const;

    U32 TotalMarkerCount() const;
    MarkerInfo GetMarker(U32 index) const;
private:
    friend ::AnalyzerResults;

    std::vector<Frame> mFrames;
    std::vector<U64> mPacketStartFrames;
    std::vector<U64> mCommitFrames;
    std::vector<MarkerInfo> mMarkers;
    std::vector<StringInfo> mStrings;
    std::vector<std::string> mTabularText;

    bool mCancelled = false;
};

// output operators for the test macro
std::ostream& operator<<(std::ostream& out, const MockResultData::FrameRange& range);

}  // of namespace

#endif
