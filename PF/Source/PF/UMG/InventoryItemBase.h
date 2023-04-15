// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Image.h>
#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemBase.generated.h"


UCLASS()
class PF_API UInventoryItemBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UImage*			mIconImage;
	UTextBlock*		mItemCountText;

	UImage*			mEquipCheckImage;

public:
	void SetItemCountText(int Count)
	{
		mItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}

	void SetEquip()
	{
		mEquipCheckImage->SetVisibility(ESlateVisibility::Visible);
	}

	void SetUnEquip()
	{
		mEquipCheckImage->SetVisibility(ESlateVisibility::Hidden);
	}

	//void SetItemDescOn(UObject* Item);
	//void SetItemDescOff(UObject* Item);

public:
	UFUNCTION(BlueprintCallable)
	void InitFromData(UObject* Data);
};
