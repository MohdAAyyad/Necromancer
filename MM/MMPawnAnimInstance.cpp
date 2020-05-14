// Fill out your copyright notice in the Description page of Project Settings.


#include "MMPawnAnimInstance.h"

UMMPawnAnimInstance::UMMPawnAnimInstance()
{
	bRise = false;
	bEnemyRise = false;
}
void UMMPawnAnimInstance::UpdateAnimationProperties()
{

}

void UMMPawnAnimInstance::SetRise(bool value_)
{
	bRise = value_;
	if (bRise)
		Montage_Play(sitToStandMontage);
}

void UMMPawnAnimInstance::SetEnemyRise(bool value_)
{
	bEnemyRise = value_;
}