#ifndef ANALYZER_SETTINGS
#define ANALYZER_SETTINGS

#include <LogicPublicTypes.h>
#include "AnalyzerSettingInterface.h"


struct AnalyzerSettingsData;
struct AnalyzerValueDescription;

class LOGICAPI AnalyzerSettings
{
public:
	AnalyzerSettings();
	virtual ~AnalyzerSettings();

	//Implement
	virtual bool SetSettingsFromInterfaces() = 0;//Get the settings out of the interfaces, validate them, and save them to your local settings vars.
	virtual void LoadSettings( const char* settings ) = 0;  //Load your settings from the provided string
	virtual const char* SaveSettings() = 0;  //Save your settings to a string and return it. (use SetSettingsString, return GetSettingsString)

protected:
	//Use, but don't override/implement
	void ClearChannels(); //clear all the reported channels before adding them, if you need to change them.
	void AddChannel( Channel& channel, const char* channel_label, bool is_used );  //used to report the channels we're working with to the outside world.

	void SetErrorText( const char* error_text );  //if settings are invalid, set the text to display to the user (used in SetSettingsFromInterfaces)
	void AddInterface( AnalyzerSettingInterface* analyzer_setting_interface ); //add your interfaces so the outside world can access.  You'll need to keep copies of all the pointers you provide.

	void AddExportOption( U32 user_id, const char* menu_text );
	void AddExportExtension( U32 user_id, const char * extension_description, const char * extension );

	const char* SetReturnString( const char* str );

	//void ClearValueOutputs();
	//void AddValueOutput( const char* name );
public:
	//Do not use, do not override
	U32 GetSettingsInterfacesCount();
	AnalyzerSettingInterface* GetSettingsInterface( U32 index );

	U32 GetFileExtensionCount( U32 index_id );
	void GetFileExtension( U32 index_id, U32 extension_id, char const ** extension_description, char const ** extension );



	U32 GetChannelsCount();
	Channel GetChannel( U32 index, char const ** channel_label, bool* channel_is_used );

	U32 GetExportOptionsCount();
	void GetExportOption( U32 index, U32* user_id, char const ** menu_text );

	const char* GetSaveErrorMessage();

	bool GetUseSystemDisplayBase();
	void SetUseSystemDisplayBase( bool use_system_display_base );
	DisplayBase GetAnalyzerDisplayBase();
	void SetAnalyzerDisplayBase( DisplayBase analyzer_display_base );
	//added 5-15-2012
	//U32 GetValueDescriptionCount();
	//AnalyzerValueDescription GetValueDescription( U32 index );

protected:
	struct AnalyzerSettingsData* mData;

};

#endif  //ANALYZER_SETTINGS
