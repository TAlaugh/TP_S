// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Range/PGA_Attack_Range.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Range_Fire.h"
#include "AbilitySystem/Effects/GE/GE_DealDamage.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Controllers/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPGA_Attack_Range::UPGA_Attack_Range()
{
	AbilityTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Range);
	ActivationOwnedTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Range);
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
	//ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Movement_Crouch);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Movement_Slide);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Movement_Dodge);
	CancelAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement_Jump);
	CancelAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Movement_DoubleJump);
	AttackType = BaseGamePlayTags::Player_Ability_Attack_Range;
}

void UPGA_Attack_Range::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	CachedPlayerCameraBoomSocket = GetPlayerCharacterFromActorInfo()->GetSpringArmComponent()->SocketOffset;
	CachedPlayerCameraBoomLength = GetPlayerCharacterFromActorInfo()->GetSpringArmComponent()->TargetArmLength;
	
	GetPlayerCharacterFromActorInfo()->GetSpringArmComponent()->TargetArmLength = 100.0f;
	GetPlayerCharacterFromActorInfo()->GetSpringArmComponent()->SocketOffset = FVector(0.f, 55.f, 65.f);
	DirectionFix(true);
	RotationSetByAim(true);
	PlayerCombatComponent = GetPlayerCombatComponentFromActorInfo();
	WeaponType = BaseGamePlayTags::Item_Equipable_Weapon_Range;
	WeaponSocketName = FName("hand_rRangeSocket");
	bUnEquipWhenEnd = false;
	EquipWeapon();
	Weapon = PlayerCombatComponent->GetPlayerCurrentEquippedWeapon();
	FireTask = UAT_Attack_Range_Fire::Action(this, 0.1f);
	FireTask->OnStartedTask.AddDynamic(this, &ThisClass::HandleFire);
	FireTask->OnFinishedTask.AddDynamic(this, &ThisClass::StopFire);
	FireTask->ReadyForActivation();
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
	DirectionFix(false);
	GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->GravityScale = 1.5f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetPlayerCombatComponentFromActorInfo()->UpdateAnimLayer();
	GetPlayerCharacterFromActorInfo()->GetSpringArmComponent()->TargetArmLength = CachedPlayerCameraBoomLength;
	GetPlayerCharacterFromActorInfo()->GetSpringArmComponent()->SocketOffset = CachedPlayerCameraBoomSocket;
}

void UPGA_Attack_Range::HandleApplyDamage(FGameplayEventData Data)
{
	Super::HandleApplyDamage(Data);
	AActor* TargetActor = const_cast<AActor*>(Data.Target.Get());
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (IsValid(TargetActor) && ASC)
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

		if (UBaseFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BaseGamePlayTags::Shared_Status_InAir)
		&& GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->IsFalling() && GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->Velocity.Z < 0)
		{
			GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->GravityScale = 0.0001f;
		}
		TArray<AActor*> Ignores;
		Ignores.Add(GetPlayerCharacterFromActorInfo());
		FVector Start = GetPlayerCharacterFromActorInfo()->GetCameraComponent()->GetComponentLocation();
		FVector End = Start + GetPlayerCharacterFromActorInfo()->GetCameraComponent()->GetForwardVector() * 10000;
		FHitResult Hit;
		// 충돌체 판별(지형지물 & Pawn)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECC_WorldStatic));
		ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECC_Pawn));
		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false, Ignores, EDrawDebugTrace::None, Hit, true);

		if (MuzzleEffect->IsValid() && Weapon && Weapon->GetSkeletalMeshComponent())
		{
			FFXSystemSpawnParameters SpawnParams = FFXSystemSpawnParameters();
			SpawnParams.AttachPointName = FName("muzzle_Socket");
			SpawnParams.AttachToComponent = Weapon->GetSkeletalMeshComponent();
			SpawnParams.bAutoDestroy = true;
			SpawnParams.bAutoActivate = true;
			SpawnParams.Location = FVector::ZeroVector;
			SpawnParams.Rotation = FRotator::ZeroRotator;
			SpawnParams.Scale = FVector(0.3f, 0.3f, 0.3f);
			SpawnParams.SystemTemplate = MuzzleEffect;
			SpawnParams.LocationType = EAttachLocation::Type::SnapToTarget;
			SpawnParams.PoolingMethod = EPSCPoolMethod::None;
			SpawnParams.bPreCullCheck = true;
			UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
			
			if (IsValid(CameraShake))
			{
				GetPlayerControllerFromActorInfo()->ClientStartCameraShake(CameraShake);
			}
			
		}
		if (IsValid(Hit.GetActor()) && UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Hit.GetActor()))
		{
			FGameplayEventData Data = FGameplayEventData();
			Data.Target = Hit.GetActor();
			Data.Instigator = GetAvatarActorFromActorInfo();
			HandleApplyDamage(Data);	
		}
		if (HitMuzzleEffect->IsValid())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitMuzzleEffect,
				Hit.Location,
				FRotator(0.1f, 0.1f, 0.1f),
				FVector(0.1f, 0.1f, 0.1f));
		}
	}
}

void UPGA_Attack_Range::StopFire()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
