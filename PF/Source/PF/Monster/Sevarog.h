// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Sevarog.generated.h"


UCLASS()
class PF_API ASevarog : public AMonster
{
	GENERATED_BODY()
	
public:
	ASevarog();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void Attack();
	virtual void Skill1();
	virtual void Skill2();
};
