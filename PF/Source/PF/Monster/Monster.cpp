// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "../PFGameInstance.h"
#include "MonsterAnimInstance.h"
#include "MonsterAIController.h"
#include "MonsterSpawnPoint.h"
#include "../UMG/MonsterHPBase.h"
#include "../Player/PlayerCharacter.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(11);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();

	mAttackEnd = false;
	mSkillEnd = false;
	mUseSkill = false;

	mHPRatio = 1.f;

	mMonsterPhase = EMonsterPhase::NormalAttack;

	mPatrolIndex = 1;
	mPatrolIndexAdd = 1;
	mPatrolWaitTime = 0.f;

	mPatrolEnable = false;
	mPatrolCurrentDistance = 0.f;

	mAttackDelay = false;
	mAttackDelayTime = 3.f;
	mCurDelayTime = 0.f;

	mDissolveEnable = false;
	mDissolveMax = 1.5f;
	mDissolveMin = -1.f;
	mDissolveTimeMax = 3.f;
	mDissolveTime = 0.f;

	mDissolve = 0.f;
	mDissolveRange = mDissolveMax - mDissolveMin;

	// Monster HP Widget
	mWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCom"));
	mWidgetComponent->SetupAttachment(GetMesh());

	// Skill Camera Shake
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> BossSkillCameraShakeBPC(
		TEXT("Blueprint'/Game/Blueprints/Camera/BPC_BossSkillCameraShake.BPC_BossSkillCameraShake_C'"));

	if (BossSkillCameraShakeBPC.Succeeded())
		mBossSkillCameraShake = BossSkillCameraShakeBPC.Class;
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	// 데이터 테이블에서 몬스터 정보 가져와서 세팅
	UPFGameInstance* GameInst = GetWorld()->GetGameInstance<UPFGameInstance>();
	const FMonsterTableInfo* Info = GameInst->FindMonsterTable(mMonsterTableRowName);

	if (Info)
	{
		mMonsterInfo.Name = Info->Name;
		mMonsterInfo.AttackPoint = Info->AttackPoint;
		mMonsterInfo.ArmorPoint = Info->ArmorPoint;
		mMonsterInfo.HP = Info->HP;
		mMonsterInfo.HPMax = Info->HP;
		mMonsterInfo.MP = Info->MP;
		mMonsterInfo.MPMax = Info->MP;
		mMonsterInfo.Level = Info->Level;
		mMonsterInfo.Exp = Info->Exp;
		mMonsterInfo.Gold = Info->Gold;
		mMonsterInfo.MoveSpeed = Info->MoveSpeed;
		mMonsterInfo.AttackDistance = Info->AttackDistance;
		mMonsterInfo.TraceDistance = Info->TraceDistance;

		// 앉아서 이동 시 이동속도의 반으로 지정
		GetCharacterMovement()->MaxWalkSpeed = mMonsterInfo.MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = mMonsterInfo.MoveSpeed * 0.5f;

		GetMesh()->SetSkeletalMesh(Info->Mesh);
		GetMesh()->SetAnimInstanceClass(Info->MonsterAnimClass);

		// Dissolve
		int32 IndexCount = mDissolveMtrlIndexArray.Num();

		for (int32 i = 0; i < IndexCount; ++i)
		{
			UMaterialInstanceDynamic* Mtrl =
				GetMesh()->CreateDynamicMaterialInstance(mDissolveMtrlIndexArray[i].Index,
					mDissolveMtrlIndexArray[i].Mtrl);

			mDissolveMtrlArray.Add(Mtrl);
		}
	}

	mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	// Monster Skill
	int32 SkillCount = mSkillNameArray.Num();

	for (int32 i = 0; i < SkillCount; ++i)
	{
		const FSkillData* Data = GameInst->FindMonsterSkillTable(mSkillNameArray[i]);

		FMonsterSkillInfo SkillInfo;

		SkillInfo.Type = Data->Type;
		SkillInfo.System = Data->System;
		SkillInfo.SkillName = Data->SkillName;
		SkillInfo.SkillOptionArray = Data->SkillOptionArray;
		SkillInfo.Distance = Data->Distance;

		mSkillDataArray.Add(SkillInfo);
	}

	// Widget Component Init
	UMonsterHPBase* HPWidget = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());

	if (IsValid(HPWidget))
		HPWidget->SetInitHP(1.f);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mPatrolEnable)
	{
		mPatrolCurrentDistance += (GetCharacterMovement()->MaxWalkSpeed * DeltaTime * mPatrolIndexAdd);

		if (GetArrive())
		{
			if (mPatrolIndexAdd == 1)
				mPatrolCurrentDistance = mPatrolIndex * mPatrolCellDistance;

			else
				mPatrolCurrentDistance = mPatrolIndex * mPatrolCellDistance;

			mPatrolEnable = false;
		}
	}

	if (mDissolveEnable)
	{
		mDissolveTime += DeltaTime;

		// 총 시간에서의 비율을 구한다.
		float Ratio = mDissolveTime / mDissolveTimeMax;

		// 비율을 이용하여 Dissolve값의 전체 범위에 곱해서 얼마나 진행되었는지를 구한다.
		mDissolve = mDissolveMax - Ratio * mDissolveRange;

		int32 Count = mDissolveMtrlArray.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("Dissolve"), mDissolve);
		}

		if (mDissolveTime >= mDissolveTimeMax)
		{
			//mSpawnPoint->RemoveMonster(this);
			Destroy();
		}
	}

	// Delay
	CheckAttackDelayTime(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	int32 Damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage = Damage - mMonsterInfo.ArmorPoint;
	Damage = Damage < 1 ? 1 : Damage;

	mMonsterInfo.HP -= Damage;

	if (mMonsterInfo.HP <= 0)	// 죽음
	{
		UMonsterHPBase* HPWidget = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());
		HPWidget->SetHP(0.f);

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DamageCauser);

		float Ratio = PlayerCharacter->GetExpRatio();
		float ExpMax = PlayerCharacter->GetExpMax();

		Ratio += mMonsterInfo.Exp * (1.f / (float)(PlayerCharacter->GetPlayerInfo().ExpMax));
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Exp: %f"), Ratio));

		PlayerCharacter->SetExp(Ratio);
		PlayerCharacter->SetGold(mMonsterInfo.Gold);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mAnimInst->ChangeAnim(EMonsterAnimType::Death);

		AAIController* AI = Cast<AAIController>(GetController());

		if (IsValid(AI))
			AI->BrainComponent->StopLogic(TEXT("Death"));
		
		//mSpawnPoint->RemoveMonster(this);
	}


	else
	{
		mAnimInst->Hit();

		// HP UI
		UMonsterHPBase* HPWidget = Cast<UMonsterHPBase>(mWidgetComponent->GetWidget());

		if (IsValid(HPWidget))
		{
			mHPRatio -= Damage * (1.f / (float)mMonsterInfo.HPMax);
			HPWidget->SetHP(mHPRatio);
		}

		// Change Phase
		float HPRatio = (float)mMonsterInfo.HP / mMonsterInfo.MPMax;

		if (mHPRatio > 0.8f)
			mMonsterPhase = EMonsterPhase::NormalAttack;
		else if (mHPRatio > 0.6f && mHPRatio <= 0.8f)
			mMonsterPhase = EMonsterPhase::Skill1;
		else if (mHPRatio > 0.4f && mHPRatio <= 0.6f)
			mMonsterPhase = EMonsterPhase::Skill2;
		else
			mMonsterPhase = EMonsterPhase::Skill3;

	}

	return Damage;
}

void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMonsterAIController* AI = Cast<AMonsterAIController>(NewController);

	if (IsValid(AI))
	{
		AI->SetBehaviorTree(TEXT("BehaviorTree'/Game/Monster/AI/BTMonster.BTMonster'"));
		AI->SetBlackboard(TEXT("BlackboardData'/Game/Monster/AI/BBMonster.BBMonster'"));
	}
}

void AMonster::UnPossessed()
{
	Super::UnPossessed();
}

void AMonster::Attack()
{

}

void AMonster::Skill1()
{
}

void AMonster::Skill2()
{
}

void AMonster::Skill3()
{
}

void AMonster::CastSkill1()
{
}

void AMonster::CastSkill2()
{
}

void AMonster::SkillStart()
{
}

void AMonster::OnDissolve()
{
	if (mDissolveMtrlArray.IsEmpty())
		return;

	mDissolveEnable = true;

	int32 Count = mDissolveMtrlArray.Num();

	for (int32 i = 0; i < Count; ++i)
		mDissolveMtrlArray[i]->SetScalarParameterValue(TEXT("DissolveEnable"), 1.f);
}

void AMonster::DestroyWidgetComponent()
{
	if (IsValid(mWidgetComponent))
	{
		mWidgetComponent->DetachFromParent();
		mWidgetComponent->DestroyComponent();
		mWidgetComponent = nullptr;
	}
}

void AMonster::CheckAttackDelayTime(float DeltaTime)
{
	if (mAttackDelay)
	{
		mCurDelayTime += DeltaTime;

		if (mCurDelayTime >= mAttackDelayTime)
		{
			mAttackDelay = false;
			mCurDelayTime = 0.f;
		}
	}
}

void AMonster::CheckUseSkill()
{
}

void AMonster::PlayBossSkillCameraShake(FVector Location)
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), mBossSkillCameraShake, Location, 100.f, 1500, 1.f, false);
}

FVector AMonster::GetPatrolLocation() const
{
	switch (mPatrolType)
	{
	case EPatrolType::Point:
		return mPatrolPointLocationArray[mPatrolIndex];
	case EPatrolType::Spline:
		return mSpawnPoint->GetSplinePoint(mPatrolCurrentDistance);
	}

	return FVector::ZeroVector;
}

FVector AMonster::GetPatrolPointLocation() const
{
	switch (mPatrolType)
	{
	case EPatrolType::Point:
		return mPatrolPointLocationArray[mPatrolIndex];
	case EPatrolType::Spline:
		return mSpawnPoint->GetSplinePoint(mPatrolIndex * mPatrolCellDistance);
	}

	return FVector::ZeroVector;
}

void AMonster::NextPatrolPoint()
{
	mPatrolIndex += mPatrolIndexAdd;

	switch (mPatrolType)
	{
	case EPatrolType::Point:
		if (mPatrolIndex == mPatrolPointLocationArray.Num())
		{
			switch (mPatrolDir)
			{
			case EPatrolEndDir::Progress:
				mPatrolIndex = 0;
				break;
			case EPatrolEndDir::Repeat:
				mPatrolIndexAdd = -1;
				mPatrolIndex = mPatrolPointLocationArray.Num() - 2;
				break;
			}
		}

		else if (mPatrolIndex < 0)
		{
			mPatrolIndexAdd = 1;
			mPatrolIndex = 1;
		}
		break;
	case EPatrolType::Spline:
		if (mPatrolIndex == mPatrolSplineCount + 1)
		{
			switch (mPatrolDir)
			{
			case EPatrolEndDir::Progress:
				mPatrolIndex = 1;
				mPatrolCurrentDistance -= mPatrolSplineLength;
				break;
			case EPatrolEndDir::Repeat:
				mPatrolCurrentDistance = mPatrolSplineLength - 100.f -
					GetCapsuleComponent()->GetScaledCapsuleRadius();
				mPatrolIndexAdd = -1;
				mPatrolIndex = mPatrolSplineCount - 1;
				break;
			}
		}

		else if (mPatrolIndex < 0)
		{
			mPatrolCurrentDistance = 100.f +
				GetCapsuleComponent()->GetScaledCapsuleRadius();
			mPatrolIndexAdd = 1;
			mPatrolIndex = 1;
		}
		break;
	}
}

