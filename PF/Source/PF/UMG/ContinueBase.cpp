// Fill out your copyright notice in the Description page of Project Settings.


#include "ContinueBase.h"
#include "../Player/PlayerCharacter.h"

void UContinueBase::NativeConstruct()
{
	Super::NativeConstruct();

	mContinueButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("ContinueButton"))));
	mMainMapButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("MainMapButton"))));
	mExitButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("ExitButton"))));


	mContinueButton->OnClicked.AddDynamic(this, &UContinueBase::OnContinueButtonClicked);
	mMainMapButton->OnClicked.AddDynamic(this, &UContinueBase::OnMainMapButtonClicked);
	mExitButton->OnClicked.AddDynamic(this, &UContinueBase::OnExitButtonClicked);

}

void UContinueBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UContinueBase::OnContinueButtonClicked()
{
	PrintViewport(1.f, FColor::Red, TEXT("Continue Button"));

	SetVisibility(ESlateVisibility::Hidden);

	// 플레이어 리스폰
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerCharacter->Respawn();
}

void UContinueBase::OnMainMapButtonClicked()
{
	PrintViewport(1.f, FColor::Red, TEXT("MainMap Button"));

	SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void UContinueBase::OnExitButtonClicked()
{
	PrintViewport(1.f, FColor::Red, TEXT("Exit Button"));
}
