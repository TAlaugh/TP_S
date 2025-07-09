// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/BasePlayerLinkedAnimLayer.h"

#include "AnimInstances/Player/BasePlayerAnimInstance.h"

UBasePlayerAnimInstance* UBasePlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UBasePlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
