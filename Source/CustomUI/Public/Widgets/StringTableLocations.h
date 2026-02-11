#pragma once
#include "Internationalization/StringTableRegistry.h"

# define GET_VALUE_FOR_KEY_FROM_ST(StringTable, KeyString) \
LOCTABLE(StringTable, KeyString)

#define STRING_TABLE_FOLDER "/Game/StringTables"
#define ST_UN_ASSORTED STRING_TABLE_FOLDER "/ST_UnAssorted.ST_UnAssorted"
#define ST_VIDEO_SETTINGS STRING_TABLE_FOLDER "/ST_VideoSettingsAndDescriptions.ST_VideoSettingsAndDescriptions"
#define ST_MAIN_MENU STRING_TABLE_FOLDER "/ST_MainMenu.ST_MainMenu"
#define ST_SOUND_SETTINGS STRING_TABLE_FOLDER "/ST_SoundSettingsAndDescriptions.ST_SoundSettingsAndDescriptions"


# define GET_VIDEO_SETTING_FOR_KEY(KeyString) GET_VALUE_FOR_KEY_FROM_ST(ST_VIDEO_SETTINGS, KeyString)
# define GET_SOUND_SETTING_FOR_KEY(KeyString) GET_VALUE_FOR_KEY_FROM_ST(ST_SOUND_SETTINGS, KeyString)
# define GET_MAIN_MENU_FOR_KEY(KeyString) GET_VALUE_FOR_KEY_FROM_ST(ST_MAIN_MENU, KeyString)
# define GET_UN_ASSORTED_FOR_KEY(KeyString) GET_VALUE_FOR_KEY_FROM_ST(ST_UN_ASSORTED, KeyString)