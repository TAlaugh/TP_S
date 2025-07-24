// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/Attack/Melee/PGA_Attack_Melee_Fall.h"

#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UPGA_Attack_Melee_Fall::UPGA_Attack_Melee_Fall()
{
	BlockAbilitiesWithTag.AddTag(BaseGamePlayTags::Player_Ability_Attack_Melee);
	ActivationRequiredTags.AddTag(BaseGamePlayTags::Shared_Status_InAir);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Status_WeaponThrown);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Movement_Dodge);
	ActivationBlockedTags.AddTag(BaseGamePlayTags::Player_Ability_Movement_Slide);
	AttackType = BaseGamePlayTags::Player_Ability_Attack_Melee_Fall;
	WeaponSocketName = FName("hand_rSocket");
}

void UPGA_Attack_Melee_Fall::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	AttackType = BaseGamePlayTags::Player_Ability_Attack_Melee_Fall;

	GetPlayerCharacterFromActorInfo()->bAttackSlam = true;
	TArray<AActor*> Ignores;
	FHitResult Hit;

	MovementFix(true);

	// 라인트레이스 계산
	FRotator Rotator = GetPlayerCharacterFromActorInfo()->GetActorRotation();
	Rotator.Pitch -= 60.f;
	FVector EndVector = Rotator.Vector();
	FVector Start = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector End = Start + EndVector * 10000.f;
		
	// 충돌체 판별(지형지물만)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECC_WorldStatic));
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, false, Ignores, EDrawDebugTrace::None, Hit, true);

	// 방향 계산
	FVector Direction = Hit.Location - Start;
	FVector DirectionNormal = Direction.GetSafeNormal();
	FVector LaunchVelocity = DirectionNormal * 1500.f;
	GetPlayerCharacterFromActorInfo()->LaunchCharacter(LaunchVelocity, true, true);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_WaitGameplayEvent* TaskToFinish = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			BaseGamePlayTags::Shared_Event_Land
			);
	TaskToFinish->EventReceived.AddDynamic(this, &ThisClass::SetNextSection);
	TaskToFinish->ReadyForActivation();
}

void UPGA_Attack_Melee_Fall::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	MovementFix(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UPGA_Attack_Melee_Fall::GetNextSection()
{
	CurrentSection++;
	if (CurrentSection > MaxSection)
	{
		CurrentSection = 1;
	}
	return *FString::Printf(TEXT("%d"), CurrentSection);
}

void UPGA_Attack_Melee_Fall::SetNextSection(FGameplayEventData Data)
{
	MontageJumpToSection(GetNextSection());
}
