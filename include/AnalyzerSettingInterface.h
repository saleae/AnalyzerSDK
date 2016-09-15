#ifndef ANALYZER_SETTING_INTERFACE
#define ANALYZER_SETTING_INTERFACE

#include <vector>
#include <string>
#include <LogicPublicTypes.h>

enum AnalyzerInterfaceTypeId { INTERFACE_BASE, INTERFACE_CHANNEL, INTERFACE_NUMBER_LIST, INTERFACE_INTEGER, INTERFACE_TEXT, INTERFACE_BOOL };

// Saleae note:  we don't need to have virtual functions to prevent code copies becuase they is only one copy of the code now -- in the DLL.
// we need one virtual function, to return the class type; we're not using rtti

//Saleae:  using PIMPL; struct can change, non-virtual functions can be added to the end;
//Saleae: Can not add/remove virtual functions


struct AnalyzerSettingInterfaceData;
class LOGICAPI AnalyzerSettingInterface
{
public:
	AnalyzerSettingInterface();
	virtual ~AnalyzerSettingInterface();

	static void operator delete ( void* p );
	static void* operator new( size_t size );
	virtual AnalyzerInterfaceTypeId GetType();

	const char* GetToolTip();
	const char* GetTitle();
	bool IsDisabled();
	void SetTitleAndTooltip( const char* title, const char* tooltip );

	//todo:  position, group, spacers, visual attributes

protected:
	struct AnalyzerSettingInterfaceData* mData;

};

struct AnalyzerSettingInterfaceChannelData;
class LOGICAPI AnalyzerSettingInterfaceChannel : public AnalyzerSettingInterface
{
public:
	AnalyzerSettingInterfaceChannel();
	virtual ~AnalyzerSettingInterfaceChannel();
	virtual AnalyzerInterfaceTypeId GetType();

	Channel GetChannel();
	void SetChannel( const Channel& channel );
	bool GetSelectionOfNoneIsAllowed();
	void SetSelectionOfNoneIsAllowed( bool is_allowed );

protected:
	struct AnalyzerSettingInterfaceChannelData* mChannelData;
};


struct AnalyzerSettingInterfaceNumberListData;
class LOGICAPI AnalyzerSettingInterfaceNumberList : public AnalyzerSettingInterface
{
public:
	AnalyzerSettingInterfaceNumberList();
	virtual ~AnalyzerSettingInterfaceNumberList();
	virtual AnalyzerInterfaceTypeId GetType();

	double GetNumber();
	void SetNumber( double number );

	U32 GetListboxNumbersCount();
	double GetListboxNumber( U32 index );

	U32 GetListboxStringsCount();
	const char* GetListboxString( U32 index );

	U32 GetListboxTooltipsCount();
	const char* GetListboxTooltip( U32 index );

	void AddNumber( double number, const char* str, const char* tooltip );
	void ClearNumbers();

protected:
	struct AnalyzerSettingInterfaceNumberListData* mNumberListData;

};


struct AnalyzerSettingInterfaceIntegerData;
class LOGICAPI AnalyzerSettingInterfaceInteger : public AnalyzerSettingInterface
{
public:
	AnalyzerSettingInterfaceInteger();
	virtual ~AnalyzerSettingInterfaceInteger();
	virtual AnalyzerInterfaceTypeId GetType();

	int GetInteger();
	void SetInteger( int integer );

	int GetMax();
	int GetMin();

	void SetMax( int max );
	void SetMin( int min );
	
protected:
	struct AnalyzerSettingInterfaceIntegerData* mIntegerData;

};


struct AnalyzerSettingInterfaceTextData;
class LOGICAPI AnalyzerSettingInterfaceText : public AnalyzerSettingInterface
{
public:
	AnalyzerSettingInterfaceText();
	virtual ~AnalyzerSettingInterfaceText();

	AnalyzerInterfaceTypeId GetType();
	const char* GetText();
	void SetText( const char* text );

	enum TextType { NormalText, FilePath, FolderPath };
	TextType GetTextType();
	void SetTextType( TextType text_type );


protected:
	struct AnalyzerSettingInterfaceTextData* mTextData;
	
};


struct AnalyzerSettingInterfaceBoolData;
class LOGICAPI AnalyzerSettingInterfaceBool : public AnalyzerSettingInterface
{
public:
	AnalyzerSettingInterfaceBool();
	virtual ~AnalyzerSettingInterfaceBool();
	virtual AnalyzerInterfaceTypeId GetType();

	bool GetValue();
	void SetValue( bool value );
	const char* GetCheckBoxText();
	void SetCheckBoxText( const char* text );

protected:
	struct AnalyzerSettingInterfaceBoolData* mBoolData;

};

#endif //ANALYZER_SETTING_INTERFACE
