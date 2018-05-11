#include "AnalyzerSettings.h"
#include "AnalyzerSettingInterface.h"

#include <cmath>
#include <cassert>
#include <algorithm>

#include "MockSettings.h"

#define D_PTR() \
    auto d = reinterpret_cast<AnalyzerTest::MockSettings*>(mData);

AnalyzerSettings::AnalyzerSettings()
{
    AnalyzerTest::MockSettings* d = new AnalyzerTest::MockSettings;
    mData = reinterpret_cast<struct AnalyzerSettingsData*>(d);
}

AnalyzerSettings::~AnalyzerSettings()
{
    D_PTR();
   // delete d;
}

void AnalyzerSettings::ClearChannels()
{
    D_PTR();
    d->mChannels.clear();
}

void AnalyzerSettings::AddChannel( Channel& channel, const char* channel_label, bool is_used )
{
    D_PTR();
    d->mChannels.push_back({channel, channel_label, is_used});
}

void AnalyzerSettings::SetErrorText( const char* error_text )
{

}

void AnalyzerSettings::AddInterface( AnalyzerSettingInterface* analyzer_setting_interface )
{
    D_PTR();
    d->mInterfaces.push_back(analyzer_setting_interface);
}


void AnalyzerSettings::AddExportOption( U32 user_id, const char* menu_text )
{

}

void AnalyzerSettings::AddExportExtension(U32 user_id, const char *extension_description, const char *extension)
{

}

const char* AnalyzerSettings::SetReturnString( const char* str )
{
    return "";
}

namespace {

// helper class to access mData outside the class. Compiler will flatten
// all this code into nothing
class DataExtractor : public AnalyzerSettings {
public:
    AnalyzerTest::MockSettings* mockData() {
        return reinterpret_cast<AnalyzerTest::MockSettings*>(mData);
    }
};

// helper class to access mData outside the class. Compiler will flatten
// all this code into nothing
class InterfaceDataExtractor : public AnalyzerSettingInterface {
public:
    AnalyzerTest::MockSettingInterface* mockData() {
        return reinterpret_cast<AnalyzerTest::MockSettingInterface*>(mData);
    }
};

} // of anonymous namespace

namespace AnalyzerTest {

MockSettings* MockSettings::MockFromSettings(AnalyzerSettings* settings)
{
    return static_cast<DataExtractor*>(settings)->mockData();
}

MockSettingInterface *MockSettings::GetSetting(const std::string &title)
{
    auto it = std::find_if(mInterfaces.begin(), mInterfaces.end(),
                  [title](AnalyzerSettingInterface* interface)
    {
            auto mockInterface = MockSettingInterface::MockFromInterface(interface);
            return title == mockInterface->mTitle;
    });

    if (it == mInterfaces.end())
        throw std::runtime_error("no such setting: " + title);

    return MockSettingInterface::MockFromInterface(*it);
}

MockSettingInterface *MockSettings::GetSetting(U32 index)
{
    if (index >= mInterfaces.size())
        return nullptr;

    return MockSettingInterface::MockFromInterface(mInterfaces.at(index));
}

MockSettingInterface* MockSettingInterface::MockFromInterface(AnalyzerSettingInterface *iface)
{
    return static_cast<InterfaceDataExtractor*>(iface)->mockData();
}

void MockSettingInterface::SetNumberedListIndexByLabel(const std::string &name)
{
    if (mTypeId != INTERFACE_NUMBER_LIST)
        throw std::runtime_error("SetNumberedListIndexByLabel: not a number list");

    auto it = std::find_if(mNamedValueList.begin(), mNamedValueList.end(),
                  [name](const NamedValue& nv) { return name == nv.name; });
    if (it == mNamedValueList.end()) {
        throw std::runtime_error("No value in list with name: " + name);
    }

    mValue = std::distance(mNamedValueList.begin(), it);
}

void MockSettingInterface::SetNumberedListIndexByValue(const double v)
{
    if (mTypeId != INTERFACE_NUMBER_LIST)
        throw std::runtime_error("SetNumberedListIndexByLabel: not a number list");

    auto it = std::find_if(mNamedValueList.begin(), mNamedValueList.end(),
                  [v](const NamedValue& nv) { return v == nv.value; });
    if (it == mNamedValueList.end()) {
        throw std::runtime_error("No value in list: " + std::to_string(v));
    }

    mValue = std::distance(mNamedValueList.begin(), it);
}

double MockSettingInterface::GetNumberedValue() const
{
    if (mTypeId != INTERFACE_NUMBER_LIST)
        throw std::runtime_error("SetNumberedListIndexByLabel: not a number list");

    return mNamedValueList.at(mValue).value;
}

int MockSettingInterface::GetNumberedValueInt() const
{
    return static_cast<int>(GetNumberedValue());
}


} // of namespace AnalyzerTest

//////////////////////////////////////////////////////////////////////////////

#undef D_PTR
#define D_PTR() \
    auto d = reinterpret_cast<AnalyzerTest::MockSettingInterface*>(mData);

AnalyzerSettingInterface::AnalyzerSettingInterface()
{
    AnalyzerTest::MockSettingInterface* d = new AnalyzerTest::MockSettingInterface;
    d->mTypeId = INTERFACE_BASE;
    mData = reinterpret_cast<struct AnalyzerSettingInterfaceData*>(d);
}

AnalyzerSettingInterface::~AnalyzerSettingInterface()
{
    D_PTR();
    delete d;
}

void AnalyzerSettingInterface::operator delete ( void* p )
{
    free(p);
}

void* AnalyzerSettingInterface::operator new( size_t size )
{
    return malloc(size);
}

AnalyzerInterfaceTypeId AnalyzerSettingInterface::GetType()
{
    D_PTR();
    return d->mTypeId;
}

const char *AnalyzerSettingInterface::GetTitle()
{
    D_PTR();
    return d->mTitle.c_str();
}

void AnalyzerSettingInterface::SetTitleAndTooltip(const char *title, const char *tooltip)
{
    D_PTR();
    d->mTitle = title;
    d->mTooltip = tooltip;
}

//////////////////////////////////////////////////////////////////////////////


AnalyzerSettingInterfaceChannel::AnalyzerSettingInterfaceChannel()
{
   D_PTR();
   d->mTypeId = INTERFACE_CHANNEL;
}

AnalyzerSettingInterfaceChannel::~AnalyzerSettingInterfaceChannel()
{

}

AnalyzerInterfaceTypeId AnalyzerSettingInterfaceChannel::GetType()
{
    return INTERFACE_CHANNEL;
}

Channel AnalyzerSettingInterfaceChannel::GetChannel()
{
    D_PTR();
    return d->mChannel;
}

void AnalyzerSettingInterfaceChannel::SetChannel(const Channel &channel)
{
    D_PTR();
    d->mChannel = channel;
}

bool AnalyzerSettingInterfaceChannel::GetSelectionOfNoneIsAllowed()
{
    D_PTR();
    return d->mNoneChannelAllowed;
}

void AnalyzerSettingInterfaceChannel::SetSelectionOfNoneIsAllowed(bool is_allowed)
{
    D_PTR();
    d->mNoneChannelAllowed = is_allowed;
}

//////////////////////////////////////////////////////////////////////////////


AnalyzerSettingInterfaceNumberList::AnalyzerSettingInterfaceNumberList()
{
    D_PTR();
    d->mTypeId = INTERFACE_NUMBER_LIST;
}

AnalyzerSettingInterfaceNumberList::~AnalyzerSettingInterfaceNumberList()
{

}

AnalyzerInterfaceTypeId AnalyzerSettingInterfaceNumberList::GetType()
{
    return INTERFACE_NUMBER_LIST;
}

double AnalyzerSettingInterfaceNumberList::GetNumber()
{
    D_PTR();
    return d->mNamedValueList.at(d->mValue).value;
}

void AnalyzerSettingInterfaceNumberList::SetNumber(double number)
{
    D_PTR();
    auto it = std::find_if(d->mNamedValueList.begin(), d->mNamedValueList.end(),
                           [number](const AnalyzerTest::MockSettingInterface::NamedValue& nv)
    { return std::fabs(nv.value - number) < 1e-12;});
    assert(it != d->mNamedValueList.end());
    d->mValue =  std::distance(d->mNamedValueList.begin(), it);
}

void AnalyzerSettingInterfaceNumberList::AddNumber( double number, const char* str, const char* tooltip )
{
    D_PTR();
    d->mNamedValueList.push_back({str, number, tooltip});
}

//////////////////////////////////////////////////////////////////////////////

AnalyzerSettingInterfaceInteger::AnalyzerSettingInterfaceInteger()
{
    D_PTR();
    d->mTypeId = INTERFACE_INTEGER;
}

AnalyzerSettingInterfaceInteger::~AnalyzerSettingInterfaceInteger()
{

}

AnalyzerInterfaceTypeId AnalyzerSettingInterfaceInteger::GetType()
{
    return INTERFACE_INTEGER;
}

int AnalyzerSettingInterfaceInteger::GetInteger()
{
    D_PTR();
    return d->mValue;
}

void AnalyzerSettingInterfaceInteger::SetInteger(int integer)
{
    D_PTR();
    d->mValue = integer;
}

int AnalyzerSettingInterfaceInteger::GetMax()
{
    D_PTR();
    return d->mMaxValue;
}

int AnalyzerSettingInterfaceInteger::GetMin()
{
    D_PTR();
    return d->mMinValue;
}

void AnalyzerSettingInterfaceInteger::SetMax(int max)
{
    D_PTR();
    d->mMaxValue = max;
}

void AnalyzerSettingInterfaceInteger::SetMin(int min)
{
    D_PTR();
    d->mMinValue = min;
}

//////////////////////////////////////////////////////////////////////////////

AnalyzerSettingInterfaceText::AnalyzerSettingInterfaceText()
{
    D_PTR();
    d->mTypeId = INTERFACE_TEXT;
}

AnalyzerSettingInterfaceText::~AnalyzerSettingInterfaceText()
{

}

AnalyzerInterfaceTypeId AnalyzerSettingInterfaceText::GetType()
{
    return INTERFACE_TEXT;
}

const char *AnalyzerSettingInterfaceText::GetText()
{
    D_PTR();
    return d->mText.c_str();
}

void AnalyzerSettingInterfaceText::SetText(const char *text)
{
    D_PTR();
    d->mText = std::string(text);
}

AnalyzerSettingInterfaceText::TextType AnalyzerSettingInterfaceText::GetTextType()
{
    D_PTR();
    return d->mTextType;
}

void AnalyzerSettingInterfaceText::SetTextType(AnalyzerSettingInterfaceText::TextType text_type)
{
    D_PTR();
    d->mTextType = text_type;
}

//////////////////////////////////////////////////////////////////////////////

AnalyzerSettingInterfaceBool::AnalyzerSettingInterfaceBool()
{
    D_PTR();
    d->mTypeId = INTERFACE_BOOL;
}

AnalyzerSettingInterfaceBool::~AnalyzerSettingInterfaceBool()
{

}

AnalyzerInterfaceTypeId AnalyzerSettingInterfaceBool::GetType()
{
    return INTERFACE_BOOL;
}

bool AnalyzerSettingInterfaceBool::GetValue()
{
    D_PTR();
    return d->mValue != 0;
}

void AnalyzerSettingInterfaceBool::SetValue(bool value)
{
    D_PTR();
    d->mValue = value;
}

const char *AnalyzerSettingInterfaceBool::GetCheckBoxText()
{
    D_PTR();
    return d->mText.c_str();
}

void AnalyzerSettingInterfaceBool::SetCheckBoxText(const char *text)
{
    D_PTR();
    d->mText = std::string(text);
}


