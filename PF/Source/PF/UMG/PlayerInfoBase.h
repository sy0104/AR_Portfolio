// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>
#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoBase.generated.h"


UCLASS()
class PF_API UPlayerInfoBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UProgressBar*		mHPBar;
	UProgressBar*		mMPBar;
	UProgressBar*		mExpBar;
	UTextBlock*			mLevelText;
	UTextBlock*			mGoldText;

public:
	void SetHP(float Ratio);
	void SetMP(float Ratio);
	void SetExp(float Ratio);
	void SetLevel(int Level);
};
