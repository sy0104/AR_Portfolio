// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "GameFramework/SaveGame.h"
#include "UMG/ItemDataBase.h"
#include "PFSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PF_API UPFSaveGame : public USaveGame
{
	GENERATED_BODY()

	friend class APlayerCharacter;
	friend class APFGameModeBase;
	friend class UInventoryManager;
	
private:
	UPROPERTY()
	FPlayerInfo		mPlayerInfo;

	UPROPERTY()
	float			mCameraZoomMin;

	UPROPERTY()
	float			mCameraZoomMax;

	UPROPERTY()
	float			mHPRatio;

	UPROPERTY()
	float			mMPRatio;

	UPROPERTY()
	float			mExpRatio;

	UPROPERTY()
	int32			mHPPotionCount;

	UPROPERTY()
	int32			mMPPotionCount;

	UPROPERTY()
	int32			mAttackPotionCount;

	UPROPERTY()
	int32			mArmorPotionCount;

	//UPROPERTY()
	//FEquipedItem	mEquipedItem;

	UPROPERTY()
	int32			mEquipedWeaponIndex;

	UPROPERTY()
	int32			mEquipedArmorIndex;

	UPROPERTY()
	int32			mEquipedAccesaryIndex;

};
