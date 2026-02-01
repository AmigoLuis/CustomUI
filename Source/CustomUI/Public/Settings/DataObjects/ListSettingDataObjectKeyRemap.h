// Amigo Luis all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Settings/DataObjects/ListSettingDataObjectBase.h"
#include "ListSettingDataObjectKeyRemap.generated.h"

struct FPlayerKeyMapping;
enum class EPlayerMappableKeySlot : uint8;
enum class ECommonInputType : uint8;
class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class CUSTOMUI_API UListSettingDataObjectKeyRemap : public UListSettingDataObjectBase
{
	GENERATED_BODY()
public:
	void InitKeyRemap(UEnhancedInputUserSettings* InputSettings, UEnhancedPlayerMappableKeyProfile* KeyProfile, 
		ECommonInputType DesiredInputType, const FPlayerKeyMapping& InPlayerKeyMapping);
	
	FSlateBrush GetSlateBrushForCurrentKey() const;
	
	FORCEINLINE ECommonInputType GetDesiredInputType() const {return CachedDesiredInputType;}
	
	void BindNewInputKey(const FKey& InNewKey);
	// UListSettingDataObjectBase
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetToDefaultValue() const override;
	virtual bool TryResetToDefaultValue() override;
	// UListSettingDataObjectBase
private:
	FPlayerKeyMapping* GetOwningKeyMapping() const;
	
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedInputSettings;
	
	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedKeyProfile;
	
	ECommonInputType CachedDesiredInputType;
	
	FName CachedMappingName;
	
	EPlayerMappableKeySlot CachedKeySlot;
};
