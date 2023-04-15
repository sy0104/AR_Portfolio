// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "IceGrux.generated.h"

/**
 * 
 */
UCLASS()
class PF_API AIceGrux : public AMonster
{
	GENERATED_BODY()
	
public:
	AIceGrux();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Attack();
	virtual void Skill1();
	virtual void SKill2();

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	UFUNCTION()
	void Skill1End(class ASkillActor* SkillActor, const FHitResult& Hit);

	UFUNCTION()
	void Skill2End(class ASkillActor* SkillActor, const FHitResult& Hit);

};
