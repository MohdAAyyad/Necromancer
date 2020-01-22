// Fill out your copyright notice in the Description page of Project Settings.


#include "LeahAnimInstance.h"

ULeahAnimInstance::ULeahAnimInstance()
{
	    forwardSpeed =
		aimForwardSpeed =
		aimRightSpeed =
		aimLeftSpeed =
		aimBackSpeed = 0.0f;

		baimMode = false;

		player = nullptr;
}

void ULeahAnimInstance::UpdateAnimationProperties()
{
	if (!player)
	{
		player = TryGetPawnOwner();
	}
	else
	{
		forwardSpeed = player->GetVelocity().Size();
	}
}