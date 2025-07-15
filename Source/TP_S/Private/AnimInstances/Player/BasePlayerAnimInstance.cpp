// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/BasePlayerAnimInstance.h"

#include "DebugHelper.h"
#include "TP_S/Public/Character/Player/BasePlayerCharacter.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "KismetAnimationLibrary.h"
#include "Components/Movement/PlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/DataValidation.h"

UBasePlayerAnimInstance::UBasePlayerAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBasePlayerAnimInstance::InitializeWithAbilitySystem(UBaseAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UBasePlayerAnimInstance::IsDataValid(class FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif

void UBasePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter != nullptr)
	{
		OwningPlayerCharacter = Cast<ABasePlayerCharacter>(OwningCharacter); 
	}
	if (OwningPlayerCharacter)
	{
		if (UBaseAbilitySystemComponent* ASC = UBaseFunctionLibrary::NativeGetBaseASCFromActor(OwningPlayerCharacter))
		{
			InitializeWithAbilitySystem(ASC);
		}
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
			GroundSpeed = 0.f;
		}
		else
		{
			if (DoesOwnerHaveTag(BaseGamePlayTags::Shared_Status_InAir))
			{
				UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(OwningPlayerCharacter, BaseGamePlayTags::Shared_Status_InAir);
			}
			GroundSpeed = OwningCharacter->GetVelocity().Size2D();
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

		CachedLocomotionDirection = LocomotionDirection;
		UPlayerMovementComponent* CharMoveComp = CastChecked<UPlayerMovementComponent>(OwningPlayerCharacter->GetCharacterMovement());
		const FPlayerGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
		GroundDistance = GroundInfo.GroundDistance;
	}
}
