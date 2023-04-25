// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "Shinbi.generated.h"

UENUM(BlueprintType)
enum class EShinbiSkillType : uint8
{
	Cast_Wolf_One,
	Cast_Wolf_Circle,
	Cast_Wolf_Group,
	Dash
};

UCLASS()
class PF_API AShinbi : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AShinbi();

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

	virtual void SkillQ();	// Cast_Wolf_One
	virtual void SkillE();	// Cast_Wolf_Circle
	virtual void SkillR();	// Cast_Wolf_Group
	virtual void SkillF();	// Dash

private:
	void SkillQKey();
	void SkillEKey();
	void SkillRKey();
	void SkillRKeyUp();
	void SkillFKey();
	void SkillRTarget(float Scale);

	void CheckCameraZoom();

public:
	virtual void SavePlayer();

public:
	UFUNCTION()
	void WolfSkillEnd(class ASkillActor* SkillActor, const FHitResult& Hit);


private:
	EShinbiSkillType	mShinbiSkillType;
	EShinbiSkillType	mSkillKeyInput;

public:
	EShinbiSkillType GetShinbiSkillType() const
	{
		return mShinbiSkillType;
	}


};
