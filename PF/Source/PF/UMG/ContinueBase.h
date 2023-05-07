// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ContinueBase.generated.h"


UCLASS()
class PF_API UContinueBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UButton*	mContinueButton;
	UButton*	mMainMapButton;
	UButton*	mExitButton;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void OnMainMapButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
};
