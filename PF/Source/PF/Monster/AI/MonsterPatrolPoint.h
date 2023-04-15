// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "MonsterPatrolPoint.generated.h"

UCLASS()
class PF_API AMonsterPatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterPatrolPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent*		mRoot;
};
