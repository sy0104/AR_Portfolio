// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "GameEndBase.generated.h"


UCLASS()
class PF_API UGameEndBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UButton*	mRestartButton;
	UButton*	mMainMapButton;
	UButton*	mExitButton;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnMainMapButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
};
