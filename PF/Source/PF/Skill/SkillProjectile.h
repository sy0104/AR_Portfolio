// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "SkillProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PF_API ASkillProjectile : public ASkillActor
{
	GENERATED_BODY()
	
public:
	ASkillProjectile();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void CollisionProjectile(const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* mProjectile;

	float			mDistance;
	FVector			mPrevLocation;
	float			mDamage;

public:
	UProjectileMovementComponent* GetProjectile()	const
	{
		return mProjectile;
	}

	void SetDistance(float Distance)
	{
		mDistance = Distance;
	}

	void SetDamage(float Damage)
	{
		mDamage = Damage;
	}

	void SetInitialSpeed(float Speed)
	{
		mProjectile->InitialSpeed = Speed;
	}
};
