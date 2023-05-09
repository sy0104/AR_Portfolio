// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PF_API UMainHUDBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	class UPlayerInfoBase*		mPlayerInfo;
	class UInventoryBase*		mInventory;
	class UPotionBase*			mPotionInfo;
	class UPlayerStatBase*		mPlayerStat;
	class UContinueBase*		mContinue;
	class UGameEndBase*			mGameEnd;

	// PlayerInfoBase
public:
	void SetHP(float Ratio);
	void SetMP(float Ratio);
	void SetExp(float Ratio);
	void SetLevel(int Level);

	// PlayerStat
public:
	void SetPlayerStatUI(FPlayerInfo PlayerInfo);
	void SetGold(int32 Gold);

	void SetEquipWeaponImage(FString& Path);
	void SetEquipArmorImage(FString& Path);
	void SetEquipAccesaryImage(FString& Path);

	void SetUnEquipWeapon(int32 AttackBuff);
	void SetUnEquipArmor(int32 ArmorBuff);
	void SetUnEquipAccesary(int32 AttackBuff, int32 ArmorBuff);

	void SetAttackBuffText(int32 AttackBuff);
	void SetArmorBuffText(int32 ArmorBuff);

	void SetAttackBuffOff(int32 AttackBuff);
	void SetArmorBuffOff(int32 ArmorBuff);

	void SetPlayerStatAttackPoint(int32 AttackPoint);
	void SetPlayerStatArmorPoint(int32 ArmorPoint);
	void SetPlayerStatHP(int32 HP);
	void SetPlayerStatHPMax(int32 HPMax);
	void SetPlayerStatMP(int32 MP);
	void SetPlayerStatMPMax(int32 MPMax);

	// Potion Info
public:
	void SetHPPotionCount(int32 Count);
	void SetMPPotionCount(int32 Count);
	void SetAttackPotionCount(int32 Count);
	void SetArmorPotionCount(int32 Count);

	// Continue
public:
	void SetContinueVisible(bool Visible);

	// GameEnd
public:
	void SetGameEndVisible(bool Visible);

public:
	void UpdatePotionCount(EItemID ID);

public:
	class UInventoryBase* GetInventoryWidget()
	{
		return mInventory;
	}

	bool IsPlayerStatOpen();
	void SetPlayerStatVisible(bool visible);
};
