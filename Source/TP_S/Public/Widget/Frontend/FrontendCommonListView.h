// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendCommonListView.generated.h"

class UDataAsset_DataListEntryMapping;
/**
 * 
 */
UCLASS()
class TP_S_API UFrontendCommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	// UCommonListView Interface
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	
private:
	// UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	
	UPROPERTY(EditAnywhere, Category = "Frontend List View Settings")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;
};
