// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Move/PlayerGameplayAbility_Jump.h"

#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


void UPlayerGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	UAnimInstance* AnimInstance = GetPlayerCharacterFromActorInfo()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !AnimInstance)
	{
		OnCompleteCallback();
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_InAir))
	{
		SecondJump();
	}
	else
	{
		Jump();
	}
	Action();
}

void UPlayerGameplayAbility_Jump::Jump()
{
	if (!GetPlayerCharacterFromActorInfo()->CanJump())
	{
		OnCompleteCallback();
		return;
	}
	
	MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Idle];
	JumpX = GetPlayerCharacterFromActorInfo()->GetActorForwardVector().X * 500.f;
	JumpY = GetPlayerCharacterFromActorInfo()->GetActorForwardVector().Y * 500.f;
	JumpZ = 740.f;
	
	if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Slide))
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Slide];
		JumpX = GetPlayerCharacterFromActorInfo()->GetVelocity().X;
		JumpY = GetPlayerCharacterFromActorInfo()->GetVelocity().Y;
		
	}
	else if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_Run))
	{
		MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_Run];
	}
	
	GetPlayerCharacterFromActorInfo()->LaunchCharacter(FVector(JumpX, JumpY, JumpZ), false, false);
	//GetPlayerCharacterFromActorInfo()->Jump();
	GetPlayerCharacterFromActorInfo()->JumpCount++;
}

void UPlayerGameplayAbility_Jump::SecondJump()
{
	Debug::Print(TEXT("%d"), GetPlayerCharacterFromActorInfo()->JumpCount);
	if (GetPlayerCharacterFromActorInfo()->JumpCount >= 2)
	{
		OnCompleteCallback();
		return;
	}
	
	MontageToPlay = MontageByTag[BaseGamePlayTags::Shared_Status_InAir];

	JumpX = GetPlayerCharacterFromActorInfo()->GetActorForwardVector().X * 500.f;
	JumpY = GetPlayerCharacterFromActorInfo()->GetActorForwardVector().Y * 500.f;
	JumpZ = 740.f;
	GetPlayerCharacterFromActorInfo()->LaunchCharacter(FVector(JumpX, JumpY, JumpZ), false, false);
	GetPlayerCharacterFromActorInfo()->JumpCount = 2;
}


