// Fill out your copyright notice in the Description page of Project Settings.


#include "MerkvillAnimInstance.h"
#include "Merkvill.h"

UMerkvillAnimInstance::UMerkvillAnimInstance()
{
	bAttack0 = false; //Fireballs	
	bAttack1 = false; //Traps	
	bAttack2 = false; //Tornadoes	
	bAttack3 = false; //Explosion	
	bDeath = false;
}

void UMerkvillAnimInstance::UpdateAnimationProperties()
{
	if (!merkvill)
	{
		merkvill = Cast<AMerkvill>(TryGetPawnOwner());
	}
}

void UMerkvillAnimInstance::NextAttack()
{
	bAttack0 = true;
}
void UMerkvillAnimInstance::NextAttack1()
{
	bAttack1 = true;
}
void UMerkvillAnimInstance::NextAttack2()
{
	bAttack2 = true;
}
void UMerkvillAnimInstance::NextAttack3()
{
	bAttack3 = true;
}

void UMerkvillAnimInstance::ResetAttack()
{
	bAttack0 = false;

	if (bAttack0 || bAttack1 || bAttack2 || bAttack3 == false) //Attacks are chained together based on the phase. This means Merkvill won't reload untill all booleans are flipped which happens when the attacks are done
	{
		if (merkvill)
			merkvill->Reload();
	}
}
void UMerkvillAnimInstance::ResetAttack1()
{
	bAttack1 = false;

	if (bAttack0 || bAttack1 || bAttack2 || bAttack3 == false)
	{
		if (merkvill)
			merkvill->Reload();
	}
}
void UMerkvillAnimInstance::ResetAttack2()
{
	bAttack2 = false;

	if (bAttack0 || bAttack1 || bAttack2 || bAttack3 == false)
	{
		if (merkvill)
			merkvill->Reload();
	}
}
void UMerkvillAnimInstance::ResetAttack3()
{
	bAttack3 = false;
	bAttack0 = false; //Explosion will happen without fireballs. Need a different boolean to tell the animation bp to play the explosion animation. Using attack0 instead of a new boolean

	if (merkvill)
		merkvill->ResetExplosionSphere();

	if (bAttack0 || bAttack1 || bAttack2 || bAttack3 == false)
	{
		if (merkvill)
			merkvill->Reload();
	}
}

void UMerkvillAnimInstance::Death()
{
	bDeath = true;
	bAttack0 = false;	
	bAttack1 = false;
	bAttack2 = false;
	bAttack3 = false;	
}
void UMerkvillAnimInstance::SpawnBP()
{
	//Does not spawn a BP instead calls Death function in Merkvill code
	if (merkvill)
		merkvill->Death();
}
void UMerkvillAnimInstance::SetHit()
{

}
void UMerkvillAnimInstance::ResetHit()
{

}
void UMerkvillAnimInstance::SetStun()
{

}
void UMerkvillAnimInstance::ResetStun()
{

}
void UMerkvillAnimInstance::Reload()
{
	bAttack0 = false;
	bAttack1 = false;
	bAttack2 = false;
	bAttack3 = false;
}

void UMerkvillAnimInstance::FireBall()
{
	if (merkvill)
		merkvill->FireBall();
}
void UMerkvillAnimInstance::Meteor()
{
	if (merkvill)
		merkvill->Meteors();
}
void UMerkvillAnimInstance::Tornado()
{
	if (merkvill)
		merkvill->Tornado();
}
void UMerkvillAnimInstance::CompleteExplosion()
{
	//Using battack0 to transition between initiateExplosion and completeExplosion
	bAttack0 = true;
}