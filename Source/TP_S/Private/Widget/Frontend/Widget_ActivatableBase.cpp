// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Frontend/Widget_ActivatableBase.h"
#include "Controllers/FrontendController.h"

AFrontendController* UWidget_ActivatableBase::GetOwningFrontendController()
{
	if (!CachedOwningFrontendPC.IsValid())
	{
		CachedOwningFrontendPC = GetOwningPlayer<AFrontendController>();
	}

	return CachedOwningFrontendPC.IsValid() ? CachedOwningFrontendPC.Get() : nullptr;
}
