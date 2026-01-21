#pragma once

UENUM(BlueprintType)
enum class EConfirmationWidgetType : uint8
{
    OK			    UMETA(DisplayName = "OK"),
    YesNo			UMETA(DisplayName = "YesNo"),
    OkCancel		UMETA(DisplayName = "OkCancel"),
    AcceptDecline	UMETA(DisplayName = "AcceptDecline"),
    Unknown			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmationChoiceType : uint8
{
    Confirmed	    UMETA(DisplayName = "Confirmed"),
    Canceled		UMETA(DisplayName = "Canceled"),
    Closed	        UMETA(DisplayName = "Closed"),
    Unknown			UMETA(Hidden)
};