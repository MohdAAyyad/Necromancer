// Fill out your copyright notice in the Description page of Project Settings.


#include "LeahAnimInstance.h"
#include "NecromancerCharacter.h"

ULeahAnimInstance::ULeahAnimInstance()
{
	    forwardSpeed =
		aimForwardSpeed =
		aimRightSpeed =
		aimLeftSpeed =
		aimBackSpeed = 0.0f;
		bHit = false;

		baimMode = false;
		banimAimShoot = false;
		buseSpell1 = false;
		buseSpell2 = false;
		buseBloodSpell1 = false;
		buseBloodSpell2 = false;

		bDash = false;

		player = nullptr;
}

void ULeahAnimInstance::UpdateAnimationProperties()
{
	if (!player)
	{
		player = TryGetPawnOwner();
		if (player)
			necro = Cast<ANecromancerCharacter>(player);
	}
	else
	{
		forwardSpeed = player->GetVelocity().Size();
	}
}

void ULeahAnimInstance::ResetSpellAnimation()
{
	buseSpell1 = false;
	buseSpell2 = false;
	buseBloodSpell1 = false;
	buseBloodSpell2 = false;
	bwall = false;
	if (necro)
		necro->bCastingSpell = false;
}

void ULeahAnimInstance::ResetHit()
{
	bHit = false;
	ResetSpellAnimation(); //If player is hit, the animation gets interrupted 
	if (necro)
		necro->EndHit();
}

void ULeahAnimInstance::ResetDash()
{
	bDash = false;
}

void ULeahAnimInstance::MoveDash()
{
	if (necro)
		necro->MoveDueToDash();
}

void ULeahAnimInstance::ConjurSpell()
{
	if (necro)
		necro->ConjurSpell();
}

void ULeahAnimInstance::SpawnWall()
{
	bwall = false;
	if (necro)
	{
		necro->SpawnWall();
		necro->bCastingSpell = false;
	}
}
