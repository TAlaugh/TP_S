// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Subsystems/GameInstanceSubsystem.h"
// #include "FrontendLoadingScreenSubsystem.generated.h"
//
// /**
//  * 
//  */
// UCLASS()
// class TP_S_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
// {
// 	GENERATED_BODY()
//
// public:
// 	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingReason);
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;
// 	
// 	// USubsystem interface
// 	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
// 	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
// 	virtual void Deinitialize() override;
//
// 	// FTickableGameObject interface
// 	virtual UWorld* GetTickableGameObjectWorld() const override;
// 	virtual void Tick(float DeltaTime) override;
// 	virtual ETickableTickType GetTickableTickType() const override;
// 	virtual bool IsTickable() const override;
// 	virtual TStatId GetStatId() const override;
// 	void SetTickableTickType(ETickableTickType NewTickType);
// 	
// private:
// 	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);
//
// 	void OnMapPostLoaded(UWorld* LoadedWorld);
//
// 	void TryUpdateLoadingScreen();
//
// 	bool IsPreLoadScreenActive() const;
// 	
// 	bool ShouldShowLoadingScreen();
//
// 	bool CheckTheNeedToShowLoadingScreen();
//
// 	void TryDisplayLoadingScreenIfNone();
//
// 	void TryRemoveLoadingScreen();
//
// 	void NotifyLoadingScreenVisibilityChanged(bool bVisible);
// 	
// 	bool bIsCurrentlyLoadingMap = false;
//
// 	float HoldLoadingScreenStartUpTime = -1.f;
//
// 	FString CurrentLoadingReason;
//
// 	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;
// 	
// 	ETickableTickType CurrentTickType;
// };
