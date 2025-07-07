// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/BasePlayerAnimInstance.h"

#include "DebugHelper.h"
#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter != nullptr)
	{
		OwningPlayerCharacter = Cast<ABasePlayerCharacter>(OwningCharacter); 
	}
}

void UBasePlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);


	
	if (OwningPlayerCharacter != nullptr)
	{
		bCrouch = OwningMovementComponent->IsCrouching(); 

		bFalling = OwningMovementComponent->IsFalling();
		
		if (bCrouch)
		{
			UBaseFunctionLibrary::AddGameplayTagToActorIfNone(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_Crouch);
		}
		else
		{
			if (DoesOwnerHaveTag(BaseGamePlayTags::Shared_Status_Crouch))
			{
				UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_Crouch);
			}
		}

		if (bFalling)
		{
			UBaseFunctionLibrary::AddGameplayTagToActorIfNone(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_InAir);
		}
		else
		{
			if (DoesOwnerHaveTag(BaseGamePlayTags::Shared_Status_InAir))
			{
				UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_InAir);
			}
		}
	
		if (bHasAcceleration)
		{
			FootPhase = GetCurveValue("FootPhase");
			UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_Idle);
			UBaseFunctionLibrary::AddGameplayTagToActorIfNone(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_Run);
		}
		else
		{
			FootPhase = 0;
			UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_Run);
			UBaseFunctionLibrary::AddGameplayTagToActorIfNone(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_Idle);
		}
	}
}
