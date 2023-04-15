// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WolfSkill.h"
#include "CastWolfOne.generated.h"

/**
 * 
 */
UCLASS()
class PF_API ACastWolfOne : public AWolfSkill
{
	GENERATED_BODY()
	
public:
	ACastWolfOne();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
