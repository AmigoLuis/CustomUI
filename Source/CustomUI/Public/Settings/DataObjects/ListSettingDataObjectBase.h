// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ListSettingDataObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectBase : public UObject
{
	GENERATED_BODY()
public:
#pragma region Getters and Setters
	[[nodiscard]] FName GetDataID() const
	{
		return DataID;
	}

	void SetDataID(const FName& DataID)
	{
		this->DataID = DataID;
	}

	[[nodiscard]] FText GetDataDisplayName() const
	{
		return DataDisplayName;
	}

	void SetDataDisplayName(const FText& DataDisplayName)
	{
		this->DataDisplayName = DataDisplayName;
	}

	[[nodiscard]] FText GetDescriptionRichText() const
	{
		return DescriptionRichText;
	}

	void SetDescriptionRichText(const FText& DescriptionRichText)
	{
		this->DescriptionRichText = DescriptionRichText;
	}

	[[nodiscard]] FText GetDisabledRichText() const
	{
		return DisabledRichText;
	}

	void SetDisabledRichText(const FText& DisabledRichText)
	{
		this->DisabledRichText = DisabledRichText;
	}

	[[nodiscard]] TSoftObjectPtr<UTexture2D> GetSoftDescriptionImage() const
	{
		return SoftDescriptionImage;
	}

	void SetSoftDescriptionImage(const TSoftObjectPtr<UTexture2D>& SoftDescriptionImage)
	{
		this->SoftDescriptionImage = SoftDescriptionImage;
	}

	[[nodiscard]] UListSettingDataObjectBase* GetParentDataRaw() const
	{
		return ParentDataRaw;
	}

	void SetParentDataRaw(UListSettingDataObjectBase* const ParentDataRaw)
	{
		this->ParentDataRaw = ParentDataRaw;
	}
#pragma endregion Getters and Setters

	// empty in parent class, children should override it
	virtual TArray<UListSettingDataObjectBase*> GetAllChildrenDataObjects() const
	{
		return TArray<UListSettingDataObjectBase*>();
	}
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	UPROPERTY(Transient)
	UListSettingDataObjectBase* ParentDataRaw;
};
