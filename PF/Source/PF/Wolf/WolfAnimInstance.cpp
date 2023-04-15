// Fill out your copyright notice in the Description page of Project Settings.


#include "WolfAnimInstance.h"

UWolfAnimInstance::UWolfAnimInstance()
{
	mAnimType = EWolfAnimType::Attack;
}

void UWolfAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UWolfAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
