// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerShape.h"
#include "LevelTrigger.generated.h"


UCLASS()
class PF_API ALevelTrigger : public ATriggerShape
{
	GENERATED_BODY()
	
public:
	ALevelTrigger();

public:
	virtual void TriggerBegin(const FHitResult& SweepResult);
	virtual void TriggerEnd();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName		mLevelName;
};
