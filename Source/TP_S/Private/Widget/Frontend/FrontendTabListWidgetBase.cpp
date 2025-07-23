// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Frontend/FrontendTabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "Widget/Frontend/FrontendCommonButtonBase.h"

#if WITH_EDITOR
void UFrontendTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class to function properly")
		));
	}
}
#endif

