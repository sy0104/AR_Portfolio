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
	virtual void OnPossess(APawn* aPawn) override;		// �÷��̾� ��Ʈ�ѷ��� Pawn�� ���ǵ� �� ȣ��ȴ�.
	virtual void OnUnPossess() override;				// �÷��̾� ��Ʈ�ѷ��� ���ǿ��� ������ �� ȣ��ȴ�.
};
