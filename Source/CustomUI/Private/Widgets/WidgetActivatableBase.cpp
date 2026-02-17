// Amigo Luis all rights reserved


#include "Widgets/WidgetActivatableBase.h"

#include "CheckAndLogAndReturn.h"
#include "ICommonInputModule.h"
#include "PlayerController/FrontEndPlayerController.h"
#include "Input/CommonUIInputTypes.h"
#include "UILogger.h"
#include "Widgets/StringTableLocations.h"

AFrontEndPlayerController* UWidgetActivatableBase::GetOwningFrontEndPlayerController()
{
	if (!CachedOwningFrontEndPC.IsValid())
	{
		CachedOwningFrontEndPC = GetOwningPlayer<AFrontEndPlayerController>();
	}
	AFrontEndPlayerController* CachedOwningFrontEndPCPtr = CachedOwningFrontEndPC.Get();
	CHECK_NULL_RETURN_VALUE(CachedOwningFrontEndPCPtr, CachedOwningFrontEndPCPtr);
	return CachedOwningFrontEndPCPtr;
}

void UWidgetActivatableBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (bShowClickActionBinding)
	{
		FBindUIActionArgs ClickActionArgs = FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultClickAction(), true,
			FSimpleDelegate::CreateUObject(
				this, 
				&UWidgetActivatableBase::HandleDefaultClickAction));
		ClickActionArgs.OverrideDisplayName = GET_MAIN_MENU_FOR_KEY("ClickAction");
		ClickActionHandle = RegisterUIActionBinding(ClickActionArgs);
	}
	if (bShowBackActionBinding)
	{
		FBindUIActionArgs BackActionArgs = FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(), true,
			FSimpleDelegate::CreateUObject(
				this, 
				&UWidgetActivatableBase::HandleDefaultBackAction));
		BackActionArgs.OverrideDisplayName = GET_MAIN_MENU_FOR_KEY("BackAction");
		BackActionHandle = RegisterUIActionBinding(BackActionArgs);
	}

}

void UWidgetActivatableBase::HandleDefaultClickAction()
{
}

void UWidgetActivatableBase::HandleDefaultBackAction()
{
	DeactivateWidget();
}
