// Copyright Epic Games, Inc. All Rights Reserved.


#include "PFGameModeBase.h"
#include "Player/Shinbi.h"
#include "Player/Countess.h"
#include "Player/PFPlayerController.h"
#include "UMG/MainHUDBase.h"
#include "PFGameInstance.h"
#include "PFSaveGame.h"
#include "Manager/InventoryManager.h"

APFGameModeBase::APFGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//// Shinbi
	//ConstructorHelpers::FClassFinder<AShinbi> ShinbiFinder(
	//	TEXT("Blueprint'/Game/Blueprints/BPC_Shinbi.BPC_Shinbi_C'"));

	//if (ShinbiFinder.Succeeded())
	//	DefaultPawnClass = ShinbiFinder.Class;
	//else
	//	DefaultPawnClass = AShinbi::StaticClass();

	// Countess
	ConstructorHelpers::FClassFinder<ACountess> CountessFinder(
		TEXT("Blueprint'/Game/Blueprints/BPC_Countess.BPC_Countess_C'"));

	if (CountessFinder.Succeeded())
		DefaultPawnClass = CountessFinder.Class;
	else
		DefaultPawnClass = ACountess::StaticClass();

	PlayerControllerClass = APFPlayerController::StaticClass();

	ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_MainHUD.UI_MainHUD_C'"));

	if (Finder.Succeeded())
		mMainHUDClass = Finder.Class;

	// 배경음
	static ConstructorHelpers::FObjectFinder<USoundWave> 
		MainMusicAsset(TEXT("SoundWave'/Game/FantasyOrchestral/audio/Magical_Fantasy.Magical_Fantasy'"));

	if (MainMusicAsset.Succeeded())
		mSoundWave = MainMusicAsset.Object;
	
	mAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	mAudio->bAutoActivate = false;
}

void APFGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	mSaveGame = NewObject<UPFSaveGame>();

	if (IsValid(mMainHUDClass))
	{
		mMainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), mMainHUDClass));

		if (IsValid(mMainHUD))
		{
			mMainHUD->AddToViewport();

			//mMainHUD->SetHP(0.33f);
		}
	}

	// 배경음 재생
	mAudio->SetSound(mSoundWave);
	//mAudio->Play();
}

void APFGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::LevelTransition)
	{
		SaveGame();
	}
}


void APFGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UPFGameInstance* GameInst = GetWorld()->GetGameInstance<UPFGameInstance>();

	if (IsValid(GameInst))
	{
		//// 나중에 직업 추가하면 수정
		//ConstructorHelpers::FClassFinder<AShinbi> ShinbiFinder(
		//	TEXT("Blueprint'/Game/Blueprints/BPC_Shinbi.BPC_Shinbi_C'"));

		//if (ShinbiFinder.Succeeded())
		//	DefaultPawnClass = ShinbiFinder.Class;
		//else
		//	DefaultPawnClass = AShinbi::StaticClass();
	}
}

void APFGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void APFGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APFGameModeBase::SaveGame()
{
	FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");
	FArchive* Writer = IFileManager::Get().CreateFileWriter(*FullPath);

	if (Writer)
	{
		*Writer << mSaveGame->mPlayerInfo.Name;
		*Writer << mSaveGame->mPlayerInfo.AttackPoint;
		*Writer << mSaveGame->mPlayerInfo.ArmorPoint;
		*Writer << mSaveGame->mPlayerInfo.HP;
		*Writer << mSaveGame->mPlayerInfo.HPMax;
		*Writer << mSaveGame->mPlayerInfo.MP;
		*Writer << mSaveGame->mPlayerInfo.MPMax;
		*Writer << mSaveGame->mPlayerInfo.Level;
		*Writer << mSaveGame->mPlayerInfo.Exp;
		*Writer << mSaveGame->mPlayerInfo.Gold;
		*Writer << mSaveGame->mPlayerInfo.MoveSpeed;
		*Writer << mSaveGame->mPlayerInfo.AttackDistance;

		*Writer << mSaveGame->mCameraZoomMin;
		*Writer << mSaveGame->mCameraZoomMax;

		*Writer << mSaveGame->mEquipedItem.Weapon;
		*Writer << mSaveGame->mEquipedItem.Armor;
		*Writer << mSaveGame->mEquipedItem.Accesary;

		*Writer << mSaveGame->mHPRatio;
		*Writer << mSaveGame->mMPRatio;
		*Writer << mSaveGame->mExpRatio;

		// Inven
		

		Writer->Close();

		delete Writer;
	}
}
