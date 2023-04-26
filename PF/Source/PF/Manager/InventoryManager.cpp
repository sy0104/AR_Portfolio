// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

#include "../PFGameInstance.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/InventoryBase.h"
#include "../UMG/ItemDataBase.h"
#include "../UMG/InventoryItemBase.h"
#include "../PFSaveGame.h"
#include <Components/ListView.h>

UWorld* UInventoryManager::mCurWorld = nullptr;

UInventoryManager::UInventoryManager()
{
	mPrevTime = 0;
}

UInventoryManager::~UInventoryManager()
{
}

UInventoryManager* UInventoryManager::GetInst(UWorld* World)
{
	mCurWorld = World;

	UPFGameInstance* GameInst = Cast<UPFGameInstance>(UGameplayStatics::GetGameInstance(World));

	// 인벤토리 처음으로 요청
	if (!GameInst->mInventory)
	{
		GameInst->mInventory = NewObject<UInventoryManager>();
		GameInst->mInventory->AddToRoot();
	}

	return GameInst->mInventory;
}

UInventoryManager* UInventoryManager::GetInst(UGameInstance* GameInst)
{
	UPFGameInstance* GameInstance = Cast<UPFGameInstance>(GameInst);

	// 인벤토리 처음으로 요청
	if (!GameInstance->mInventory)
	{
		GameInstance->mInventory = NewObject<UInventoryManager>();
		GameInstance->mInventory->AddToRoot();
	}

	return GameInstance->mInventory;
}

void UInventoryManager::ShowInventory(bool Show)
{
	//// 딜레이
	//int32 CurTime = 0;
	//float CurPartial = 0.f;
	//UGameplayStatics::GetAccurateRealTime(CurTime, CurPartial);

	//if (CurTime - mPrevTime < 1)
	//	return;

	//mPrevTime = CurTime;

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));

	if (!GameMode)
		return;

	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InventoryWidget = MainHUD->GetInventoryWidget();

	// Inventory On
	if (Show)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = mCurWorld->GetFirstPlayerController();

		//FInputModeUIOnly uiOnly;
		FInputModeGameAndUI GameAndUI;
		Controller->SetInputMode(GameAndUI);
		Controller->bShowMouseCursor = true;
	}

	// Inventory Off
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		APlayerController* Controller = mCurWorld->GetFirstPlayerController();

		FInputModeGameOnly GameOnly;
		Controller->SetInputMode(GameOnly);
		Controller->bShowMouseCursor = true;
	}
}

bool UInventoryManager::IsInventoryOpen()
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));

	if (!GameMode)
		return false;

	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UInventoryBase* InventoryWidget = MainHUD->GetInventoryWidget();

	return InventoryWidget->IsVisible();
}

FItemDataInfo* UInventoryManager::GetItemInfo(EItemID ItemID)
{
	// ItemID에 연결해둔 RowName을 찾아서 데이터 테이블에서 검색한다.
	FName RowName = mRowNameFromID.FindRef(ItemID);
	FItemDataInfo* ItemInfo = mItemTable->FindRow<FItemDataInfo>(RowName, nullptr);

	return ItemInfo;
}

void UInventoryManager::SetItemTable(UDataTable* Table)
{
	mItemTable = Table;

	// 데이터 테이블에 들어있는 모든 행 이름
	FString str;
	TArray<FItemDataInfo*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataInfo>(str, AllItemInfo);

	// 데이터 테이블에 들어있는 모든 행 이름
	TArray<FName> AllRowName;
	AllRowName = mItemTable->GetRowNames();

	// 테이블 정보를 받아와서 반복문을 돈다.
	// ItemID를 키값으로, 해당 아이템의 RowName을 데이터로 연결한다.
	int Count = AllItemInfo.Num();

	for (int i = 0; i < Count; ++i)
	{
		mRowNameFromID.Add(AllItemInfo[i]->ID, AllRowName[i]);
	}
}

void UInventoryManager::AddItem(FItemDataInfo* Item)
{
	//mInventoryItem.Add(Item->ID, Item);

	// 아이템 중복 체크
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();
	TArray<UObject*> TileViewItems;
	int32 Count = 0;

	if (IsValid(TileView))
	{
		TileViewItems = TileView->GetListItems();
		Count = TileViewItems.Num();
	}

	else
		return;

	int32 Index, ItemCount;
	bool bCheck = false;

	// Potion(소모품) 종류는 중복체크 O, 장착 아이템은 중복체크 X
	for (int32 i = 0; i < Count; ++i)
	{
		if (Cast<UItemDataBase>(TileViewItems[i])->GetItemInfo()->ID == Item->ID)
		{
			Index = i;
			ItemCount = Cast<UItemDataBase>(TileViewItems[i])->GetItemInfo()->ItemCount;

			if (ItemCount >= 1 && Item->ItemType == EItemType::Potion)
			{
				bCheck = true;
				break;
			}
		}
	}

	if (bCheck)	// 중복된 아이템이 있는 경우
	{
		UItemDataBase* IndexItem;
		IndexItem = Cast<UItemDataBase>(TileView->GetItemAt(Index));

		if (IsValid(IndexItem))
		{
			// TilewView 중복된 아이템 개수 변경 (ItemCount)
			IndexItem->SetItemCount(ItemCount + 1);

			// UI에 표시된 아이탬 개수 변경
			UInventoryItemBase* ItemBase =
				Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(IndexItem));

			if (IsValid(ItemBase))
				ItemBase->SetItemCountText(ItemCount + 1);
		}
	}

	else
	{
		// TileView에 아이템 새로 추가
		UItemDataBase* NewData = NewObject<UItemDataBase>();

		NewData->SetItemInfo(Item);
		NewData->SetItemCount(1);
		TileView->AddItem(NewData);
	}

	UpdateInventoryItems(TileViewItems);
}

void UInventoryManager::SaveInventory()
{
	UPFSaveGame* SaveGame = NewObject<UPFSaveGame>();

	UInventoryBase* InventoryBase = NewObject<UInventoryBase>();
	UTileView* TileView = InventoryBase->GetTileView();

	if (IsValid(TileView))
	{
		TArray<UObject*> TileViewItems = TileView->GetListItems();
		//SaveGame->mInven = TileViewItems;

		int32 Count = TileViewItems.Num();
		SaveGame->mInvenSize = Count;

		for (int i = 0; i < Count; ++i)
		{
			FSaveItemInfo SaveItem;
			UItemDataBase* Item = Cast<UItemDataBase>(TileView->GetItemAt(i));
			SaveGame->mInven.Add(Item);
		}
	}
}
