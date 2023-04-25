// Fill out your copyright notice in the Description page of Project Settings.


#include "Shinbi.h"
#include "PlayerAnimInstance.h"
#include "../Particle/ParticleNiagara.h"
#include "../Skill/WolfSkill/CastWolfOne.h"
#include "../Wolf/Wolf.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"

AShinbi::AShinbi()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SinbiAsset(
		TEXT("SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Skins/Tier_1/Shinbi_Jade/Meshes/ShinbiJade.ShinbiJade'"));

	if (SinbiAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SinbiAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass
	(TEXT("AnimBlueprint'/Game/Player/Shinbi/ABShinbi.ABShinbi_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);
}

void AShinbi::BeginPlay()
{
	Super::BeginPlay();

	// PlayerInfo 값 설정
	mPlayerInfo.Name = TEXT("Shinbi");
	//mPlayerInfo.AttackPoint = 500;	// 100
	//mPlayerInfo.ArmorPoint = 50;
	//mPlayerInfo.HP = 1000;
	//mPlayerInfo.HPMax = 1000;
	//mPlayerInfo.MP = 100;
	//mPlayerInfo.MPMax = 100;
	//mPlayerInfo.Level = 1;
	//mPlayerInfo.Exp = 0;
	//mPlayerInfo.ExpMax = 1000;
	//mPlayerInfo.Gold = 1000;
	//mPlayerInfo.MoveSpeed = 2000.f;
	//mPlayerInfo.AttackDistance = 200.f;

	InitUI();
}

void AShinbi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCameraZoom();

}

void AShinbi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<AShinbi>(TEXT("SkillQ"), EInputEvent::IE_Pressed, this, &AShinbi::SkillQKey);
	PlayerInputComponent->BindAction<AShinbi>(TEXT("SkillE"), EInputEvent::IE_Pressed, this, &AShinbi::SkillEKey);
	PlayerInputComponent->BindAction<AShinbi>(TEXT("SkillR"), EInputEvent::IE_Pressed, this, &AShinbi::SkillRKey);
	PlayerInputComponent->BindAction<AShinbi>(TEXT("SkillR"), EInputEvent::IE_Released, this, &AShinbi::SkillRKeyUp);
	PlayerInputComponent->BindAction<AShinbi>(TEXT("SkillF"), EInputEvent::IE_Pressed, this, &AShinbi::SkillFKey);

	PlayerInputComponent->BindAxis<AShinbi>(TEXT("SkillRTarget"), this, &AShinbi::SkillRTarget);
}

void AShinbi::InitUI()
{
	Super::InitUI();

}

void AShinbi::NormalAttackCheck()
{
	FVector StartLocation = GetActorLocation() + 
		GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + 
		GetActorForwardVector() * mPlayerInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, 
		StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6, 
		FCollisionShape::MakeSphere(50.f), param);

//#if ENABLE_DRAW_DEBUG
//	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;
//
//	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
//		mPlayerInfo.AttackDistance / 2.f, 50.f,
//		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
//		DrawColor, false, 0.5f);
//#endif

	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = 
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AParticleNiagara* Particle = GetWorld()->SpawnActor<AParticleNiagara>(
				CollisionResult[i].ImpactPoint, CollisionResult[i].ImpactNormal.Rotation(),
				SpawnParam);

			Particle->SetParticle(TEXT(
				"NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Impact/NS_Impact_Laser_2.NS_Impact_Laser_2'"));

			CollisionResult[i].GetActor()->TakeDamage((float)mPlayerInfo.AttackPoint,
				FDamageEvent(), GetController(), this);
		}
	}
}

void AShinbi::UseSkill(int32 SkillNumber)
{
	mShinbiSkillType = (EShinbiSkillType)SkillNumber;

	switch (mShinbiSkillType)
	{
	case EShinbiSkillType::Cast_Wolf_One:
		SkillQ();
		break;
	case EShinbiSkillType::Cast_Wolf_Circle:
		SkillE();
		break;
	case EShinbiSkillType::Cast_Wolf_Group:
		SkillR();
		break;
	case EShinbiSkillType::Dash:
		SkillF();
		break;
	}
}




void AShinbi::SkillQ()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillQ"));

	//ACastWolfOne* CastWolfOne = NewObject<ACastWolfOne>(this, ACastWolfOne::StaticClass());

	//CastWolfOne->SetSpawnLocation(this->GetActorLocation() + GetActorForwardVector() * 150.f);
	//CastWolfOne->SetSpawnRotator(this->GetActorRotation());
	//CastWolfOne->mSkillInfo.WolfAnimType = EWolfAnimType::Attack;
	// 
	// 늑대 소환
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWolf* Wolf = GetWorld()->SpawnActor<AWolf>(
		GetActorLocation() + GetActorForwardVector() * 150.f, 
		GetActorRotation(), SpawnParam);

}

void AShinbi::SkillE()
{
	// 늑대 원으로 둘러쌈
	PrintViewport(1.f, FColor::Red, TEXT("SkillE"));

}

void AShinbi::SkillR()
{
	// 조준해서 늑대 여러마리 소환
	PrintViewport(1.f, FColor::Red, TEXT("SkillR"));


	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = GetActorLocation() + GetActorForwardVector() * 180.f;
	float RandomRange = 10.f;

	for (int32 i = 0; i < 6; ++i)
	{
		FVector RandomLocation;
		RandomLocation.X = Location.X;
		RandomLocation.Y = FMath::RandRange(Location.Y - 150.f, Location.Y + 150.f);
		RandomLocation.Z = FMath::RandRange(Location.Z - 25.f, Location.Z + 25.f);

		FRotator RandomRotation;
		RandomRotation = GetActorRotation();
		RandomRotation.Yaw = FMath::RandRange(GetActorRotation().Yaw - 5.f, GetActorRotation().Yaw + 5.f);

		AWolf* Wolf = GetWorld()->SpawnActor<AWolf>(RandomLocation, RandomRotation, SpawnParam);
	}
}

void AShinbi::SkillF()
{
	//PrintViewport(1.f, FColor::Red, TEXT("SkillF"));

	const FVector ForwardDir = GetActorRotation().Vector();
	LaunchCharacter(ForwardDir * 20.f * 800.f, true, true);
}

void AShinbi::SkillQKey()
{
	mSkillKeyInput = EShinbiSkillType::Cast_Wolf_One;
}

void AShinbi::SkillEKey()
{
	mSkillKeyInput = EShinbiSkillType::Cast_Wolf_Circle;
}

void AShinbi::SkillRKey()
{
	mSkillKeyInput = EShinbiSkillType::Cast_Wolf_Group;
}

void AShinbi::SkillRKeyUp()
{
	mTargeting = false;
}

void AShinbi::SkillFKey()
{
	mSkillKeyInput = EShinbiSkillType::Dash;
}


void AShinbi::SkillRTarget(float Scale)
{
	if (Scale == 0.f)
		return;

	mTargeting = true;
}

void AShinbi::CheckCameraZoom()
{
	// SkillR - Targeting
	if (mSkillKeyInput == EShinbiSkillType::Cast_Wolf_Group)
	{
		if (mTargeting)
		{
			mSpringArm->TargetArmLength -= 10.f;

			if (mSpringArm->TargetArmLength < 200.f)
				mSpringArm->TargetArmLength = 200.f;
		}

		else if (!mTargeting && mSpringArm->TargetArmLength < 500.f)
		{
			mSpringArm->TargetArmLength += 10.f;

			//PrintViewport(1.f, FColor::Red, TEXT("Targeting End"));
		}
	}

	// SkillF - Dash
	else if (mSkillKeyInput == EShinbiSkillType::Dash)
	{
		if (mDash)
		{
			mSpringArm->TargetArmLength -= 20.f;

			if (mSpringArm->TargetArmLength < 200.f)
				mSpringArm->TargetArmLength = 200.f;
		}

		else if (!mDash && mSpringArm->TargetArmLength < 500.f)
		{
			mSpringArm->TargetArmLength += 20.f;
		}
	}
}

void AShinbi::SavePlayer()
{
	Super::SavePlayer();
}

void AShinbi::WolfSkillEnd(ASkillActor* SkillActor, const FHitResult& Hit)
{
}
