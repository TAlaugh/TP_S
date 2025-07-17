// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PreviewPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/BasePlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APreviewPlayerCharacter::APreviewPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	GetMesh()->PrimaryComponentTick.bTickEvenWhenPaused = true;

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->DisableMovement();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// CameraBoom->SetupAttachment(GetRootComponent());
	// CameraBoom->TargetArmLength = -200.0f;
	// CameraBoom->SocketOffset = FVector(0.f, -30.f, 30.f);
	// CameraBoom->bUsePawnControlRotation = false;
	//
	// FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// FollowCamera->SetRelativeRotation(FRotator(-180.f, 0.f, 180.f));
	// FollowCamera->bUsePawnControlRotation = false;

	PreviewWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewWeapon"));
	PreviewWeapon->SetupAttachment(GetMesh());
	PreviewWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PreviewWeapon->SetVisibility(false);
	PreviewWeapon->PrimaryComponentTick.bTickEvenWhenPaused = true;

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(GetRootComponent());
	SceneCapture->bCaptureEveryFrame = true;
	SceneCapture->bCaptureOnMovement = false;
	SceneCapture->SetTickableWhenPaused(true);
	SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	// SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCapture->TextureTarget = RenderTarget;
	SceneCapture->SetRelativeRotation(FRotator(-180.f, 0.f, 180.f));
}

void APreviewPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void APreviewPlayerCharacter::PlayEnterMontage()
{
	if (!PreviewEnterMontage || !GetMesh()) return;

	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		Anim->Montage_Play(PreviewEnterMontage);
	}
}

void APreviewPlayerCharacter::CopyMeshFromPlayer(ABasePlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter) return;

	USkeletalMeshComponent* SourceMesh = PlayerCharacter->GetMesh();
	if (SourceMesh)
	{
		GetMesh()->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
		GetMesh()->SetAnimInstanceClass(SourceMesh->AnimClass);
	}
}

void APreviewPlayerCharacter::AttachWeaponMeshComponent(USkeletalMesh* WeaponMesh, FName SocketName)
{
	if (!WeaponMesh) return;

	PreviewWeapon->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	PreviewWeapon->SetSkeletalMesh(WeaponMesh);
	PreviewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	PreviewWeapon->SetVisibility(true);
}

void APreviewPlayerCharacter::PlayEquipMontage(FGameplayTag WeaponTag)
{
	if (!GetMesh()) return;

	if (UAnimMontage** FoundMontage = EquipMontageMap.Find(WeaponTag))
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			if (Anim->Montage_IsPlaying(*FoundMontage) == false)
			{
				Anim->Montage_Play(*FoundMontage);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Montage found for tag: %s"), *WeaponTag.ToString());
	}
}

