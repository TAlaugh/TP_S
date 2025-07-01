// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace BaseGamePlayTags
{
	// Input Tags
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_DoubleJump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slide);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Dodge);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Hold);


	// Shared Status(Dead, Idle, Jump, etc)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Idle);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Walk);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Run);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Slide);

	// Shared Event(HitReact, Land)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Land);

	// Player Ability(Movement)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Slide);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Dodge);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Crouch);


	// Item Tags
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Consumable_HpPotion);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Consumable_ActionPotion);
	
}