// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayTags.h"

namespace BaseGamePlayTags
{
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_DoubleJump, "InputTag.DoubleJump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slide, "InputTag.Slide");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dodge, "InputTag.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold, "InputTag.Hold");
	
	// Shared Status(Dead, Idle, Jump, etc)
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Idle, "Shared.Status.Idle");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Crouch, "Shared.Status.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Jump, "Shared.Status.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Walk, "Shared.Status.Walk");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Run, "Shared.Status.Run");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Slide, "Shared.Status.Slide");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_DoubleJump, "Shared.Status.DoubleJump");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_InAir, "Shared.Status.InAir");

	// Shared Event(HitReact, Land)
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Land, "Shared.Event.Land");

	// Player Ability(Movement)
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Jump, "Player.Ability.Movement.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_DoubleJump, "Player.Ability.Movement.DoubleJump");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Slide, "Player.Ability.Movement.Slide");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Dodge, "Player.Ability.Movement.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Crouch, "Player.Ability.Movement.Crouch");

	// Player Event
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_DoubleJump, "Player.Event.DoubleJump");


	// Item Tags
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_HpPotion, "Item.Consumable.HpPotion");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_ActionPotion, "Item.Consumable.ActionPotion");
}