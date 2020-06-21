// Fill out your copyright notice in the Description page of Project Settings.


#include "CryptoAnimInstance.h"
#include "Crypto.h"


UCryptoAnimInstance::UCryptoAnimInstance()
{
	crypto = nullptr;
	bCast0 = false;
	bSpecial0 = false;
	bReload = false;
	bDeath = false;
	bHit = false;
	bZombify = false;
}
void UCryptoAnimInstance::UpdateAnimationProperties()
{
	if (!crypto)
	{
		crypto = Cast<ACrypto>(TryGetPawnOwner());
	}
}

void UCryptoAnimInstance::Reload()
{
	bReload = true;
	bCast0 = false;
	bSpecial0 = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload has been called"));
}
void UCryptoAnimInstance::Death()
{
	bDeath = true;
	bCast0 = false;
	bSpecial0 = false;
	bReload = false;
	bHit = false;
	bZombify = false;
	UE_LOG(LogTemp, Warning, TEXT("Death has been called"));
}
void UCryptoAnimInstance::SpawnBP()
{
	if (crypto)
		crypto->SpawnBloodPool();
}
void UCryptoAnimInstance::NextCast()
{
	bCast0 = true;
	bReload = false;
	bSpecial0 = false;
	bHit = false;
	UE_LOG(LogTemp, Warning, TEXT("Next Cast has been called"));
}

void UCryptoAnimInstance::NextSpecial()
{
	bSpecial0 = true;
	bReload = false;
	bHit = false;
	bCast0 = false;
	UE_LOG(LogTemp,Warning, TEXT("Next Special has been called"));
}

void UCryptoAnimInstance::ResetSpecial()
{
	bSpecial0 = false;
	UE_LOG(LogTemp, Warning, TEXT("Reset special has been called"));
}
void UCryptoAnimInstance::ResetCast()
{
	bCast0 = false;
	bSpecial0 = false;
	bReload = true;

}
void UCryptoAnimInstance::ResetReload()
{
	bReload = false;

}
void UCryptoAnimInstance::SetHit()
{
	if (!bHit)
		bHit = true;
	
}
void UCryptoAnimInstance::ResetHit()
{
	bHit = false;

}
void UCryptoAnimInstance::SetZombify()
{
	bZombify = true;
	bDeath = false;
	bCast0 = false;
	bSpecial0 = false;
	bReload = false;
	bHit = false;
}
void UCryptoAnimInstance::ResetZombify()
{
	bZombify = false;
	if (crypto)
		crypto->ActivateZombie();
}

void UCryptoAnimInstance::EndAbsorb()
{
	if (crypto)
		crypto->EndAbsorb();
}