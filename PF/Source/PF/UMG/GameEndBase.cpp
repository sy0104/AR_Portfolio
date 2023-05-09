// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndBase.h"
#include "../Player/PlayerCharacter.h"

void UGameEndBase::NativeConstruct()
{
	Super::NativeConstruct();

	mRestartButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("RestartButton"))));
	mMainMapButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("MainMapButton"))));
	mExitButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("ExitButton"))));

	mRestartButton->OnClicked.AddDynamic(this, &UGameEndBase::OnRestartButtonClicked);
	mMainMapButton->OnClicked.AddDynamic(this, &UGameEndBase::OnMainMapButtonClicked);
	mExitButton->OnClicked.AddDynamic(this, &UGameEndBase::OnExitButtonClicked);
}

void UGameEndBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGameEndBase::OnRestartButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->RestartGame();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void UGameEndBase::OnMainMapButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void UGameEndBase::OnExitButtonClicked()
{

}
