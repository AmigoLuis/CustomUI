// Amigo Luis all rights reserved


#include "Widgets/Components/FrontEndCommonTextBlock.h"

#include "Widgets/StringTableLocations.h"

void UFrontEndCommonTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (!TextKeyInStringTable.IsEmpty())
	{
		const FName& UsedStringTableId = 
			StringTableId.IsNone() ? TEXT(ST_UN_ASSORTED) : StringTableId;
		const FText& LocalizedText = 
			GET_VALUE_FOR_KEY_FROM_ST_DIRECT(UsedStringTableId, TextKeyInStringTable);
		SetText(LocalizedText);
	}
}
