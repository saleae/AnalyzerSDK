#ifndef ANALYZER_TEST_MOCK_SETTINGS_H
#define ANALYZER_TEST_MOCK_SETTINGS_H

#include "AnalyzerSettings.h"

namespace AnalyzerTest
{

class MockSettingInterface;

class MockSettings
{
public:

    static MockSettings *MockFromSettings(AnalyzerSettings *settings);

    struct SettingChannel {
        Channel channel;
        std::string label;
        bool used;
    };

    std::vector<SettingChannel> mChannels;
    std::vector<AnalyzerSettingInterface*> mInterfaces;

    MockSettingInterface* GetSetting(U32 index);
    MockSettingInterface* GetSetting(const std::string& title);
};

class MockSettingInterface
{
public:
    static MockSettingInterface *MockFromInterface(AnalyzerSettingInterface *iface);

    void SetNumberedListIndexByLabel(const std::string& name);
    void SetNumberedListIndexByValue(const double v);

    double GetNumberedValue() const;

    /**
     * @brief GetNumberedValueInt - return the value but cast to an int
     * This is syntactic sugar for MSVC especially
     * @return
     */
    int GetNumberedValueInt() const;

    AnalyzerInterfaceTypeId mTypeId;
    std::string mTitle;
    std::string mTooltip;

    // to simplify the test code, we store all the different settings
    // interface types as instances of this class

    // interface-channel
    Channel mChannel;
    bool mNoneChannelAllowed = false;

    int mValue; // bool, integer, number list


    struct NamedValue
    {
        std::string name;
        double value;
        std::string tooltip;
    };

    std::vector<NamedValue> mNamedValueList; // number list

    std::string mText; // text, bool
    AnalyzerSettingInterfaceText::TextType mTextType;

    int mMinValue, mMaxValue; // integer
};

}  // of namespace

#endif // of ANALYZER_TEST_MOCK_SETTINGS_H
