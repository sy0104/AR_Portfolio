// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Blueprint\UserWidget.h>

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "PFGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PF_API APFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APFGameModeBase();

public:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime) override;

private:
	TSubclassOf<UUserWidget>	mMainHUDClass;
	class UMainHUDBase*			mMainHUD;

public:
	UMainHUDBase* GetMainHUD() 
	{ 
		return mMainHUD; 
	}
};
