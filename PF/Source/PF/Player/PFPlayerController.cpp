// Fill out your copyright notice in the Description page of Project Settings.


#include "PFPlayerController.h"

APFPlayerController::APFPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;


}

void APFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
	bShowMouseCursor = true;
}

void APFPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void APFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void APFPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
