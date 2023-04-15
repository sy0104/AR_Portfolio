// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "WolfSkill.generated.h"

UCLASS()
class PF_API AWolfSkill : public AActor
{
	GENERATED_BODY()

	friend class AShinbi;

public:	
	AWolfSkill();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:
	FShinbiSkillInfo		mSkillInfo;
	FVector					mSpawnLocation;
	FRotator				mSpawnRotator;

public:
	FShinbiSkillInfo GetSkillInfo() const
	{
		return mSkillInfo;
	}

public:
	void SetSpawnLocation(FVector SpawnLocation)
	{
		mSpawnLocation = SpawnLocation;
	}

	void SetSpawnRotator(FRotator SpawnRotator)
	{
		mSpawnRotator = SpawnRotator;
	}


};
