#pragma once

UENUM()
enum class EBaseConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EBaseValidType : uint8
{
	Valid,
	InValid
};

UENUM()
enum class EBaseSuccessType : uint8
{
	Successful,
	Failed
};

UENUM(BlueprintType)
enum class EAbility : uint8
{
	None        UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel      UMETA(DisplayName = "Cancel"),
	Attack      UMETA(DisplayName = "Attack"),
	Jump        UMETA(DisplayName = "Jump"),
	Dash        UMETA(DisplayName = "Dash"),
	Slide       UMETA(DisplayName = "Slide"),
	Special     UMETA(DisplayName = "Special"),
};