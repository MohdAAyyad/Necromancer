// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerAnimInstance.h"
#include "../../NPC/PrisonerQuestNPC.h"

UPrisonerAnimInstance::UPrisonerAnimInstance():UAnimInstance()
{
	prisoner = nullptr;
	bDead = false;
	bZombie = false;
	bThanks = false;
}

void UPrisonerAnimInstance::UpdateAnimationProperties()
{
	if (!prisoner)
	{
		prisoner = Cast<APrisonerQuestNPC>(TryGetPawnOwner());
	}
	else
	{
		forwardSpeed = prisoner->GetVelocity().Size();
	}
}

void UPrisonerAnimInstance::SetDead()
{
	bDead = true;
}
void UPrisonerAnimInstance::SetZombie()
{
	bZombie = true;
}
void UPrisonerAnimInstance::SetThanks(bool value_)
{
	bThanks = value_;
}

void UPrisonerAnimInstance::ActivateZombie()
{
	if (prisoner)
		prisoner->ActivateZombie();

	bDead = false;
	bZombie = false;
}