// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager.generated.h"


UCLASS()
class PF_API UInventoryManager : public UObject
{
	GENERATED_BODY()
	
public:
	UInventoryManager();
	~UInventoryManager();

private:
	static UWorld*		mCurWorld;

public:
	static UInventoryManager* GetInst(UWorld* World);

	UFUNCTION(BlueprintCallable)
	static UInventoryManager* GetInst(UGameInstance* GameInst);

public:
	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool Show);

public:
	bool IsInventoryOpen();

public:
	UDataTable*				mItemTable;
	TMap<EItemID, FName>	mRowNameFromID;				// 아이템 데이터 가져오는 용도(아이템테이블)



	int32					mPrevTime;

public:
	FItemDataInfo* GetItemInfo(EItemID ItemID);

public:
	void SetItemTable(UDataTable* Table);
	void AddItem(FItemDataInfo* Item);
};
