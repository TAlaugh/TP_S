// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "BaseFunctionLibrary.h"
#include "BaseGamePlayTags.h"
#include "DebugHelper.h"
#include "GameplayEffectExtension.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/BaseUIComponent.h"
#include "Interfaces/BaseUIInterface.h"

UBaseAttributeSet::UBaseAttributeSet()
{
	InitCurrentHp(1.f);
	InitMaxHp(1.f);
	// InitCurrentMp(1.f);
	// InitMaxMp(1.f);
	InitAttack(1.f);
	InitDefence(1.f);
	InitDamageTaken(1.f);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (!CachedUIInterface.IsValid())
	{
		Debug::Print(TEXT("!CachedUIInterface.IsValid()"));
		CachedUIInterface = TWeakInterfacePtr<IBaseUIInterface>(Data.Target.GetAvatarActor());
	}

	// checkf(CachedUIInterface.IsValid(), TEXT("%s does not Implementation IBaseUIInterface."), *Data.Target.GetAvatarActor()->GetActorLabel());
	
	// UBaseUIComponent* BaseUIComponent = CachedUIInterface->GetBaseUIComponent();

	// checkf(BaseUIComponent, TEXT("Can not Load BaseUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorLabel());
	
	if (Data.EvaluatedData.Attribute == GetCurrentHpAttribute())
	{
		const float NewCurrentHp = FMath::Clamp(GetCurrentHp(), 0.0f, GetMaxHp());
		SetCurrentHp(NewCurrentHp);

		// BaseUIComponent->OnCurrentHpChanged.Broadcast(GetCurrentHp()/GetMaxHp());
	}

	// if (Data.EvaluatedData.Attribute == GetCurrentMpAttribute())
	// {
	// 	const float NewCurrentMp = FMath::Clamp(GetCurrentMp(), 0.0f, GetMaxMp());
	// 	SetCurrentMp(NewCurrentMp);
	//
	// 	if (UHeroUIComponent* HeroUIComponent = CachedUIInterface->GetHeroUIComponent())
	// 	{
	// 		HeroUIComponent->OnCurrentMpChanged.Broadcast(GetCurrentMp()/GetMaxMp());
	// 	}
	// }

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float BeforeHp = GetCurrentHp();
		const float Damage = GetDamageTaken();

		const float NewCurrentHp = FMath::Clamp(BeforeHp - Damage, 0.0f, GetMaxHp());
		SetCurrentHp(NewCurrentHp);

		const FString DebugString =
			FString::Printf(TEXT("Before Hp: %f, Damage: %f, NewCurrentHp : %f"), BeforeHp, Damage, NewCurrentHp);
		
		// TODO:: Ui에 값을 전달
		// BaseUIComponent->OnCurrentHpChanged.Broadcast(GetCurrentHp()/GetMaxHp());

		//Character Death Process
		if (NewCurrentHp == 0.0f)
		{
			UBaseFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BaseGamePlayTags::Shared_Status_Dead);
		}
	}
}
