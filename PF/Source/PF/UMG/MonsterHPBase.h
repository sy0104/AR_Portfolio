// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBase.generated.h"

/**
 * 
 */
UCLASS()
class PF_API UMonsterHPBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UProgressBar*	mHPBar;
	float			mInitRatio;

public:
	void SetInitHP(float Ratio)
	{
		mInitRatio = Ratio;
	}

	void SetHP(float Ratio)	// BeginPlay에서 호출 X
	{
		if (IsValid(mHPBar))
			mHPBar->SetPercent(Ratio);
	}
};
