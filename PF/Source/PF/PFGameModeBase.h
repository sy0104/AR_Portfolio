// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Blueprint\UserWidget.h>
#include <Components/TileView.h>

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "PFGameModeBase.generated.h"


UCLASS()
class PF_API APFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APFGameModeBase();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime)	override;

private:
	void SaveGame();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAudioComponent* mAudio;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundWave*		mSoundWave;

private:
	TSubclassOf<UUserWidget>	mMainHUDClass;
	class UMainHUDBase*			mMainHUD;

	class UPFSaveGame*			mSaveGame;

public:
	UMainHUDBase* GetMainHUD() 
	{ 
		return mMainHUD; 
	}

	class UPFSaveGame* GetSaveGame()
	{
		return mSaveGame;
	}
};
