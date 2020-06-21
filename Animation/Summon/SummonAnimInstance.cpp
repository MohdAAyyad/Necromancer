// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonAnimInstance.h"
#include "../../Spells/Blood/SummonBase.h"

USummonAnimInstance::USummonAnimInstance()
{
	 forwardSpeed = 0.0f;
	 summonOwner = nullptr;
	 bActive = bDead = bReload = bAttack = false;
}

void USummonAnimInstance::UpdateAnimationProperties()
{
	if (!summonOwner)
	{
		summonOwner = Cast<ASummonBase>(TryGetPawnOwner());
	}
	else
	{
		forwardSpeed = summonOwner->GetVelocity().Size();
	}
}

void USummonAnimInstance::ActivateSummon()
{
	bActive = true;
	if (summonOwner)
		summonOwner->ActivateSummon();
}

void USummonAnimInstance::EnableHitBox()
{
	if (summonOwner)
		summonOwner->EnableAttackHitBox();
}
void USummonAnimInstance::DisableHitBox()
{
	bAttack = false;
	bReload = true;
	if (summonOwner)
		summonOwner->DisableAttackHitBox();
}

void USummonAnimInstance::NextAttack()
{
	bAttack = true;
}

void USummonAnimInstance::ResetReload()
{
	bReload = false;
}

void USummonAnimInstance::Death()
{
	bDead = true;
	bActive = false;
	if (summonOwner)
		summonOwner->LeaveScene();
}

void USummonAnimInstance::EndHit()
{
	bHit = false;
}