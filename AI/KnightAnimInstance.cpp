// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightAnimInstance.h"
#include "Knight.h"


UKnightAnimInstance::UKnightAnimInstance()
{
	owner = nullptr;
	forwardSpeed = 0.0f;
	 bAttack0 = false;
	 bCast0 = false;
	 bSpecial0 = false;
	 bReload = false;
	 bStrafe = false;
	 bDeath = false;
	 bHit = false;
	 bZombify = false;
}

void UKnightAnimInstance::UpdateAnimationProperties()
{
	if (!owner)
	{
		owner = TryGetPawnOwner();
		if (owner)
			knight = Cast<AKnight>(owner);
	}
	else
	{
		forwardSpeed = owner->GetVelocity().Size();
		
	}
}

void UKnightAnimInstance::NextAttack()
{
	if (!bAttack0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NEEEEXT ATTTACCKKK"));
		bAttack0 = true;
		bReload = false;
	}
}
void UKnightAnimInstance::NextSpecial()
{
	if (!bSpecial0)
	{
		bSpecial0 = true;
	}
}

void UKnightAnimInstance::NextCast()
{
	if (!bCast0)
	{
		bCast0 = true;
	}
}

void UKnightAnimInstance::Reload()
{
	bReload = true;
}
void UKnightAnimInstance::Death()
{
	bDeath = true;
	bAttack0 = false;
	bCast0 = false;
	bSpecial0 = false;
	bReload = false;
	bStrafe = false;
}

void UKnightAnimInstance::SetStrafe()
{
	bStrafe = true;
	bHit = false;
	UE_LOG(LogTemp, Warning, TEXT("Setting Strafe"));
}


void UKnightAnimInstance::ResetAttack()
{
	//Called from anim notify
	if (bAttack0)
	{
		UE_LOG(LogTemp, Warning, TEXT("STTTTOOPPPPP ATTTACCKKK"));
		bAttack0 = false;

		//Placeholder code: Current state should not be public
			if (knight)
			{
				//UE_LOG(LogTemp, Error, TEXT("RESET ATTACK"));
				knight->Reload();
			}
	}
}

void UKnightAnimInstance::ResetCast()
{
	if (bCast0)
	{
		bCast0 = false;

		if (knight)
		{
			//UE_LOG(LogTemp, Error, TEXT("RESET CAST"));
			knight->currentState = AKnight::EKnightState::RELOADING;
			knight->Reload();
			bReload = true;
		}
	}
}

void UKnightAnimInstance::ResetSpecial()
{
	if (bSpecial0)
	{
		bSpecial0 = false;
		bHit = false;
		if (knight)
		{
			knight->EndSpecial();
		}
	}
}

void UKnightAnimInstance::ResetReload()
{
	bReload = false;
}

void UKnightAnimInstance::ResetStrafe()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset strafe"));
	bStrafe = false;
}

void UKnightAnimInstance::SpawnBP()
{
	//Called from anim notify: Death animation
	if (knight)
	{
		knight->SpawnBloodPool();
	}
}

void UKnightAnimInstance::EnableAttackHitBox()
{
	//Called from anim notify
	if (knight)
	{
		knight->EnableHitBox();
	}
}

void UKnightAnimInstance::CastSpell()
{
	if (knight)
	{
		knight->SpawnCastProjectile();
	}
}

void UKnightAnimInstance::SetHit()
{
	bHit = true;
}
void UKnightAnimInstance::ResetHit()
{
	bHit = false;
	if (knight)
		knight->EndHit();
}

void UKnightAnimInstance::SetZombify()
{
	bAttack0 = false;
	bCast0 = false;
	bSpecial0 = false;
	bReload = false;
	bStrafe = false;
	bDeath = false;
	bHit = false;
	bZombify = true;
}
void UKnightAnimInstance::ResetZombify()
{
	bZombify = false;
	if (knight)
		knight->ActivateZombie();
}
