// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"


UMonsterAnimInstance::UMonsterAnimInstance()
{
	mAnimType = EMonsterAnimType::Idle;
	mHitAdditive = 0.f;

	mAttack = false;
	mAttackEnable = true;
	mAttackIndex = 0;
	mAttackEnd = false;

	mAttackDelay = false;
	mAttackDelayTime = 0.f;
	mCurDelayTime = 0.f;

	mUseSkill = false;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UMonsterAnimInstance::Attack()
{
	//if (!Montage_IsPlaying(mAttackMontageArray[mAttackIndex]))
	//{
	//	Montage_SetPosition(mAttackMontageArray[mAttackIndex], 0.f);
	//	Montage_Play(mAttackMontageArray[mAttackIndex]);

	//	mAttackIndex = (mAttackIndex + 1) % mAttackMontageArray.Num();
	//}

	//if (!IsValid(mAttackMontage))
	//	return;

	//if (!Montage_IsPlaying(mAttackMontage))
	//{
	//	Montage_SetPosition(mAttackMontage, 0.f);
	//	Montage_Play(mAttackMontage);
	//}

}

void UMonsterAnimInstance::Hit()
{
	if (!IsValid(mHitMontage))
		return;

	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontage))
	{
		Montage_SetPosition(mHitMontage, 0.f);
		Montage_Play(mHitMontage);
	}
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Attack();
	}

	mAttackEnd = false;
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->SetAttackEnd(true);
		Monster->SetAttackDelay(true);

		//Monster->SetAttackDelay(true);
	}

	mAttackIndex = 0;
	mAttackEnd = true;
}

void UMonsterAnimInstance::AnimNotify_Skill1()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Skill1();
		Monster->SetSkillEnd(false);
	}
}

void UMonsterAnimInstance::AnimNotify_Skill2()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Skill2();
		Monster->SetSkillEnd(false);
	}
}

void UMonsterAnimInstance::AnimNotify_Skill3()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->Skill3();
		Monster->SetSkillEnd(false);
	}
}

void UMonsterAnimInstance::AnimNotify_CastSkill1()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->CastSkill1();
	}
}

void UMonsterAnimInstance::AnimNotify_CastSkill2()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->CastSkill2();
	}
}

void UMonsterAnimInstance::AnimNotify_SkillEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->SetAttackDelay(true);
		Monster->SetSkillEnd(true);
	}
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->DestroyWidgetComponent();
		Monster->OnDissolve();
	}

	//TryGetPawnOwner()->Destroy();
}
