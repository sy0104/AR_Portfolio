// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class PF_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform);

public:
	UFUNCTION()
	void ComputeSpline();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent*		mRoot;

	// TSubclassOf: 클래스 정보를 저장해두었다가 필요 시 해당 클래스 정보를 이용하여 객체를 생성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMonster>		mSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		mSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		mSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonster*>		mMonsterArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPatrolType					mPatrolType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USplineComponent*			mPatrolSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32						mDivideCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool						mDivideMeshVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh*				mDivideMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>				mSplinePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRotator>			mSplineRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*>	mMeshArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AMonsterPatrolPoint*>	mPatrolPointArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EPatrolEndDir					mPatrolDir;

protected:
	TArray<FVector>		mPatrolPointLocationArray;

	float		mSplineLength;
	float		mCellDistance;

	int32		mPrevDivideCount;
	float		mPrevLength;

	float		mTime;

public:
	const TArray<FVector>& GetSplinePointArray() const
	{
		return mSplinePoint;
	}

	FVector GetSplinePoint(int32 Index) const
	{
		return mSplinePoint[Index];
	}

	FVector GetSplinePoint(float Distance) const
	{
		return mPatrolSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	}

	float GetSplineLength() const
	{
		return mSplineLength;
	}

public:
	void RemoveMonster(class AMonster* Monster)
	{
		mMonsterArray.Remove(Monster);
	}
};
