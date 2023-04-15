// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "WolfAnimInstance.generated.h"


UCLASS()
class PF_API UWolfAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UWolfAnimInstance();

public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EWolfAnimType		mAnimType;

public:
	void SetAnimType(EWolfAnimType Anim)
	{
		mAnimType = Anim;
	}
};
