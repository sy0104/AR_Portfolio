// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemBase.h"
#include "ItemDataBase.h"

void UInventoryItemBase::NativeConstruct()
{
	Super::NativeConstruct();

	mIconImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("Icon"))));
	mItemCountText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ItemCount"))));
	mEquipCheckImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("EquipCheckImage"))));
}

void UInventoryItemBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

//void UInventoryItemBase::SetItemDescOn(UObject* Item)
//{
//	mDescBackImage->SetVisibility(ESlateVisibility::Visible);
//	mDescFrameImage->SetVisibility(ESlateVisibility::Visible);
//	mItemNameText->SetVisibility(ESlateVisibility::Visible);
//	mItemDescText->SetVisibility(ESlateVisibility::Visible);
//
//	//mItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemCount)));
//
//	FItemDataInfo* ItemInfo = Cast<UItemDataBase>(Item)->GetItemInfo();
//
//	mItemNameText->SetText(FText::FromString(ItemInfo->ItemName));
//	mItemDescText->SetText(FText::FromString(ItemInfo->Description));
//}
//
//void UInventoryItemBase::SetItemDescOff(UObject* Item)
//{
//	mDescBackImage->SetVisibility(ESlateVisibility::Hidden);
//	mDescFrameImage->SetVisibility(ESlateVisibility::Hidden);
//	mItemNameText->SetVisibility(ESlateVisibility::Hidden);
//	mItemDescText->SetVisibility(ESlateVisibility::Hidden);
//
//}

void UInventoryItemBase::InitFromData(UObject* Data)
{
	UItemDataBase* ItemData = Cast<UItemDataBase>(Data);

	if (IsValid(ItemData))
	{
		const FString& IconPath = ItemData->GetItemInfo()->IconPath;
		int ItemCount = ItemData->GetItemInfo()->ItemCount;

		UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *IconPath);

		if (IsValid(Tex2D))
		{
			mIconImage->SetBrushFromTexture(Tex2D);
		}

		mItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemCount)));
	}
}
