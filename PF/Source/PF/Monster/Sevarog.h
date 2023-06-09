// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Sevarog.generated.h"

enum class ESevarogSkill : uint8
{
	Skill1,
	Skill2,
	Skill3
};

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
	virtual void Skill3();
	virtual void CastSkill1();
	virtual void CastSkill2();

	virtual void SkillStart();

private:
	void Skill2Start();

private:
	ESevarogSkill	mSkillType;

	FVector		CastLoc;
	FVector		CastForward;
};
