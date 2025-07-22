 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace	BaseGamePlayTags
{
	// FrontendGameplay Tags
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHud);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);

	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
	
	// Input Tags
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_DoubleJump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slide);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Dodge);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Hold);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Melee_Light);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Melee_Heavy);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Melee_ReceiveWeapon);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Melee_Fall);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Range);


	// Shared Status(Dead, Idle, Jump, etc)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Idle);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Crouch);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Walk);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Run);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_DoubleJump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Slide);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_InAir);

	
	// Shared Event(HitReact, Land)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Hit);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Hit_Melee);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Hit_Range);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Land);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_SpawnProjectile);


	// Shared Ability(HitReact, Death)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

	
	// Shared Status(Dead)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);

	
	// Shared SetByCaller(BaseDamage)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);


	//Shared Direction
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Front)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Left)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Right)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_HitReact_Back)

	
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability);

	// Player Ability(Movement)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Jump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_DoubleJump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Slide);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Dodge);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Movement_Crouch);

	// Player Ability(Attack)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Melee);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Melee_Light);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Melee_Heavy);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Melee_Fall);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Melee_ReceiveWeapon);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Range);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Range_Single);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Range_Stream);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Melee);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Range);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_SpawnWeapon);

	// Player Event
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_DoubleJump);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Next);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Finish);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Throw);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Equip);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Equip_Right);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Equip_Left);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_Equip_Switch);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Attack_UnEquip);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Melee);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Melee_PoleArm);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Range);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Range_MachineGun);

	// Player Status
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_WeaponThrown);


	// Item Tags
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Consumable_HpPotion);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Consumable_ActionPotion);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipable_Weapon);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipable_Weapon_Melee);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipable_Weapon_Melee_PoleArm);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipable_Weapon_Range);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equipable_Weapon_Range_MachineGun);

	// Item GE
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Effect_HpPercent);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Effect_HpFloat);
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Effect_AtkFloat);
	
	// Enemy Tag
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon)

	
	// Enemy Ability
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Ranged)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_1)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_2)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_3)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_Lianhuo_SpawnSkill)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_Lianhuo_SpawnSkill2)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_Lianhuo_SpawnEnemy_Skeleton)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Skill_Lianhuo_Rage)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_SpawnEnemy)

	//Enemy Event
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_SpawnSkill)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_SpawnEnemy)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Event_Rage)
	
	// Enemy Status
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_UnderAttack)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Rage)


	// Gameplay Cue
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FX_Hit_Base)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FX_Hit_Melee)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FX_Hit_Melee_PoleArm)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FX_Hit_Range)
	TP_S_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_FX_Hit_Range_MachineGun)
}