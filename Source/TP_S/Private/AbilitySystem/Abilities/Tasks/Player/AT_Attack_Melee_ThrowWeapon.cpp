// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/Player/AT_Attack_Melee_ThrowWeapon.h"

#include "AbilitySystemComponent.h"
#include "BaseFunctionLibrary.h"
#include "BaseGameplayTags.h"
#include "DebugHelper.h"
#include "NiagaraVariant.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/Combat/Player/BasePlayerCombatComponent.h"
#include "Items/Weapons/BasePlayerWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

UAT_Attack_Melee_ThrowWeapon* UAT_Attack_Melee_ThrowWeapon::Init(UGameplayAbility* OwningAbility,
                                                                 FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UAT_Attack_Melee_ThrowWeapon* Task = NewAbilityTask<UAT_Attack_Melee_ThrowWeapon>(OwningAbility);
	Task->Tag = EventTag;
	Task->OnlyTriggerOnce = OnlyTriggerOnce;
	Task->OnlyMatchExact = OnlyMatchExact;
	Task->bTickingTask = true;
	return Task;
}

void UAT_Attack_Melee_ThrowWeapon::Activate()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
	Player = Cast<ABasePlayerCharacter>(GetAvatarActor());
	if (Player)
	{
		Weapon = Player->GetPlayerCombatComponent()->GetPlayerCurrentEquippedWeapon();
		PlayerLocation = Player->GetActorLocation();
		if (Weapon)
		{
			
			TempTargetLocation = PlayerLocation + Player->GetActorForwardVector() * 2000.f;
			
			TArray<AActor*> Ignores;
			FHitResult Hit;
			// 충돌체 판별(지형지물만)
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECC_WorldStatic));
			UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PlayerLocation, TempTargetLocation, ObjectTypes, false, Ignores, EDrawDebugTrace::None, Hit, true);
			TargetLocation = Hit.IsValidBlockingHit() ? Hit.Location : TempTargetLocation;
			
			UAbilitySystemComponent* ASC = Ability->GetAbilitySystemComponentFromActorInfo();
			if (Ability && ASC)
			{
				MyHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag).AddUObject(this, &UAT_Attack_Melee_ThrowWeapon::GameplayEventCallback);
			}
			Weapon->GetWeaponCollisionBox()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlappedStatic);
			Weapon->GetWeaponCollisionBox()->OnComponentHit.AddDynamic(this, &ThisClass::OnHitStatic);
		}
	}
}

void UAT_Attack_Melee_ThrowWeapon::GameplayEventContainerCallback(FGameplayTag MatchingTag,
	const FGameplayEventData* Payload)
{
	bHasThrow = true;
	FDetachmentTransformRules Rule(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, false);	
	Weapon->DetachFromActor(Rule);
	Weapon->SetOwner(Player);
	Weapon->SetInstigator(Player);
	
	// 장착중인 무기를 던졌을 경우 태그랑 위치를 컴뱃 컴포넌트에 저장
	Player->GetPlayerCombatComponent()->WeaponThrownLocation = TargetLocation;
	Player->GetPlayerCombatComponent()->CurrentThrownWeaponTag = Player->GetPlayerCombatComponent()->CurrentEquippedWeaponTag;
	Player->GetPlayerCombatComponent()->ToggleWeaponCollision(true);
	Player->GetPlayerCombatComponent()->CurrentEquippedWeaponTag = FGameplayTag();
	Weapon->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_Play(Weapon->ItemMontage);
}

void UAT_Attack_Melee_ThrowWeapon::TickTask(float DeltaTime)
{
	if (bHasThrow && TargetLocation != WeaponLocation)
	{
		FVector CurrentLocation = Weapon->GetActorLocation();
		WeaponLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		Weapon->SetActorLocation(WeaponLocation, true);
		Weapon->GetWeaponCollisionBox()->SetWorldLocation(WeaponLocation, true);
		
		if (float Distance = FVector::Dist(CurrentLocation, TargetLocation) <= 100.f)
		{
			TargetLocation = WeaponLocation;
			UBaseFunctionLibrary::AddGameplayTagToActorIfNone(Player, BaseGamePlayTags::Player_Status_WeaponThrown);
			if (!Timer.IsValid())
			{
				GetWorld()->GetTimerManager().SetTimer(
					Timer,
					FTimerDelegate::CreateLambda([this]()
					{
						EndTask();
					}),
					1.5f,
					false);
			}
		}
	}
}

void UAT_Attack_Melee_ThrowWeapon::OnOverlappedStatic(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* TargetPrimitiveComponent, int level, bool bBool, const FHitResult& Hits)
{
	if (!bStop)
	{
		Debug::Print(PrimitiveComponent->GetName() + " : " + UEnum::GetValueAsString(TargetPrimitiveComponent->GetCollisionObjectType()));
		Debug::Print(UEnum::GetValueAsString(TargetPrimitiveComponent->GetCollisionObjectType()));
		Debug::Print(Actor->GetName());

		if (Hits.GetActor())
		{
			//Debug::Print(Hits.GetActor()->GetName());
		}
		
		//bStop = true;
	}
}

void UAT_Attack_Melee_ThrowWeapon::OnHitStatic(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* TargetPrimitiveComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Debug::Print(PrimitiveComponent->GetName() + " : " + UEnum::GetValueAsString(TargetPrimitiveComponent->GetCollisionObjectType()));
	Debug::Print(UEnum::GetValueAsString(TargetPrimitiveComponent->GetCollisionObjectType()));
	Debug::Print(Actor->GetName());
}
