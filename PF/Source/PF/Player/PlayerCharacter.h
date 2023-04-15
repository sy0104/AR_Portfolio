// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


struct Buff
{
	bool		bBuffOn;
	float		CurBuffTime;
	float		BuffTime;
	float		BuffValue;
	float		OriginStat;
};

struct EquipedItem
{
	UObject*		Weapon;
	UObject*		Armor;
	UObject*		Accesary;
};

UCLASS()
class PF_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	// 축매핑 바인드 함수
	void MoveFront(float Scale);
	void MoveSide(float Scale);
	void RotationCameraZ(float Scale);
	void RotationCameraY(float Scale);
	void CameraZoom(float Scale);

	// 액션매핑 바인드 함수
	void NormalAttack();
	void SkillQKey();
	void SkillEKey();
	void SkillRKey();
	void SkillFKey();
	void JumpKey();
	void InventoryOn();
	void PlayerStatOnOff();
	void CheatKey();
	void ItemKey();
	void HPPotionkey();
	void MPPotionKey();
	void AttackPotionKey();
	void ArmorPotionKey();

public:
	virtual void NormalAttackCheck();
	virtual void UseSkill(int32 SkillNumber);
	virtual void SkillQ();
	virtual void SkillE();
	virtual void SkillR();
	virtual void SkillF();

public:
	void LevelUp();

	void UsePotion(FItemDataInfo* ItemInfo);

	virtual void AttackBuffOn(float DeltaTime);
	virtual void ArmorBuffOn(float DeltaTime);

	virtual void EquipItem(UObject* Item);
	virtual void UnEquipItem(UObject* Item);

protected:
	virtual void InitUI();
	virtual void SetPotionInfo(FItemDataInfo* ItemInfo);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	FPlayerInfo				mPlayerInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*	mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*		mCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TArray<FSkillInfo>		mSkillInfoArray;

protected:
	class UPlayerAnimInstance*	mAnimInst;

	float		mMoveDir;
	bool		mDeath;
	bool		mUseSkill;
	bool		mTargeting;
	bool		mDash;
	float		mCameraZoomMin;
	float		mCameraZoomMax;

	float		mHPRatio;
	float		mMPRatio;
	float		mExpRatio;

	Buff		mAttackBuff;
	Buff		mArmorBuff;

	EquipedItem		mEquipedItem;

public:
	const FPlayerInfo& GetPlayerInfo() const
	{
		return mPlayerInfo;
	}

	float GetCameraZoomMin() const
	{
		return mCameraZoomMin;
	}

	float GetCameraZoomMax() const
	{
		return mCameraZoomMax;
	}

	float GetExpRatio() const
	{
		return mExpRatio;
	}

	float GetExpMax() const
	{
		return mPlayerInfo.ExpMax;
	}

	bool IsEquipedWeapon()
	{
		if (mEquipedItem.Weapon)
			return true;
		
		return false;
	}

	bool IsEquipedArmor()
	{
		if (mEquipedItem.Armor)
			return true;

		return false;
	}

	bool IsEquipedAccesary()
	{
		if (mEquipedItem.Accesary)
			return true;

		return false;
	}

	UObject* GetEquipedWeapon()
	{
		return mEquipedItem.Weapon;
	}

	UObject* GetEquipedArmor()
	{
		return mEquipedItem.Armor;
	}

	UObject* GetEquipedAccesary()
	{
		return mEquipedItem.Accesary;
	}

public:
	void SetUseSkill(bool UseSkill)
	{
		mUseSkill = UseSkill;
	}

	void SetTargeting(bool Targeting)
	{
		mTargeting = Targeting;
	}

	void SetDash(bool Dash)
	{
		mDash = Dash;
	}

	void SetExp(float Ratio);
	void SetGold(int Gold);
};
