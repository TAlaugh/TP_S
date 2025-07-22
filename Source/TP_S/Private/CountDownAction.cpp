// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownAction.h"

void FBaseCountDownAction::UpdateOperation(FLatentResponse& Response)
{
	
	if (bNeedToCancel)
	{
		CountDownOutput = EBaseCountDownActionOutput::Cancelled;
	
		Response.FinishAndTriggerIf(true, ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}


	if (ElapsedTimeSinceStart >=TotalCountDownTime)
	{
		CountDownOutput = EBaseCountDownActionOutput::Completed;
	
		Response.FinishAndTriggerIf(true, ExecutionFunction,OutputLink,CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f? UpdateInterval : Response.ElapsedTime();
		
		OutRemainingTime = TotalCountDownTime - ElapsedTimeSinceStart;

		CountDownOutput = EBaseCountDownActionOutput::Updated;
		
		Response.TriggerLink(ExecutionFunction,OutputLink,CallbackTarget);
		
		ElapsedInterval = 0.f;
	}
	
}

void FBaseCountDownAction::CancelAction()
{
	bNeedToCancel = true;
}
