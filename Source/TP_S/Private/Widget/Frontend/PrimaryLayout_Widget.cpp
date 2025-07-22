// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Frontend/PrimaryLayout_Widget.h"

#include "DebugHelper.h"

UCommonActivatableWidgetContainerBase* UPrimaryLayout_Widget::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisterWidgetStackMap.Contains(InTag), TEXT("Can not find the widget stack by the tag %s"), *InTag.ToString());

	return RegisterWidgetStackMap.FindRef(InTag);
}

void UPrimaryLayout_Widget::RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisterWidgetStackMap.Contains(InStackTag))
		{
			RegisterWidgetStackMap.Add(InStackTag, InStack);

			// Debug::Print(TEXT("Widget Stack Registered under the tag ") + InStackTag.ToString());
		}
	}
}
