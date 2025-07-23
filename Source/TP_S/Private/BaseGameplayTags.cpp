// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayTags.h"

namespace BaseGamePlayTags
{
	// FrontendGameplay Tags
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHud, "Frontend.WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");
	
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen");
	
	// Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_DoubleJump, "InputTag.DoubleJump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Slide, "InputTag.Slide");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dodge, "InputTag.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold, "InputTag.Hold");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Melee_Light, "InputTag.Attack.Melee.Light");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Melee_Heavy, "InputTag.Attack.Melee.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Melee_ReceiveWeapon, "InputTag.Attack.Melee.ReceiveWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Melee_Fall, "InputTag.Attack.Melee.Fall");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_Range, "InputTag.Attack.Range");
	
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
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Death, "Shared.Event.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit, "Shared.Event.Hit")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit_Melee, "Shared.Event.Hit.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit_Range, "Shared.Event.Hit.Range")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Land, "Shared.Event.Land")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile")
	
	//Shared Ability(HitReact, Death)
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact,"Shared.Ability.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death,"Shared.Ability.Death")
	
	//Shared Status(Dead)
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead,"Shared.Status.Dead")


	// Shared SetByCaller(BaseDamage)
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage,"Shared.SetByCaller.BaseDamage")


	//Shared Direction
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back")
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability, "Player.Ability");
	
	// Player Ability(Movement)
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement, "Player.Ability.Movement");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Jump, "Player.Ability.Movement.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_DoubleJump, "Player.Ability.Movement.DoubleJump");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Slide, "Player.Ability.Movement.Slide");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Dodge, "Player.Ability.Movement.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Crouch, "Player.Ability.Movement.Crouch");

	// Player Ability(Attack)
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack, "Player.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Melee, "Player.Ability.Attack.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Melee_Light, "Player.Ability.Attack.Melee.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Melee_Heavy, "Player.Ability.Attack.Melee.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Melee_Fall, "Player.Ability.Attack.Melee.Fall");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Melee_ReceiveWeapon, "Player.Ability.Attack.Melee.ReceiveWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Range, "Player.Ability.Attack.Range");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Range_Single, "Player.Ability.Attack.Range.Single");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Range_Stream, "Player.Ability.Attack.Range.Stream");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Melee, "Player.Ability.Equip.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Range, "Player.Ability.Equip.Range");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpawnWeapon, "Player.Ability.SpawnWeapon");

	// Player Event
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_DoubleJump, "Player.Event.DoubleJump");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack, "Player.Event.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Next, "Player.Event.Attack.Next");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Finish, "Player.Event.Attack.Finish");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Throw, "Player.Event.Attack.Throw");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Equip, "Player.Event.Attack.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Equip_Right, "Player.Event.Attack.Equip.Right");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Equip_Left, "Player.Event.Attack.Equip.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_Equip_Switch, "Player.Event.Attack.Equip.Switch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Attack_UnEquip, "Player.Event.Attack.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip, "Player.Event.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Melee, "Player.Event.Equip.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Melee_PoleArm, "Player.Event.Equip.Melee.PoleArm");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Range, "Player.Event.Equip.Range.MachineGun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Range_MachineGun, "Player.Event.Equip.Range.MachineGun");

	// Player Status
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_WeaponThrown, "Player.Status.WeaponThrown");

	// Item Tags
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_HpPotion, "Item.Consumable.HpPotion");
	UE_DEFINE_GAMEPLAY_TAG(Item_Consumable_ActionPotion, "Item.Consumable.ActionPotion");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipable_Weapon_Melee, "Item.Equipable.Weapon.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipable_Weapon_Melee_PoleArm, "Item.Equipable.Weapon.Melee.PoleArm");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipable_Weapon_Range, "Item.Equipable.Weapon.Range");
	UE_DEFINE_GAMEPLAY_TAG(Item_Equipable_Weapon_Range_MachineGun, "Item.Equipable.Weapon.Range.MachineGun");

	// Item GE
	UE_DEFINE_GAMEPLAY_TAG(Item_Effect_HpPercent, "Item.Effect.HpPercent");
	UE_DEFINE_GAMEPLAY_TAG(Item_Effect_HpFloat, "Item.Effect.HpFloat");
	UE_DEFINE_GAMEPLAY_TAG(Item_Effect_AtkFloat, "Item.Effect.AtkFloat");
	
	// Enemy Tag
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon,"Enemy.Weapon")
	
	// Enemy Ability
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee,"Enemy.Ability.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged,"Enemy.Ability.Ranged")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill,"Enemy.Ability.Skill")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill_1,"Enemy.Ability.Skill.1")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill_2,"Enemy.Ability.Skill.2")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill_3,"Enemy.Ability.Skill.3")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill_Lianhuo_SpawnSkill,"Enemy.Ability.Skill.Lianhuo.SpawnSkill")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill_Lianhuo_SpawnSkill2,"Enemy.Ability.Skill.Lianhuo.SpawnSkill2")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SKill_Lianhuo_SpawnEnemy_Skeleton,"Enemy.Ability.Skill.Lianhuo.SpawnEnemy.Skeleton")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Skill_Lianhuo_Rage,"Enemy.Ability.Skill.Lianhuo.Rage")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SpawnEnemy,"Enemy.Ability.SpawnEnemy")
	 


	// Enemy Event
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SpawnSkill, "Enemy.Event.SpawnSkill")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SpawnEnemy, "Enemy.Event.SpawnEnemy")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Rage, "Enemy.Event.Rage")
	
	// Enemy Status
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing,"Enemy.Status.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack,"Enemy.Status.UnderAttack")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Rage, "Enemy.Status.Rage")

	// GameplayCue
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_FX_Hit_Base, "GameplayCue.FX.Hit.Base")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_FX_Hit_Melee, "GameplayCue.FX.Hit.Melee")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_FX_Hit_Melee_PoleArm, "GameplayCue.FX.Hit.Melee.PoleArm")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_FX_Hit_Range, "GameplayCue.FX.Hit.Range")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_FX_Hit_Range_MachineGun, "GameplayCue.FX.Hit.Range.MachineGun")
}



