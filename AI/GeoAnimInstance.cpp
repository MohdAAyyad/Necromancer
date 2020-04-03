// Fill out your copyright notice in the Description page of Project Settings.

#include "GeoAnimInstance.h"
#include "Geomancer.h"

UGeoAnimInstance::UGeoAnimInstance()
{
	geomancer = nullptr;
	forwardSpeed = 0.0f;
	bCast0 = false;
	bSpecial0 = false;
	bReload = false;
	bDeath = false;
	bHit = false;
	bZombify = false;
	bSitting = false;
}

void UGeoAnimInstance::UpdateAnimationProperties()
{
	if (!geomancer)
	{
		geomancer = Cast<AGeomancer>(TryGetPawnOwner());
	}
	else
	{
		forwardSpeed = geomancer->GetVelocity().Size();
	}
}

void UGeoAnimInstance::Reload()
{
	bReload = true;
}
void UGeoAnimInstance::Death()
{
	bDeath = true;
	bCast0 = false;
	bCast1 = false;
	bSpecial0 = false;
	bReload = false;
	bHit = false;
	bZombify = false;
	bSitting = false;
	bEvading = false;
}
void UGeoAnimInstance::SpawnBP()
{
	if (geomancer)
		geomancer->SpawnBloodPool();
}
void UGeoAnimInstance::NextCast()
{
	bCast0 = true;
	bReload = false;
}

void UGeoAnimInstance::NextCast1()
{
	bCast1 = true;
	bReload = false;		
}

void UGeoAnimInstance::NextSpecial()
{
	bSpecial0 = true;
}
void UGeoAnimInstance::ResetCast()
{
	bCast0 = false;
	bCast1 = false;
	bSpecial0 = false;
	bReload = true;
	if (geomancer)
		geomancer->EndAttack();
}
void UGeoAnimInstance::ResetReload()
{
	bReload = false;
}
void UGeoAnimInstance::SetHit()
{
	bHit = true;
}
void UGeoAnimInstance::ResetHit()
{
	bHit = false;
	if (geomancer)
		geomancer->EndHit();
}
void UGeoAnimInstance::SetZombify()
{
	bZombify = true;
	bDeath = false;
	bCast0 = false;
	bCast1 = false;
	bSpecial0 = false;
	bReload = false;
	bHit = false;
	bSitting = false;
	bEvading = false;
}
void UGeoAnimInstance::ResetZombify()
{
	bZombify = false;
	if (geomancer)
		geomancer->ActivateZombie();
}

void UGeoAnimInstance::SetSitting()
{
	bSitting = true;
}
void UGeoAnimInstance::ResetSitting()
{
	bSitting = false;
	if (geomancer)
		geomancer->EndSitting();
}

void UGeoAnimInstance::SpawnProjectiles(int index_)
{
	if (index_ == 0)
	{
		if (geomancer)
			geomancer->SpawnCastProjectile();
	}
	else if (index_ == 1)
	{
		if (geomancer)
			geomancer->SpawnCastProjectile1();
	}
	else if (index_ == 2)
	{
		if (geomancer)
			geomancer->SpawnSpecialProjectile();
	}
}

void UGeoAnimInstance::SetEvading()
{
	bEvading = true;
	bDeath = false;
	bCast0 = false;
	bCast1 = false;
	bSpecial0 = false;
	bReload = false;
	bHit = false;
}
void UGeoAnimInstance::ResetEvading()
{
	bEvading = false;
	bDeath = false;
	bCast0 = false;
	bCast1 = false;
	bSpecial0 = false;
	bReload = false;
	bHit = false;
}

void UGeoAnimInstance::EvadeTeleport()
{
	if (geomancer)
		geomancer->EvadeTeleport();
}