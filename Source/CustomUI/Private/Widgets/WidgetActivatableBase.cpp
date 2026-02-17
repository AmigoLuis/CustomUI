// Amigo Luis all rights reserved


#include "Widgets/WidgetActivatableBase.h"

#include "CheckAndLogAndReturn.h"
#include "ICommonInputModule.h"
#include "PlayerController/FrontEndPlayerController.h"
#include "Input/CommonUIInputTypes.h"
#include "UILogger.h"

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
		ClickActionHandle = RegisterUIActionBinding(FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultClickAction(), true,
			FSimpleDelegate::CreateUObject(
				this, 
				&UWidgetActivatableBase::HandleDefaultClickAction)));
	}
	if (bShowBackActionBinding)
	{
		BackActionHandle = RegisterUIActionBinding(FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(), true,
			FSimpleDelegate::CreateUObject(
				this, 
				&UWidgetActivatableBase::HandleDefaultBackAction)));
	}

}

void UWidgetActivatableBase::HandleDefaultClickAction()
{
}

void UWidgetActivatableBase::HandleDefaultBackAction()
{
	DeactivateWidget();
}
