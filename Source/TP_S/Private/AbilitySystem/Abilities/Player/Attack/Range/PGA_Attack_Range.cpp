// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Range/PGA_Attack_Range.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Range_Fire.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Controllers/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPGA_Attack_Range::UPGA_Attack_Range()
{
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_Light);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_Heavy);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee_ReceiveWeapon);
	
}

void UPGA_Attack_Range::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	EquipWeapon(FName("hand_rRangeSocket"));
	FireTask = UAT_Attack_Range_Fire::Action(this, 0.2f);
	FireTask->OnStartedTask.AddDynamic(this, &ThisClass::HandleFire);
	FireTask->OnFinishedTask.AddDynamic(this, &ThisClass::StopFire);
	FireTask->ReadyForActivation();

	
	if (UAnimInstance* Anim = GetOwningComponentFromActorInfo()->GetAnimInstance())
	{
		if (AnimLayer)
		{
			Anim->LinkAnimClassLayers(AnimLayer);
		}
	}
	/*
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("Attack"),
			MontageToPlay,
			1.f,
			GetNextSection(),
			false);
	//Task->OnCancelled.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleteCallback);
	Task->ReadyForActivation();
	*/
	
	if (GetPlayerCharacterFromActorInfo())
	{
		GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->bOrientRotationToMovement = false;
		//GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = 200.f;
		GetPlayerCharacterFromActorInfo()->bUseControllerRotationYaw = true;
		
	}
}

void UPGA_Attack_Range::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UPGA_Attack_Range::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (FireTask)
	{
		FireTask->StopFire();
	}
}

void UPGA_Attack_Range::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPGA_Attack_Range::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (FireTask)
	{
		FireTask->StopFire();
	}
	if (GetPlayerCharacterFromActorInfo())
	{
		GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->bOrientRotationToMovement = true;
		GetPlayerCharacterFromActorInfo()->bUseControllerRotationYaw = false;
		//GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnEquipWeapon(FGameplayEventData());
}

void UPGA_Attack_Range::EquipWeapon(FName SocketName)
{
	if (GetPlayerCombatComponentFromActorInfo())
	{
		GetPlayerCombatComponentFromActorInfo()->EquipWeapon(BaseGamePlayTags::Item_Equipable_Weapon_Range, SocketName);
	}
}

void UPGA_Attack_Range::UnEquipWeapon(FGameplayEventData TargetData)
{
	if (GetPlayerCombatComponentFromActorInfo())
	{
		GetPlayerCombatComponentFromActorInfo()->UnEquipWeapon(BaseGamePlayTags::Item_Equipable_Weapon_Range);
	}
}

void UPGA_Attack_Range::HandleFire()
{
	// 총알 발사 구현부
	if (GetPlayerCharacterFromActorInfo()){
		TArray<AActor*> Ignores;
		FVector Start = GetPlayerCharacterFromActorInfo()->GetCameraComponent()->GetComponentLocation();
		FVector End = Start + GetPlayerCharacterFromActorInfo()->GetCameraComponent()->GetForwardVector() * 1000;
		TArray<FHitResult> Hits;
		UKismetSystemLibrary::LineTraceMulti(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, Ignores, EDrawDebugTrace::ForDuration, Hits, true);

		for (const FHitResult& Hit : Hits)
		{
			//Debug::Print(Hit.GetActor()->GetActorLabel());
		}
	}
}

void UPGA_Attack_Range::StopFire()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
