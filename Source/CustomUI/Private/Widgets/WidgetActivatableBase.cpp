// Amigo Luis all rights reserved


#include "Widgets/WidgetActivatableBase.h"
#include "PlayerController/FrontEndPlayerController.h"
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
