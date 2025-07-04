// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Jump.h"

#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"


void UPlayerGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	GetPlayerCharacterFromActorInfo();
	UAnimInstance* AnimInstance = GetAvatarActorFromActorInfo()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (AnimInstance != nullptr)
	{

		if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Run))
		{
			MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Run];
		}
		if (MontageToPlay != nullptr)
		{
			//Debug::Print("Is Exist");
			/*
			PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				FName("None"),
				MontageToPlay
				);
			PlayMontageAndWait->Activate();
			PlayMontageAndWait->ReadyForActivation();
			*/
		}
		
	}

}

void UPlayerGameplayAbility_Jump::OnLandedEvent(FGameplayEventData Payload)
{
	//Debug::Print("Lande2d",FColor::Green);
	//if (CachedPlayerCharacter.IsValid() && JumpEndMontage)
	//{
		//CachedPlayerCharacter->PlayAnimMontage(JumpEndMontage);
	//}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

