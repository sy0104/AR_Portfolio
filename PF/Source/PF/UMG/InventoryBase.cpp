// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBase.h"
#include "ItemDataBase.h"
#include "InventoryItemBase.h"
#include "../Player/PlayerCharacter.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"

void UInventoryBase::NativeConstruct()
{
	Super::NativeConstruct();

	mTileView = Cast<UTileView>(GetWidgetFromName(FName(TEXT("ItemTileView"))));

	mTileView->OnItemClicked().AddUObject(this, &UInventoryBase::ClickItem);
	mTileView->OnItemDoubleClicked().AddUObject(this, &UInventoryBase::UnEquipItem);

	mDescBackImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("DescBack"))));
	mDescFrameImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("DescBackFrame"))));
	mItemNameText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ItemNameText"))));
	mItemDescText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ItemDescText"))));
}

void UInventoryBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryBase::ClickItem(UObject* Item)
{
	PrintViewport(1.f, FColor::Red, TEXT("ClickItem"));

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

	if (!IsValid(TileView))
		return;

	UInventoryItemBase* NewItem = Cast<UInventoryItemBase>(MainHUD->GetInventoryWidget()->GetTileView()->GetEntryWidgetFromItem(Item));
	FItemDataInfo* ItemInfo = Cast<UItemDataBase>(Item)->GetItemInfo();

	switch (ItemInfo->ItemType)
	{
	case EItemType::Potion:
		PlayerCharacter->UsePotion(ItemInfo);
		break;

	case EItemType::Equip_Weapon:
		if (PlayerCharacter->IsEquipedWeapon())
		{
			UInventoryItemBase* EquipedWeapon = 
				Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(PlayerCharacter->GetEquipedWeapon()));

			if (IsValid(EquipedWeapon))
			{
				EquipedWeapon->SetUnEquip();	// (인벤토리 UI) 착용상태 해제
				PlayerCharacter->UnEquipItem(PlayerCharacter->GetEquipedWeapon());
			}
		}

		if (IsValid(NewItem))
			NewItem->SetEquip();

		PlayerCharacter->EquipItem(Item);
		PlayerCharacter->SetEquipedWeaponIndex(TileView->GetIndexForItem(Item));
		break;

	case EItemType::Equip_Armor:
		if (PlayerCharacter->IsEquipedArmor())
		{
			UInventoryItemBase* EquipedArmor =
				Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(PlayerCharacter->GetEquipedArmor()));

			EquipedArmor->SetUnEquip();
			PlayerCharacter->UnEquipItem(PlayerCharacter->GetEquipedArmor());
		}

		if (IsValid(NewItem))
			NewItem->SetEquip();

		PlayerCharacter->EquipItem(Item);
		PlayerCharacter->SetEquipedArmorIndex(TileView->GetIndexForItem(Item));
		break;

	case EItemType::Equip_Accesary:
		if (PlayerCharacter->IsEquipedAccesary())
		{
			UInventoryItemBase* EquipedAccesary =
				Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(PlayerCharacter->GetEquipedAccesary()));

			EquipedAccesary->SetUnEquip();
			PlayerCharacter->UnEquipItem(PlayerCharacter->GetEquipedAccesary());
		}

		if (IsValid(NewItem))
			NewItem->SetEquip();

		PlayerCharacter->EquipItem(Item);
		PlayerCharacter->SetEquipedArmorIndex(TileView->GetIndexForItem(Item));
		break;
	}

	// UI
	if (ItemInfo->ItemType == EItemType::Potion)
	{
		if (ItemInfo->ItemCount == 1)
		{
			ItemInfo->ItemCount = 0;
			mTileView->RemoveItem(Item);

			switch (ItemInfo->ID)
			{
			case EItemID::PT_HP_Potion:
				MainHUD->SetHPPotionCount(0);
				break;
			case EItemID::PT_MP_Potion:
				MainHUD->SetMPPotionCount(0);
				break;
			case EItemID::PT_Attack_Potion:
				MainHUD->SetAttackPotionCount(0);
				break;
			case EItemID::PT_Armor_Potion:
				MainHUD->SetArmorPotionCount(0);
				break;
			}
		}

		else
		{
			ItemInfo->ItemCount--;
			NewItem->SetItemCountText(ItemInfo->ItemCount);

			switch (ItemInfo->ID)
			{
			case EItemID::PT_HP_Potion:
				MainHUD->SetHPPotionCount(ItemInfo->ItemCount);
				break;
			case EItemID::PT_MP_Potion:
				MainHUD->SetMPPotionCount(ItemInfo->ItemCount);
				break;
			case EItemID::PT_Attack_Potion:
				MainHUD->SetAttackPotionCount(ItemInfo->ItemCount);
				break;
			case EItemID::PT_Armor_Potion:
				MainHUD->SetArmorPotionCount(ItemInfo->ItemCount);
				break;
			}
		}
	}
}

void UInventoryBase::UnEquipItem(UObject* Item)
{
	PrintViewport(1.f, FColor::Red, TEXT("DoubleClick"));

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

	if (IsValid(TileView))
	{
		UInventoryItemBase* ItemBase =
			Cast<UInventoryItemBase>(MainHUD->GetInventoryWidget()->GetTileView()->GetEntryWidgetFromItem(Item));

		ItemBase->SetUnEquip();

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (IsValid(PlayerCharacter))
			PlayerCharacter->UnEquipItem(Item);
	}
}

void UInventoryBase::ShowItemDesc(UObject* Item, bool IsHovered, FVector2D Mouse)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	// Visible
	if (IsHovered)
	{
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("X : %f"), Mouse.X));
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Y : %f"), Mouse.Y));

		//mDescBackImage->SetVisibility(ESlateVisibility::Visible);
		//mDescFrameImage->SetVisibility(ESlateVisibility::Visible);
		//mItemNameText->SetVisibility(ESlateVisibility::Visible);
		//mItemDescText->SetVisibility(ESlateVisibility::Visible);

		//FItemDataInfo* ItemInfo = Cast<UItemDataBase>(Item)->GetItemInfo();

		//mItemNameText->SetText(FText::FromString(ItemInfo->ItemName));
		//mItemDescText->SetText(FText::FromString(ItemInfo->Description));

		//mDescBackImage->SetRenderTranslation(FVector2D(Mouse.X, Mouse.Y));
		//mDescFrameImage->SetRenderTranslation(FVector2D(Mouse.X, Mouse.Y));
		//mItemNameText->SetRenderTranslation(FVector2D(Mouse.X, Mouse.Y));
		//mItemDescText->SetRenderTranslation(FVector2D(Mouse.X, Mouse.Y));
	}

	// Hidden
	else
	{
		int Index = mTileView->GetIndexForItem(Item);

		UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

		if (IsValid(TileView))
		{
			UItemDataBase* IndexItem = Cast<UItemDataBase>(TileView->GetItemAt(Index));
			UInventoryItemBase* ItemBase =
				Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(IndexItem));
		}

		//ItemBase->SetItemDescOff(Item);
		//mDescBackImage->SetRenderTranslation
	}
}

void UInventoryBase::UpdatePotionCount(EItemID ID)
{
	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();

	if (IsValid(TileView))
	{
		TArray<UObject*> TileViewItems = TileView->GetListItems();

		int32 Count = TileViewItems.Num();
		int32 ItemCount;

		for (int32 i = 0; i < Count; ++i)
		{
			if (Cast<UItemDataBase>(TileViewItems[i])->GetItemInfo()->ID == ID)
			{
				ItemCount = Cast<UItemDataBase>(TileViewItems[i])->GetItemInfo()->ItemCount;

				if (ItemCount <= 0)
				{
					mTileView->RemoveItem(TileViewItems[i]);
					break;
				}

				else
				{
					UInventoryItemBase* ItemBase = Cast<UInventoryItemBase>(TileView->GetEntryWidgetFromItem(TileViewItems[i]));

					ItemBase->SetItemCountText(ItemCount);
					break;
				}
			}
		}
	}
}

void UInventoryBase::SetItemDescOn(UObject* Item)
{

}

void UInventoryBase::SetItemDescOff(UObject* Item)
{
}
