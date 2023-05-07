// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/TileView.h>
#include <Components\Image.h>
#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBase.generated.h"

UCLASS()
class PF_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UFUNCTION()
	void ClickItem(UObject* Item);

	UFUNCTION()
	void UnEquipItem(UObject* Item);

	UFUNCTION(BlueprintCallable)
	void ShowItemDesc(UObject* Item, bool IsHovered, FVector2D Mouse);

public:
	void UpdatePotionCount(EItemID ID);

private:
	UTileView*		mTileView;

	UImage*			mDescBackImage;
	UImage*			mDescFrameImage;
	UTextBlock*		mItemNameText;
	UTextBlock*		mItemDescText;

public:
	void SetItemDescOn(UObject* Item);
	void SetItemDescOff(UObject* Item);

public:
	UTileView* GetTileView()
	{
		return mTileView;
	}
};
