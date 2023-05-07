// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "PFGameInstance.generated.h"

class UInventoryManager;

UCLASS()
class PF_API UPFGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class UInventoryManager;
	
public:
	UPFGameInstance();
	~UPFGameInstance();

public:
	virtual void Init();

private:
	UDataTable*			mItemTable;
	UDataTable*			mMonsterTable;
	UDataTable*			mMonsterSkillInfoTable;
	UDataTable*			mPlayerSkillInfoTable;

	UInventoryManager*	mInventory;

	TArray<FSaveItemInfo>	mSaveInven;

public:
	const FMonsterTableInfo* FindMonsterTable(const FName& Name);
	const FSkillData* FindMonsterSkillTable(const FName& Name);
	const FSkillData* FindPlayerSkillTable(const FName& Name);

public:
	void SetSaveInven(TArray<FSaveItemInfo> Inven)
	{
		mSaveInven = Inven;
	}

public:
	TArray<FSaveItemInfo> GetSaveInven()
	{
		return mSaveInven;
	}
};
