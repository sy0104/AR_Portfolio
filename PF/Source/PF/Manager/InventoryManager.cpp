// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

#include "../PFGameInstance.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/InventoryBase.h"
#include "../UMG/ItemDataBase.h"
#include "../UMG/InventoryItemBase.h"
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

	// �κ��丮 ó������ ��û
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

	// �κ��丮 ó������ ��û
	if (!GameInstance->mInventory)
	{
		GameInstance->mInventory = NewObject<UInventoryManager>();
		GameInstance->mInventory->AddToRoot();
	}

	return GameInstance->mInventory;
}

void UInventoryManager::ShowInventory(bool Show)
{
	//// ������
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
	// ItemID�� �����ص� RowName�� ã�Ƽ� ������ ���̺����� �˻��Ѵ�.
	FName RowName = mRowNameFromID.FindRef(ItemID);
	FItemDataInfo* ItemInfo = mItemTable->FindRow<FItemDataInfo>(RowName, nullptr);

	return ItemInfo;
}

void UInventoryManager::SetItemTable(UDataTable* Table)
{
	mItemTable = Table;

	// ������ ���̺��� ����ִ� ��� �� �̸�
	FString str;
	TArray<FItemDataInfo*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataInfo>(str, AllItemInfo);

	// ������ ���̺��� ����ִ� ��� �� �̸�
	TArray<FName> AllRowName;
	AllRowName = mItemTable->GetRowNames();

	// ���̺� ������ �޾ƿͼ� �ݺ����� ����.
	// ItemID�� Ű������, �ش� �������� RowName�� �����ͷ� �����Ѵ�.
	int Count = AllItemInfo.Num();

	for (int i = 0; i < Count; ++i)
	{
		mRowNameFromID.Add(AllItemInfo[i]->ID, AllRowName[i]);
	}
}

void UInventoryManager::AddItem(FItemDataInfo* Item)
{
	//mInventoryItem.Add(Item->ID, Item);

	// ������ �ߺ� üũ
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(mCurWorld));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	TArray<UObject*> TileViewItems = MainHUD->GetInventoryWidget()->GetTileView()->GetListItems();

	int32 Count = TileViewItems.Num();
	int32 Index, ItemCount;
	bool bCheck = false;

	// Potion(�Ҹ�ǰ) ������ �ߺ�üũ O, ���� �������� �ߺ�üũ X
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

	if (bCheck)	// �ߺ��� �������� �ִ� ���
	{
		// TilewView �ߺ��� ������ ���� ���� (ItemCount)
		Cast<UItemDataBase>(MainHUD->GetInventoryWidget()->GetTileView()->
			GetItemAt(Index))->SetItemCount(ItemCount + 1);

		// UI�� ǥ�õ� ItemCount ���� ����
		UItemDataBase* Item = Cast<UItemDataBase>(
			MainHUD->GetInventoryWidget()->GetTileView()->GetItemAt(Index));

		UInventoryItemBase* ItemBase =
			Cast<UInventoryItemBase>(MainHUD->GetInventoryWidget()->
				GetTileView()->GetEntryWidgetFromItem(Item));

		ItemBase->SetItemCountText(ItemCount + 1);
	}

	else
	{
		// TileView�� ������ ���� �߰�
		UItemDataBase* NewData = NewObject<UItemDataBase>();

		NewData->SetItemInfo(Item);
		NewData->SetItemCount(1);
		MainHUD->GetInventoryWidget()->GetTileView()->AddItem(NewData);
	}
}