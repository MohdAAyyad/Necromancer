// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenAnimInstance.h"
#include "Raven.h"

URavenAnimInstance::URavenAnimInstance()
{
	bAttack0 = false;
	bZombify = false;
	bHit = false;
	bSkeleton = false;
	bDead = false;
}

void URavenAnimInstance::UpdateAnimationProperties()
{
	if (!raven)
	{
		raven = Cast<ARaven>(TryGetPawnOwner());
	}
}

void URavenAnimInstance::NextAttack()
{
	bAttack0 = true;
}
void URavenAnimInstance::ResetAttack()
{
	bAttack0 = false;
}
void URavenAnimInstance::SetHit()
{
	bHit = true;
}
void URavenAnimInstance::ResetHit()
{
	bHit = false;
}
void URavenAnimInstance::SetZombify()
{
	bZombify = true;
}
void URavenAnimInstance::ResetZombify()
{
	bZombify = false;
	if (raven)
		raven->ActivateZombie();
}
void URavenAnimInstance::SummonSkeleton()
{
	bSkeleton = true;
	if (raven)
		raven->PlaySpawnParticles();
}

void URavenAnimInstance::SpawnSkeleton()
{
	if (raven)
		raven->SpawnSkeleton();
	bSkeleton = false;
}

void URavenAnimInstance::SpawnAimProj()
{
	if (raven)
		raven->SpawnAimProj();
}

void URavenAnimInstance::Death()
{
	bAttack0 = false;
	bZombify = false;
	bHit = false;
	bSkeleton = false;
	bDead = true;
}

void URavenAnimInstance::EndDeath()
{
	if (raven)
		raven->DestroySelf();
}