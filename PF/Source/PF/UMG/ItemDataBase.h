// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataBase.generated.h"


UCLASS()
class PF_API UItemDataBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDataBase();
	~UItemDataBase();

private:
	FItemDataInfo* mItemInfo;

public:
	FItemDataInfo* GetItemInfo() const
	{
		return mItemInfo;
	}

	EItemID GetItemID() const
	{
		return mItemInfo->ID;
	}


public:
	void SetItemInfo(FItemDataInfo* ItemInfo)
	{
		mItemInfo = ItemInfo;
	}

	void SetItemCount(int ItemCount)
	{
		mItemInfo->ItemCount = ItemCount;
	}



};
