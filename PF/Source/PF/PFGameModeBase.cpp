// Copyright Epic Games, Inc. All Rights Reserved.


#include "PFGameModeBase.h"
#include "Player/Shinbi.h"
#include "Player/Countess.h"
#include "Player/PFPlayerController.h"
#include "UMG/MainHUDBase.h"
#include "UMG/InventoryBase.h"
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
		MainMusicAsset(TEXT("SoundWave'/Game/FantasyOrchestral/audio/Enchanted_Forest.Enchanted_Forest'"));
	if (MainMusicAsset.Succeeded())
		mMainMapSoundWave = MainMusicAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave>
		FireMusicAsset(TEXT("SoundWave'/Game/FantasyOrchestral/audio/Harp_of_Chronos.Harp_of_Chronos'"));
	if (FireMusicAsset.Succeeded())
		mFireMapSoundWave = FireMusicAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave>
		IceMusicAsset(TEXT("SoundWave'/Game/FantasyOrchestral/audio/Fantasy_Adventure.Fantasy_Adventure'"));
	if (IceMusicAsset.Succeeded())
		mIceMapSoundWave = IceMusicAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave>
		BossMusicAsset(TEXT("SoundWave'/Game/FantasyOrchestral/audio/Fantasy_Adventure.Fantasy_Adventure'"));
	if (BossMusicAsset.Succeeded())
		mBossMapSoundWave = BossMusicAsset.Object;

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
		}
	}

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (LevelName == TEXT("IceMap"))
		mAudio->SetSound(mIceMapSoundWave);
	else if (LevelName == TEXT("FireMap"))
		mAudio->SetSound(mFireMapSoundWave);
	else if (LevelName == TEXT("BossMap"))
		mAudio->SetSound(mBossMapSoundWave);
	else
		mAudio->SetSound(mMainMapSoundWave);

	// 배경음 재생
	mAudio->Play();
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

		*Writer << mSaveGame->mHPRatio;
		*Writer << mSaveGame->mMPRatio;
		*Writer << mSaveGame->mExpRatio;

		*Writer << mSaveGame->mHPPotionCount;
		*Writer << mSaveGame->mMPPotionCount;
		*Writer << mSaveGame->mAttackPotionCount;
		*Writer << mSaveGame->mArmorPotionCount;

		*Writer << mSaveGame->mEquipedWeaponIndex;
		*Writer << mSaveGame->mEquipedArmorIndex;
		*Writer << mSaveGame->mEquipedAccesaryIndex;

		Writer->Close();

		delete Writer;
	}

	APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();

	UTileView* TileView = MainHUD->GetInventoryWidget()->GetTileView();
	TArray<FSaveItemInfo> SaveItemArray;

	if (IsValid(TileView))
	{
		int32 Count = TileView->GetNumItems();

		for (int32 i = 0; i < Count; ++i)
		{
			FSaveItemInfo SaveItem;

			UItemDataBase* IndexItem = Cast<UItemDataBase>(TileView->GetItemAt(i));
			FItemDataInfo* ItemInfo = IndexItem->GetItemInfo();

			SaveItem.ID = ItemInfo->ID;
			SaveItem.ItemCount = ItemInfo->ItemCount;

			SaveItemArray.Add(SaveItem);
		}

		UPFGameInstance* GameInst = GetWorld()->GetGameInstance<UPFGameInstance>();
		GameInst->SetSaveInven(SaveItemArray);
	}
}
