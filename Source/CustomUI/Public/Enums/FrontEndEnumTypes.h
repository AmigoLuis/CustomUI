#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
    OK			    UMETA(DisplayName = "OK"),
    YesNo			UMETA(DisplayName = "YesNo"),
    OkCancel		UMETA(DisplayName = "OkCancel"),
    AcceptDecline	UMETA(DisplayName = "AcceptDecline"),
    Unknown			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
    Confirmed	    UMETA(DisplayName = "Confirmed"),
    Canceled		UMETA(DisplayName = "Canceled"),
    Closed	        UMETA(DisplayName = "Closed"),
    Unknown			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESettingsListDataModifyReason : uint8
{
    DirectlyModified	    UMETA(DisplayName = "DirectlyModified"),
    DependencyModified		UMETA(DisplayName = "DependencyModified"),
    ResetToDefault	        UMETA(DisplayName = "ResetToDefault")
};