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

UENUM()
enum class EBaseCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EBaseCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

