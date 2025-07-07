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
