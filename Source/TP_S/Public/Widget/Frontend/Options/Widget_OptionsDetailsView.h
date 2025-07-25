// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"

class UListDataObject_Base;
class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class TP_S_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:
	// UUserWidget Interface
	virtual void NativeOnInitialized() override;
	
private:
	// Bound Widget
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_Description;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_DynamicDetails;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_DisabledReason;
};
