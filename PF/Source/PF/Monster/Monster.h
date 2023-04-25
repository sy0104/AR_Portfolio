// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\WidgetComponent.h>

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

struct FConvertMaterial
{
	int32	Index;
	UMaterialInstanceDynamic* Mtrl;

	FConvertMaterial() :
		Index(-1),
		Mtrl(nullptr)
	{
	}
};

UCLASS()
class PF_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

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

public:
	void OnDissolve();
	void DestroyWidgetComponent();

protected:
	virtual void CheckAttackDelayTime(float DeltaTime);
	virtual void CheckUseSkill();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo		mMonsterInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName				mMonsterTableRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent*	mWidgetComponent;

protected:
	class UMonsterAnimInstance*		mAnimInst;
	class AMonsterSpawnPoint*		mSpawnPoint;

	bool				mAttackEnd;
	bool				mSkillEnd;
	float				mHPRatio;
	EMonsterPhase		mMonsterPhase;


	// Patrol
	EPatrolType			mPatrolType;

	TArray<FVector>		mPatrolPointLocationArray;
	EPatrolEndDir		mPatrolDir;
	int32				mPatrolIndex;
	int32				mPatrolIndexAdd;
	float				mPatrolWaitTime;
	bool				mPatrolEnable;

	int32				mPatrolSplineCount;
	float				mPatrolSplineLength;
	float				mPatrolCellDistance;
	float				mPatrolCurrentDistance;

	float				mPatrolTime;
	float				mPatrolTimeAcc;

	bool				mAttackDelay;
	float				mAttackDelayTime;
	float				mCurDelayTime;

	// Dissolve
	TArray<UMaterialInstanceDynamic*>	mDissolveMtrlArray;
	TArray<FConvertMaterial>			mDissolveMtrlIndexArray;	// dissolve Àû¿ëÇÒ ½½·Ô ÀÎµ¦½º

	bool				mDissolveEnable;
	float				mDissolve;
	float				mDissolveRange;
	float				mDissolveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mDissolveTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mDissolveMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mDissolveMax;

	// Skill
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool				mSkillEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FName>		mSkillNameArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FMonsterSkillInfo>	mSkillDataArray;

	int32	mUseSkillIndex;
	bool	mUseSkill;
	
public:
	const FMonsterInfo& GetMonsterInfo() const
	{
		return mMonsterInfo;
	}

	class UMonsterAnimInstance* GetMonsterAnimInst() const
	{
		return mAnimInst;
	}

	bool GetAttackEnd() const
	{
		return mAttackEnd;
	}

	bool GetSkillEnd() const
	{
		return mSkillEnd;
	}

	float GetPatrolWaitTime() const
	{
		return mPatrolWaitTime;
	}

	EPatrolType GetPatrolType() const
	{
		return mPatrolType;
	}

	float GetPatrolSplineLength() const
	{
		return mPatrolSplineLength;
	}

	int32 GetPatrolSplineCount() const
	{
		return mPatrolSplineCount;
	}

	bool GetAttackDelay() const
	{
		return mAttackDelay;
	}

	EMonsterPhase GetMonsterPhase() const
	{
		return mMonsterPhase;
	}

	bool GetArrive() const
	{
		float Dist = 10.f + GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (mPatrolIndexAdd == 1)
			return mPatrolIndex * mPatrolCellDistance - Dist <= mPatrolCurrentDistance;

		return mPatrolIndex * mPatrolCellDistance + Dist >= mPatrolCurrentDistance;
	}

	bool GetPatrolEnable() const
	{
		return mPatrolPointLocationArray.Num() >= 2;
	}

	FVector GetPatrolLocation() const;
	FVector GetPatrolPointLocation() const;

	int32 GetUseSkillIndex() const
	{
		return mUseSkillIndex;
	}

	const FMonsterSkillInfo* GetSkillData()
	{
		if (mUseSkillIndex == -1)
			return nullptr;

		return &mSkillDataArray[mUseSkillIndex];
	}

public:
	void SetAttackEnd(bool AttackEnd)
	{
		mAttackEnd = AttackEnd;
	}

	void SetSkillEnd(bool SkillEnd)
	{
		mSkillEnd = SkillEnd;
	}

	void AddPatrolWaitTime(float Time)
	{
		mPatrolWaitTime += Time;
	}

	void ClearPatrolWaitTime()
	{
		mPatrolWaitTime = 0.f;
	}

	void SetPatrolEnable(bool Enable)
	{
		mPatrolEnable = Enable;
	}

	void SetPatrolSplineCount(int32 Count)
	{
		mPatrolSplineCount = Count;
	}

	void SetPatrolCellDistance(float Length)
	{
		mPatrolCellDistance = Length;
	}

	void SetPatrolSplineLength(float Length)
	{
		mPatrolSplineLength = Length;
	}

	void SetPatrolType(EPatrolType Type)
	{
		mPatrolType = Type;
	}

	void SetPatrolDir(EPatrolEndDir Dir)
	{
		mPatrolDir = Dir;
	}

	void SetPatrolPointLocation(const TArray<FVector>& Array)
	{
		mPatrolPointLocationArray = Array;
	}

	void SetSpawnPoint(class AMonsterSpawnPoint* SpawnPoint)
	{
		mSpawnPoint = SpawnPoint;
	}

	void SetAttackDelay(bool AttackDelay)
	{
		mAttackDelay = AttackDelay;
	}

public:
	void NextPatrolPoint();
};
