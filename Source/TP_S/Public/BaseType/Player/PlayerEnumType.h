#pragma once

UENUM(BlueprintType)
enum class EAbility : uint8
{
	None        UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel      UMETA(DisplayName = "Cancel"),
	Attack      UMETA(DisplayName = "Attack"),
	Jump        UMETA(DisplayName = "Jump"),
	DoubleJump	UMETA(DisplayName = "DoubleJump"),
	Dash        UMETA(DisplayName = "Dash"),
	Dodge       UMETA(DisplayName = "Dodge"),
	Slide       UMETA(DisplayName = "Slide"),
	Special     UMETA(DisplayName = "Special"),
};

UENUM(BlueprintType)
enum class EInventoryCategory : uint8
{
	None,
	Consumable,
	Melee,
	Ranged
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	None	UMETA(DisplayName = "None"),
	Idle	UMETA(DisplayName = "Idle"),
	Run		UMETA(DisplayName = "Run"),
	Attack	UMETA(DisplayName = "Attack"),
	InAir	UMETA(DisplayName = "InAir"),
	Slide	UMETA(DisplayName = "Slide"),
	Dodge	UMETA(DisplayName = "Dodge"),
};

UENUM(BlueprintType)
enum class EAnimEnumCardinalDirection : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Backward	UMETA(DisplayName = "Backward"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EAnimEnum_RootYawOffsetMode : uint8
{
	BlendOut	UMETA(DisplayName = "BlendOut"),
	Hold		UMETA(DisplayName = "Hold"),
	Accumulate	UMETA(DisplayName = "Accumulate"),
};