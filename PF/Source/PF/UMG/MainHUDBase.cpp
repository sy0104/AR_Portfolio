// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "PlayerInfoBase.h"
#include "InventoryBase.h"
#include "PotionBase.h"
#include "PlayerStatBase.h"

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerInfo = Cast<UPlayerInfoBase>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
	mInventory = Cast<UInventoryBase>(GetWidgetFromName(FName(TEXT("Inventory"))));
	mPotionInfo = Cast<UPotionBase>(GetWidgetFromName(FName(TEXT("PotionInfo"))));
	mPlayerStat = Cast<UPlayerStatBase>(GetWidgetFromName(FName(TEXT("PlayerStat"))));
}

void UMainHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUDBase::SetHP(float Ratio)
{
	mPlayerInfo->SetHP(Ratio);
}

void UMainHUDBase::SetMP(float Ratio)
{
	mPlayerInfo->SetMP(Ratio);
}

void UMainHUDBase::SetExp(float Ratio)
{
	mPlayerInfo->SetExp(Ratio);
}

void UMainHUDBase::SetLevel(int Level)
{
	mPlayerInfo->SetLevel(Level);
}

void UMainHUDBase::SetPlayerStatUI(FPlayerInfo PlayerInfo)
{
	mPlayerStat->SetAttackText(PlayerInfo.AttackPoint);
	mPlayerStat->SetArmorText(PlayerInfo.ArmorPoint);
	mPlayerStat->SetHPText(PlayerInfo.HP);
	mPlayerStat->SetMPText(PlayerInfo.MP);
	mPlayerStat->SetHPMaxText(PlayerInfo.HPMax);
	mPlayerStat->SetMPMaxText(PlayerInfo.MPMax);
	mPlayerStat->SetGoldText(PlayerInfo.Gold);
}

void UMainHUDBase::SetGold(int32 Gold)
{
	mPlayerStat->SetGoldText(Gold);
}

void UMainHUDBase::SetEquipWeaponImage(FString& Path)
{
	mPlayerStat->SetEquipWeaponImage(Path);
}

void UMainHUDBase::SetEquipArmorImage(FString& Path)
{
	mPlayerStat->SetEquipArmorImage(Path);
}

void UMainHUDBase::SetEquipAccesaryImage(FString& Path)
{
	mPlayerStat->SetEquipAccesaryImage(Path);
}

void UMainHUDBase::SetUnEquipWeapon(int32 AttackBuff)
{
	mPlayerStat->SetUnEquipWeapon(AttackBuff);
}

void UMainHUDBase::SetUnEquipArmor(int32 ArmorBuff)
{
	mPlayerStat->SetUnEquipArmor(ArmorBuff);
}

void UMainHUDBase::SetUnEquipAccesary(int32 AttackBuff, int32 ArmorBuff)
{
	mPlayerStat->SetUnEquipAccesary(AttackBuff, ArmorBuff);
}

void UMainHUDBase::SetAttackBuffText(int32 AttackBuff)
{
	mPlayerStat->SetAttackBuffTextOn(AttackBuff);
}

void UMainHUDBase::SetArmorBuffText(int32 ArmorBuff)
{
	mPlayerStat->SetArmorBuffTextOn(ArmorBuff);
}

void UMainHUDBase::SetAttackBuffOff(int32 AttackBuff)
{
	mPlayerStat->SetAttackBuffTextOff(AttackBuff);
}

void UMainHUDBase::SetArmorBuffOff(int32 ArmorBuff)
{
	mPlayerStat->SetArmorBuffTextOff(ArmorBuff);
}

void UMainHUDBase::SetPlayerStatAttackPoint(int32 AttackPoint)
{
	mPlayerStat->SetAttackText(AttackPoint);
}

void UMainHUDBase::SetPlayerStatArmorPoint(int32 ArmorPoint)
{
	mPlayerStat->SetArmorText(ArmorPoint);
}

void UMainHUDBase::SetPlayerStatHP(int32 HP)
{
	mPlayerStat->SetHPText(HP);
}

void UMainHUDBase::SetPlayerStatHPMax(int32 HPMax)
{
	mPlayerStat->SetHPMaxText(HPMax);
}

void UMainHUDBase::SetPlayerStatMP(int32 MP)
{
	mPlayerStat->SetMPText(MP);
}

void UMainHUDBase::SetPlayerStatMPMax(int32 MPMax)
{
	mPlayerStat->SetMPMaxText(MPMax);
}

void UMainHUDBase::SetHPPotionCount(int32 Count)
{
	mPotionInfo->SetHPPotionCount(Count);
}

void UMainHUDBase::SetMPPotionCount(int32 Count)
{
	mPotionInfo->SetMPPotionCount(Count);
}

void UMainHUDBase::SetAttackPotionCount(int32 Count)
{
	mPotionInfo->SetAttackPotionCount(Count);
}

void UMainHUDBase::SetArmorPotionCount(int32 Count)
{
	mPotionInfo->SetArmorPointCount(Count);
}

void UMainHUDBase::UpdatePotionCount(EItemID ID)
{
	mInventory->UpdatePotionCount(ID);
}

bool UMainHUDBase::IsPlayerStatOpen()
{
	return mPlayerStat->IsVisible();
}

void UMainHUDBase::SetPlayerStatVisible(bool visible)
{
	if (visible)
		mPlayerStat->SetVisibility(ESlateVisibility::Visible);
	else
		mPlayerStat->SetVisibility(ESlateVisibility::Hidden);
}
