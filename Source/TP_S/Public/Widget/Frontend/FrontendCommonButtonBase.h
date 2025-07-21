// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class TP_S_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);
	
private:
	// UUserWidget Interface
	virtual void NativePreConstruct() override;

	// UCommonButtonBase Interface
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	
	// Bound Widgets
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;
	// Bound Widgets

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendButton", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendButton", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendButton", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};
