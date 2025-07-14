// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Range/PGA_Attack_Range.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Range_Fire.h"
#include "AbilitySystem/Effects/GE/GE_DealDamage.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Controllers/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPGA_Attack_Range::UPGA_Attack_Range()
{
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
}

void UPGA_Attack_Range::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	DirectionFix(true);
	PlayerCombatComponent = GetPlayerCombatComponentFromActorInfo();
	WeaponType = BaseGamePlayTags::Item_Equipable_Weapon_Range;
	WeaponSocketName = FName("hand_rRangeSocket");
	EquipWeapon();
	
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
	DirectionFix(true);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPGA_Attack_Range::HandleApplyDamage(FGameplayEventData Data)
{
	Super::HandleApplyDamage(Data);
	AActor* TargetActor = const_cast<AActor*>(Data.Target.Get());
	if (IsValid(TargetActor))
	{
		TSubclassOf<UGameplayEffect> Effect = UGE_DealDamage::StaticClass();
		float BaseDamage = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquippedWeaponDamageAtLevel(GetAbilityLevel());
		int ComboCount = 0;
		FGameplayEffectSpecHandle SpecHandle = MakePlayerDamageGameplayEffectHandle(
			Effect,
			BaseDamage,
			BaseGamePlayTags::Player_Ability_Attack_Range,
			ComboCount);
		NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, BaseGamePlayTags::Shared_Event_HitReact, Data);
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
			FGameplayEventData Data = FGameplayEventData();
			Data.Target = Hit.GetActor();
			HandleApplyDamage(Data);
		}
	}
}

void UPGA_Attack_Range::StopFire()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
