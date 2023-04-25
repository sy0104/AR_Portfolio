// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"




UCLASS()
class PF_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class APFSaveGame;

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void UnPossessed();

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
	void SkillRMKey();

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
	virtual void SkillRM();
	virtual void SkillF();

public:
	virtual void SkillRCast();
	virtual void SkillRMCast();

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

public:
	virtual void SavePlayer();
	virtual void StartSkill();

	void PlaySkillCameraShake();
	void PlayUltimateCameraShake();

protected:
	void CameraZoomInOut();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	FPlayerInfo				mPlayerInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*	mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*		mCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TArray<FSkillInfo>		mSkillInfoArray;

	// Camera Shake
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase>	mDamageCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase>	mSkillCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase>	mUltimateCameraShake;

protected:
	class UPlayerAnimInstance*	mAnimInst;

	float		mMoveDir;
	bool		mDeath;
	bool		mUseSkill;
	bool		mTargeting;
	bool		mDash;
	float		mCameraZoomMin;
	float		mCameraZoomMax;

	bool		mCameraZoom;
	bool		mCameraZoomIn;
	float		mCameraZoomSpeed;
	float		mSkillCameraZoomMin;
	float		mSkillCameraZoomMax;

	float		mHPRatio;
	float		mMPRatio;
	float		mExpRatio;

	FPotionBuff			mAttackBuff;
	FPotionBuff			mArmorBuff;

	FEquipedItem		mEquipedItem;

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

	bool IsEquipedWeapon() const
	{
		if (mEquipedItem.Weapon)
			return true;
		
		return false;
	}

	bool IsEquipedArmor() const
	{
		if (mEquipedItem.Armor)
			return true;

		return false;
	}

	bool IsEquipedAccesary() const
	{
		if (mEquipedItem.Accesary)
			return true;

		return false;
	}

	UObject* GetEquipedWeapon() const
	{
		return mEquipedItem.Weapon;
	}

	UObject* GetEquipedArmor() const
	{
		return mEquipedItem.Armor;
	}

	UObject* GetEquipedAccesary() const
	{
		return mEquipedItem.Accesary;
	}

	bool GetUseSkill() const
	{
		return mUseSkill;
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

	void SetTimeDilation(float TimeDilation)
	{
		CustomTimeDilation = TimeDilation;
	}

	void SetCameraZoom(bool CameraZoom, float ZoomSpeed, bool CameraZoomIn, float Min, float Max)
	{
		mCameraZoom = CameraZoom;
		mCameraZoomSpeed = ZoomSpeed;
		mCameraZoomIn = CameraZoomIn;
		mSkillCameraZoomMin = Min;
		mSkillCameraZoomMax = Max;
	}

	void SetExp(float Ratio);
	void SetGold(int Gold);
};
