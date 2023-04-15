// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\TextBlock.h>
#include <Components\Image.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatBase.generated.h"


UCLASS()
class PF_API UPlayerStatBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock*		mAttackText;
	UTextBlock*		mArmorText;
	UTextBlock*		mHPText;
	UTextBlock*		mMPText;
	UTextBlock*		mHPMaxText;
	UTextBlock*		mMPMaxText;
	UTextBlock*		mGoldText;

	UTextBlock*		mAttackBuffText;
	UTextBlock*		mArmorBuffText;
	UTextBlock*		mAttackBuffPlusText;
	UTextBlock*		mArmorBuffPlusText;
	
	UImage*			mEquipWeaponImage;
	UImage*			mEquipArmorImage;
	UImage*			mEquipAccesaryImage;

	UTexture2D*		mEmptyTex;

	int32			mAttackBuff;
	int32			mArmorBuff;


public:
	void SetAttackText(int32 PlayerAttack)
	{
		mAttackText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerAttack)));
	}

	void SetArmorText(int32 PlayerArmor)
	{
		mArmorText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerArmor)));
	}

	void SetHPText(int32 PlayerHP)
	{
		mHPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerHP)));
	}

	void SetMPText(int32 PlayerMP)
	{
		mMPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerMP)));
	}

	void SetHPMaxText(int32 PlayerHPMax)
	{
		mHPMaxText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerHPMax)));
	}

	void SetMPMaxText(int32 PlayerMPMax)
	{
		mMPMaxText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerMPMax)));
	}

	void SetGoldText(int32 Gold)
	{
		mGoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}

	void SetAttackBuffTextOn(int32 AttackBuff)
	{
		mAttackBuff += AttackBuff;

		mAttackBuffText->SetVisibility(ESlateVisibility::Visible);
		mAttackBuffText->SetText(FText::FromString(FString::Printf(TEXT("%d"), mAttackBuff)));
		mAttackBuffPlusText->SetVisibility(ESlateVisibility::Visible);
	}

	void SetArmorBuffTextOn(int32 ArmorBuff)
	{
		mArmorBuff += ArmorBuff;

		mArmorBuffText->SetVisibility(ESlateVisibility::Visible);
		mArmorBuffText->SetText(FText::FromString(FString::Printf(TEXT("%d"), mArmorBuff)));
		mArmorBuffPlusText->SetVisibility(ESlateVisibility::Visible);
	}

	void SetEquipWeaponImage(const FString& Path)
	{
		UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *Path);

		if (IsValid(Tex2D))
		{
			mEquipWeaponImage->SetBrushFromTexture(Tex2D);
		}
	}

	void SetEquipArmorImage(const FString& Path)
	{
		UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *Path);

		if (IsValid(Tex2D))
		{
			mEquipArmorImage->SetBrushFromTexture(Tex2D);
		}
	}

	void SetEquipAccesaryImage(const FString& Path)
	{
		UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *Path);

		if (IsValid(Tex2D))
		{
			mEquipAccesaryImage->SetBrushFromTexture(Tex2D);
		}
	}

public:
	void SetAttackBuffTextOff(int32 AttackBuff)
	{
		mAttackBuff -= AttackBuff;

		if (mAttackBuff <= 0)
		{
			mAttackBuffText->SetVisibility(ESlateVisibility::Hidden);
			mAttackBuffPlusText->SetVisibility(ESlateVisibility::Hidden);
		}

		else
			mAttackBuffText->SetText(FText::FromString(FString::Printf(TEXT("%d"), mArmorBuff)));
	}

	void SetArmorBuffTextOff(int32 ArmorBuff)
	{
		mArmorBuff -= ArmorBuff;

		if (mArmorBuff <= 0)
		{
			mArmorBuffText->SetVisibility(ESlateVisibility::Hidden);
			mArmorBuffPlusText->SetVisibility(ESlateVisibility::Hidden);
		}

		else
			mArmorBuffText->SetText(FText::FromString(FString::Printf(TEXT("%d"), mArmorBuff)));
	}

	void SetUnEquipWeapon(int32 AttackBuff)
	{
		SetAttackBuffTextOff(AttackBuff);
		mEquipWeaponImage->SetBrushFromTexture(mEmptyTex);
	}

	void SetUnEquipArmor(int32 ArmorBuff)
	{
		SetArmorBuffTextOff(ArmorBuff);
		mEquipArmorImage->SetBrushFromTexture(mEmptyTex);
	}

	void SetUnEquipAccesary(int32 AttackBuff, int32 ArmorBuff)
	{
		SetAttackBuffTextOff(AttackBuff);
		SetArmorBuffTextOff(ArmorBuff);
		mEquipAccesaryImage->SetBrushFromTexture(mEmptyTex);
	}
};
