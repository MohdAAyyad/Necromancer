// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcheressAnimInstance.h"
#include "Archeress.h"

UArcheressAnimInstance::UArcheressAnimInstance()
{
	bAttack0 = false;
	bCast0 = false;
	bDeath = false;
	bHit = false;
	bShoot = false;
	bZombify = false;
}
void UArcheressAnimInstance::UpdateAnimationProperties()
{
	if (!owner)
	{
		owner = TryGetPawnOwner();
		if(owner)
			archeress = Cast<AArcheress>(owner); //Need to call functions inside archeress to spawn projectiles
	}
}
void UArcheressAnimInstance::NextAttack()
{
	if (aimShootMontage)
		Montage_Play(aimShootMontage);
}
void UArcheressAnimInstance::Death()
{
	bDeath = true;
	bAttack0 = false;
	bCast0 = false;
	bHit = false;
	bShoot = false;
}
void UArcheressAnimInstance::SpawnBP()
{
	if (archeress)
		archeress->SpawnBloodPool();
}
void UArcheressAnimInstance::NextCast()
{
	if (!bCast0)
		bCast0 = true;

	if (aimShootMontage)
		Montage_Play(aimShootMontage);
}
void UArcheressAnimInstance::ResetAttack()
{
	if (archeress)
	{
		if (bCast0) //If the enemy has used the cast ability, deactivate the particle system
		{
			bCast0 = false;
			archeress->DeactivateCastParticles();
		}
		else
		{
			archeress->SpawnRegularArrow();
		}
	}
}
void UArcheressAnimInstance::ResetCast()
{
}
void UArcheressAnimInstance::SetHit()
{
	bHit = true;
	if (aimShootMontage)
		Montage_Pause(aimShootMontage);
}
void UArcheressAnimInstance::ResetHit()
{
	bHit = false;
	if (aimShootMontage)
		Montage_Resume(aimShootMontage);
}
void UArcheressAnimInstance::CastSpell()
{
}

void UArcheressAnimInstance::SetAim()
{
	bAim = true;
}

bool UArcheressAnimInstance::IsMontagePlaying()
{
	if (aimShootMontage)
		return Montage_IsPlaying(aimShootMontage);
	return false;
}

void UArcheressAnimInstance::SetZombify()
{
	bAttack0 = false;
	bCast0 = false;
	bDeath = false;
	bHit = false;
	bShoot = false;
	bZombify = true;
}
void UArcheressAnimInstance::ResetZombify()
{
	bZombify = false;
	if (archeress)
		archeress->ActivateZombie();
}




