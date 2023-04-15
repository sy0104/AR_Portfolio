// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatBase.h"

void UPlayerStatBase::NativeConstruct()
{
	Super::NativeConstruct();

	mAttackText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("AttackText"))));
	mArmorText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ArmorText"))));
	mHPText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("HPText"))));
	mMPText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("MPText"))));
	mHPMaxText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("HPMaxText"))));
	mMPMaxText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("MPMaxText"))));
	mGoldText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("GoldText"))));

	mAttackBuffText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("AttackBuffText"))));
	mArmorBuffText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ArmorBuffText"))));
	mAttackBuffPlusText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("AttackBuffPlusText"))));
	mArmorBuffPlusText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ArmorBuffPlusText"))));

	mEquipWeaponImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("EquipWeaponImage"))));
	mEquipArmorImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("EquipArmorImage"))));
	mEquipAccesaryImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("EquipAccesaryImage"))));

	const FString& Path = TEXT("Texture2D'/Game/Texture/Dark_Brown_GUI__Kit/Background/big_dark_background1.big_dark_background1'");
	mEmptyTex = LoadObject<UTexture2D>(nullptr, *Path);

	mAttackBuffText->SetVisibility(ESlateVisibility::Hidden);
	mArmorBuffText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerStatBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
