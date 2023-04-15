// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoBase.h"

void UPlayerInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	mHPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HPBar"))));
	mMPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("MPBar"))));
	mExpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("ExpBar"))));
	mLevelText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("LevelText"))));
	mGoldText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("GoldText"))));
}

void UPlayerInfoBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerInfoBase::SetHP(float Ratio)
{
	mHPBar->SetPercent(Ratio);
}

void UPlayerInfoBase::SetMP(float Ratio)
{
	mMPBar->SetPercent(Ratio);
}

void UPlayerInfoBase::SetExp(float Ratio)
{
	mExpBar->SetPercent(Ratio);
}

void UPlayerInfoBase::SetLevel(int Level)
{
	mLevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Level)));
}
