// Copyright Epic Games, Inc. All Rights Reserved.


#include "PFGameModeBase.h"
#include "Player/Shinbi.h"
#include "Player/PFPlayerController.h"
#include "UMG/MainHUDBase.h"

APFGameModeBase::APFGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultPawnClass = AShinbi::StaticClass();
	PlayerControllerClass = APFPlayerController::StaticClass();

	ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_MainHUD.UI_MainHUD_C'"));

	if (Finder.Succeeded())
		mMainHUDClass = Finder.Class;
}

void APFGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(mMainHUDClass))
	{
		mMainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), mMainHUDClass));

		if (IsValid(mMainHUD))
		{
			mMainHUD->AddToViewport();

			//mMainHUD->SetHP(0.33f);
		}
	}
}

void APFGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APFGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void APFGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
