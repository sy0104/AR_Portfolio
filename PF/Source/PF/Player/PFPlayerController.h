// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "PFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PF_API APFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APFPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();

protected:
	virtual void OnPossess(APawn* aPawn) override;		// 플레이어 컨트롤러가 Pawn에 빙의될 때 호출된다.
	virtual void OnUnPossess() override;				// 플레이어 컨트롤러가 빙의에서 해제될 때 호출된다.
};
