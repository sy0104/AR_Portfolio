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
	FEquipedItem	mEquipedItem;

	UPROPERTY()
	float			mHPRatio;

	UPROPERTY()
	float			mMPRatio;

	UPROPERTY()
	float			mExpRatio;
};
