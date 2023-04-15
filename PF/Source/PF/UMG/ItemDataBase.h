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
	//FString		mIconPath;		// ������ �̹��� ���
	//FString		mItemName;		// ������ �̸�
	//FString		mItemDesc;		// ������ ����
	//int			mItemCount;		// ������ ����

	FItemDataInfo* mItemInfo;

public:
	//const FString& GetIconPath()
	//{
	//	return mIconPath;
	//}

	FItemDataInfo* GetItemInfo() const
	{
		return mItemInfo;
	}

	//const FString& GetItemName()
	//{
	//	return mItemName;
	//}

	//const FString& GetItemDesc()
	//{
	//	return mItemDesc;
	//}

	//int GetItemCount() const
	//{
	//	return mItemCount;
	//}

public:
	void SetItemInfo(FItemDataInfo* ItemInfo)
	{
		mItemInfo = ItemInfo;
	}

	void SetItemCount(int ItemCount)
	{
		mItemInfo->ItemCount = ItemCount;
	}

	//void SetIconPath(const FString& IconPath)
	//{
	//	mIconPath = IconPath;
	//}

	//void SetItemName(const FString& Name)
	//{
	//	mItemName = Name;
	//}

	//void SetItemDesc(const FString& Desc)
	//{
	//	mItemDesc = Desc;
	//}

	//void SetItemCount(int Count)
	//{
	//	mItemCount = Count;
	//}

};
