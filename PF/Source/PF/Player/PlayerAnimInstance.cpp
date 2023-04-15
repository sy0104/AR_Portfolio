// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "Shinbi.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mAnimType = EPlayerAnimType::Ground;

	mMoveDir = 0.f;
	mGround = true;

	mAttackIndex = 0;
	mAttack = false;
	mAttackEnable = true;
	
	mFallRecoveryAdditive = 0.f;
	mHitAdditive = 0.f;

	mUseSkillNumber = -1;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();

		mSpeedRatio = Movement->Velocity.Size() / Movement->MaxWalkSpeed;

		// �� ���� �ִ��� �Ǵ�
		mGround = Movement->IsMovingOnGround();
		
		if (!mGround && mAnimType != EPlayerAnimType::Jump)
		{
			//PrintViewport(1.f, FColor::Red, TEXT("Fall"));
			//mAnimType = EPlayerAnimType::Fall;
		}
	}
}

void UPlayerAnimInstance::Attack()
{
	if (!mAttackEnable)
		return;

	mAttackEnable = false;

	if (!Montage_IsPlaying(mAttackMontageArray[mAttackIndex]))
	{
		Montage_SetPosition(mAttackMontageArray[mAttackIndex], 0.f);
		Montage_Play(mAttackMontageArray[mAttackIndex]);

		mAttackIndex = (mAttackIndex + 1) % mAttackMontageArray.Num();
		mAttack = true;
	}

	else
		mAttackEnable = true;
}

void UPlayerAnimInstance::Jump()
{
	mAnimType = EPlayerAnimType::Jump;
}

void UPlayerAnimInstance::UseSkill(int32 SkillNumber)
{
	if (!Montage_IsPlaying(mSkillMontageArray[SkillNumber].Montage))
	{
		mUseSkillNumber = SkillNumber;

		Montage_SetPosition(mSkillMontageArray[SkillNumber].Montage, 0.f);
		Montage_Play(mSkillMontageArray[SkillNumber].Montage);

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
		PlayerCharacter->SetUseSkill(true);
	}
}

void UPlayerAnimInstance::Hit()
{
	if (!IsValid(mHitMontage))
		return;

	if (mAttack)
		return;

	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontage))
	{
		Montage_SetPosition(mHitMontage, 0.f);
		Montage_Play(mHitMontage);
	}
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->NormalAttackCheck();
}

void UPlayerAnimInstance::AnimNotify_AttackEnable()
{
	mAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	mAttackIndex = 0;
	mAttackEnable = true;
	mAttack = false;
}

void UPlayerAnimInstance::AnimNotify_Cast()
{
	//APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	//if (IsValid(PlayerCharacter))
	//	PlayerCharacter->SkillQ();
}

void UPlayerAnimInstance::AnimNotify_UseSkill()
{
	if (mUseSkillNumber == -1)
		return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->UseSkill(mUseSkillNumber);
}

void UPlayerAnimInstance::AnimNotify_SkillEnd()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillEnd"));

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	PlayerCharacter->SetUseSkill(false);
}

void UPlayerAnimInstance::AnimNotify_JumpEnd()
{
	mAnimType = EPlayerAnimType::Fall;
}

void UPlayerAnimInstance::AnimNotify_FallEnd()
{
	mAnimType = EPlayerAnimType::Ground;

	if (IsValid(mFallRecoveryMontage))
	{
		mFallRecoveryAdditive = 1.f;

		Montage_SetPosition(mFallRecoveryMontage, 0.f);
		Montage_Play(mFallRecoveryMontage);
	}
}

void UPlayerAnimInstance::AnimNotify_FallRecoveryEnd()
{
	mFallRecoveryAdditive = 0.f;
}

void UPlayerAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;

	mAttackEnable = true;
	mAttack = false;
}

void UPlayerAnimInstance::AnimNotify_DeathEnd()
{
	// ������
}

// Shinbi
void UPlayerAnimInstance::AnimNotify_DashStart()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	PlayerCharacter->SetDash(true);

	PrintViewport(1.f, FColor::Red, TEXT("DashStart"));
}

void UPlayerAnimInstance::AnimNotify_DashEnd()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	PlayerCharacter->SetDash(false);

	PrintViewport(1.f, FColor::Red, TEXT("DashEnd"));

}