// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PotionBase.generated.h"

/**
 * 
 */
UCLASS()
class PF_API UPotionBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock*		mHPPotionCount;
	UTextBlock*		mMPPotionCount;
	UTextBlock*		mAttackPotionCount;
	UTextBlock*		mArmorPotionCount;

public:
	void SetHPPotionCount(int32 Count);
	void SetMPPotionCount(int32 Count);
	void SetAttackPotionCount(int32 Count);
	void SetArmorPointCount(int32 Count);
};
