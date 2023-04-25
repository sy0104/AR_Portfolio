// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "Countess.generated.h"

enum class ECountessSkillType : uint8
{
	BlinkStrike,	// Q
	RollingDark,	// E
	Ultimate,		// R
	BladeSiphon		// 마우스 우클릭
};


UCLASS()
class PF_API ACountess : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	ACountess();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void InitUI() override;

public:
	virtual void NormalAttackCheck();
	virtual void UseSkill(int32 SkillNumber);
	virtual void SkillQ();
	virtual void SkillE();
	virtual void SkillR();
	virtual void SkillRM();

private:
	void SkillQKey();
	void SkillEKey();
	void SkillRKey();
	void SkillRMKey();

	void SkillETargeting(float Scale);
	void SkillEKeyUp();

	void CheckCameraZoom();
	void CheckReturn(float DeltaTime);

private:
	void SkillQStart();
	void SkillRStart();

public:
	virtual void SkillRCast();
	virtual void SkillRMCast();

	virtual void StartSkill();

public:
	virtual void SavePlayer();

private:
	ECountessSkillType		mCountessSkillType;

	FVector		mReturnLocation;
	FVector		mReturnRotation;

	bool		mReturnEnable;
	float		mReturnTime;
	float		mCurTime;

public:
	UFUNCTION()
	void SkillEEnd(class ASkillActor* SkillActor, const FHitResult& Hit);
};
