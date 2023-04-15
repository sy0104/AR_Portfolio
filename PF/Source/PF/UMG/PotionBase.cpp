// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionBase.h"

void UPotionBase::NativeConstruct()
{
	Super::NativeConstruct();

	mHPPotionCount = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("HPPotionCount"))));
	mMPPotionCount = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("MPPotionCount"))));
	mAttackPotionCount = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("AttackPotionCount"))));
	mArmorPotionCount = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ArmorPotionCount"))));

}

void UPotionBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UPotionBase::SetHPPotionCount(int32 Count)
{
	mHPPotionCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}

void UPotionBase::SetMPPotionCount(int32 Count)
{
	mMPPotionCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}

void UPotionBase::SetAttackPotionCount(int32 Count)
{
	mAttackPotionCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}

void UPotionBase::SetArmorPointCount(int32 Count)
{
	mArmorPotionCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
}
