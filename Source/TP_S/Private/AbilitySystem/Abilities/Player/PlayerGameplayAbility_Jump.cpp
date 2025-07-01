// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/PlayerGameplayAbility_Jump.h"

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
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAnimInstance* AnimInstance = GetAvatarActorFromActorInfo()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
	
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is NULL!"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (GetPlayerCharacterFromActorInfo() && GetPlayerCharacterFromActorInfo()->CanJump() && CurrentActorInfo->SkeletalMeshComponent->GetAnimInstance())
	{
		GetPlayerCharacterFromActorInfo()->Jump();
		
		if (JumpStartMontage)
		{
			/*
			UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				TEXT("None"),
				JumpStartMontage ,
				1.f
				);
			
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &ThisClass::OnCanceled);
			PlayMontageAndWait->OnInterrupted.AddDynamic(this, &ThisClass::OnInterrupted);
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
			PlayMontageAndWait->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOut);
			PlayMontageAndWait->Activate();
			
			//Debug::Print(PlayMontageAndWait->GetDebugString(), GetWorld()->GetTimeSeconds());
			PlayMontageAndWait->ReadyForActivation();
			//Debug::Print(!CachedPlayerCharacter->GetMesh()->GetAnimInstance() ? "AnimInstance is NULL" : "AnimInstance exists");
			*/
		}
		/*
		WaitLand = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Shared_Event_Land,
			nullptr,
			false,
			true
		);
		
		WaitLand->EventReceived.AddDynamic(this, &ThisClass::OnLandedEvent);
		WaitLand->ReadyForActivation();
		*/
		//CachedPlayerCharacter->PlayAnimMontage(JumpLoopMontage);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);		
	}
	
}

void UPlayerGameplayAbility_Jump::OnLandedEvent(FGameplayEventData Payload)
{
	Debug::Print("Lande2d",FColor::Green);
	if (CachedPlayerCharacter.IsValid() && JumpEndMontage)
	{
		//CachedPlayerCharacter->PlayAnimMontage(JumpEndMontage);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UPlayerGameplayAbility_Jump::OnCanceled()
{
	Debug::Print("Cancelled", FColor::Red);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerGameplayAbility_Jump::OnCompleted()
{
	Debug::Print("Completed", FColor::Green);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPlayerGameplayAbility_Jump::OnInterrupted()
{
	Debug::Print("Interrupted");
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerGameplayAbility_Jump::OnBlendOut()
{
	Debug::Print("BlendOut", FColor::Green);
}
