// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "Wolf.generated.h"

UCLASS()
class PF_API AWolf : public ACharacter
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWolfDead, ASkillActor*, SkillActor, const FHitResult&, Hit);

public:
	AWolf();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Attack();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent*	mProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class AParticleCascade*			mParticle;

	int32				mAttackPoint;
	float				mAttackDistance;
	EWolfAnimType		mWolfAnimType;

	FVector				mSpawnLocation;
	FVector				mSpawnDir;
	FRotator			mSpawnRotation;

	class UWolfAnimInstance*	mAnimInst;

public:
	UPROPERTY(BlueprintAssignable)
	FOnWolfDead		mOnWolfDead;

public:
	void SetAttackPoint(int32 AttackPoint)
	{
		mAttackPoint = AttackPoint;
	}

	void SetAttackDistance(float AttackDistance)
	{
		mAttackDistance = AttackDistance;
	}

	void SetWolfAnimType(EWolfAnimType WolfAnimType)
	{
		mWolfAnimType = WolfAnimType;
	}

	void SetSpawnLocation(FVector Location)
	{
		mSpawnLocation = Location;
	}

	void SetSpawnDir(FVector Dir)
	{
		mSpawnDir = Dir;
	}

	void SetSpawnRotation(FRotator Rotation)
	{
		mSpawnRotation = Rotation;
	}
};
