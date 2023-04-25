// Fill out your copyright notice in the Description page of Project Settings.


#include "Countess.h"
#include "PlayerAnimInstance.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "../Skill/SkillProjectile.h"
#include "../PFGameModeBase.h"
#include "../UMG/MainHUDBase.h"

ACountess::ACountess()
{
	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CountessAsset(
		TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));

	if (CountessAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(CountessAsset.Object);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(
		TEXT("AnimBlueprint'/Game/Player/Countess/ABCountess.ABCountess_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);


	mReturnEnable = false;
	mReturnTime = 2.f;
	mCurTime = 0.f;

	// Skill
	mSkillNameArray.Add(TEXT("CountessSkillQ"));
	mSkillNameArray.Add(TEXT("CountessSkillE"));
	mSkillNameArray.Add(TEXT("CountessSkillR"));
	mSkillNameArray.Add(TEXT("CountessSkillRM"));
}

void ACountess::BeginPlay()
{
	Super::BeginPlay();


	mPlayerInfo.Name = TEXT("Countess");
}

void ACountess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CheckCameraZoom();
	CheckReturn(DeltaTime);
}

void ACountess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 액션 매핑
	PlayerInputComponent->BindAction<ACountess>(TEXT("SkillQ"), EInputEvent::IE_Pressed, this, &ACountess::SkillQKey);
	PlayerInputComponent->BindAction<ACountess>(TEXT("SkillE"), EInputEvent::IE_Pressed, this, &ACountess::SkillEKey);
	PlayerInputComponent->BindAction<ACountess>(TEXT("SkillE"), EInputEvent::IE_Released, this, &ACountess::SkillEKeyUp);
	PlayerInputComponent->BindAction<ACountess>(TEXT("SkillR"), EInputEvent::IE_Pressed, this, &ACountess::SkillRKey);
	PlayerInputComponent->BindAction<ACountess>(TEXT("SkillRM"), EInputEvent::IE_Pressed, this, &ACountess::SkillRMKey);

	// 축 매핑
	PlayerInputComponent->BindAxis<ACountess>(TEXT("SkillETarget"), this, &ACountess::SkillETargeting);

}

void ACountess::InitUI()
{
	Super::InitUI();
}

void ACountess::NormalAttackCheck()
{
	//UGameplayStatics::PlayWorldCameraShake(GetWorld(), mCameraShake, GetActorLocation(), 100.f, 2000.f, 1.f, false);
	mPlayerInfo.AttackDistance = 200.f;

	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mPlayerInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult,
		StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
		mPlayerInfo.AttackDistance / 2.f, 50.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);
#endif

	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AParticleCascade* ImpactParticle =
				GetWorld()->SpawnActor<AParticleCascade>(
					CollisionResult[i].ImpactPoint,
					CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

			ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Primary/FX/p_CountessImpact.p_CountessImpact'"));

			CollisionResult[i].GetActor()->TakeDamage((float)mPlayerInfo.AttackPoint,
				FDamageEvent(), GetController(), this);
		}
	}
}

void ACountess::UseSkill(int32 SkillNumber)
{
	mCountessSkillType = (ECountessSkillType)SkillNumber;

	switch (mCountessSkillType)
	{
	case ECountessSkillType::BlinkStrike:
		SkillQ();
		break;
	case ECountessSkillType::RollingDark:
		SkillE();
		break;
	case ECountessSkillType::Ultimate:
		SkillR();
		break;
	case ECountessSkillType::BladeSiphon:
		SkillRM();
		break;
	}
}

void ACountess::SkillQ()
{
	SetHidden(false);

	mReturnEnable = true;

	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* FlashParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	FlashParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/p_Countess_BlinkStrikeFlash.p_Countess_BlinkStrikeFlash'"));

	AParticleCascade* TeleportParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	TeleportParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/P_Countess_TeleportBegin.P_Countess_TeleportBegin'"));

	// Skill
	int32 SkillNum = (int32)ECountessSkillType::BlinkStrike;
	mPlayerInfo.AttackDistance = mSkillDataArray[SkillNum].Distance;

	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mPlayerInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult,
		StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
		mPlayerInfo.AttackDistance / 2.f, 50.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);
#endif

	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			AParticleCascade* ImpactParticle =
				GetWorld()->SpawnActor<AParticleCascade>(
					CollisionResult[i].ImpactPoint,
					CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

			ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/P_Countess_BlinkStrike_HitFX.P_Countess_BlinkStrike_HitFX'"));

			int32 OptionCount = mSkillDataArray[SkillNum].SkillOptionArray.Num();
			float DamageRatio = 0.f;

			for (int32 j = 0; j < OptionCount; ++j)
			{
				if (mSkillDataArray[SkillNum].SkillOptionArray[j].Type == ESkillOptionType::Damage)
					DamageRatio += mSkillDataArray[SkillNum].SkillOptionArray[j].Option;
			}

			CollisionResult[i].GetActor()->TakeDamage((float)mPlayerInfo.AttackPoint * DamageRatio,
				FDamageEvent(), GetController(), this);
		}
	}
}

void ACountess::SkillE()
{
	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += 90.f;

	AParticleCascade* EffectParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), Rotation, SpawnParam);

	EffectParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/RollingDark/FX/p_RollingDark_SegmentFX.p_RollingDark_SegmentFX'"));

	// Skill
	int32 SkillNum = (int32)ECountessSkillType::RollingDark;
	mPlayerInfo.AttackDistance = mSkillDataArray[SkillNum].Distance;

	AParticleCascade* Particle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/RollingDark/FX/p_RollingDark_ImpactFX.p_RollingDark_ImpactFX'"));

	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mPlayerInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult,
		StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
		mPlayerInfo.AttackDistance / 2.f, 50.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);
#endif

	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			AParticleCascade* ImpactParticle =
				GetWorld()->SpawnActor<AParticleCascade>(
					CollisionResult[i].ImpactPoint,
					CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

			ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/P_Countess_BlinkStrike_HitFX.P_Countess_BlinkStrike_HitFX'"));

			int32 OptionCount = mSkillDataArray[SkillNum].SkillOptionArray.Num();
			float DamageRatio = 0.f;

			for (int32 j = 0; j < OptionCount; ++j)
			{
				if (mSkillDataArray[SkillNum].SkillOptionArray[j].Type == ESkillOptionType::Damage)
					DamageRatio += mSkillDataArray[SkillNum].SkillOptionArray[j].Option;
			}

			CollisionResult[i].GetActor()->TakeDamage((float)mPlayerInfo.AttackPoint * DamageRatio,
				FDamageEvent(), GetController(), this);
		}
	}


	//ASkillProjectile* Skill =
	//	GetWorld()->SpawnActor<ASkillProjectile>(
	//		GetActorLocation() + GetActorForwardVector() * 50.f, Rotation, SpawnParam);

	//Skill->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/RollingDark/FX/p_RollingDark_ImpactFX.p_RollingDark_ImpactFX'"));
	//Skill->SetCollisionProfile(TEXT("PlayerAttack"));
	//Skill->SetLifeSpan(4.f);
	//Skill->GetProjectile()->InitialSpeed = 4000.f;

	PlaySkillCameraShake();
}

void ACountess::SkillR()
{
	CustomTimeDilation = 2.5f;
	PlayUltimateCameraShake();
	SetCameraZoom(false, 10.f, false, 500.f, 700.f);

	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* SlashParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	SlashParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Ultimate/FX/p_CountessUlt_SlashFX.p_CountessUlt_SlashFX'"));

	AParticleCascade* Particle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Ultimate/FX/p_CountessUlt_CastFX.p_CountessUlt_CastFX'"));


	// Skill
	int32 SkillNum = (int32)ECountessSkillType::Ultimate;
	mPlayerInfo.AttackDistance = mSkillDataArray[SkillNum].Distance;

	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mPlayerInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult,
		StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f), param);

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
		mPlayerInfo.AttackDistance / 2.f, 50.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor, false, 0.5f);
#endif

	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			AParticleCascade* ImpactParticle =
				GetWorld()->SpawnActor<AParticleCascade>(
					CollisionResult[i].ImpactPoint,
					CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

			ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Ultimate/FX/p_CountessUltImpact.p_CountessUltImpact'"));

			int32 OptionCount = mSkillDataArray[SkillNum].SkillOptionArray.Num();
			float DamageRatio = 0.f;

			for (int32 j = 0; j < OptionCount; ++j)
			{
				if (mSkillDataArray[SkillNum].SkillOptionArray[j].Type == ESkillOptionType::Damage)
					DamageRatio += mSkillDataArray[SkillNum].SkillOptionArray[j].Option;
			}

			CollisionResult[i].GetActor()->TakeDamage((float)mPlayerInfo.AttackPoint * DamageRatio,
				FDamageEvent(), GetController(), this);
		}
	}

}

void ACountess::SkillRM()
{
	// SKill
	int32 SkillNum = (int32)ECountessSkillType::Ultimate;
	mPlayerInfo.AttackDistance = mSkillDataArray[SkillNum].Distance;

	FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
	FVector EndLocation = StartLocation + GetActorForwardVector() * mPlayerInfo.AttackDistance;

	FCollisionQueryParams param(NAME_None, false, this);

	TArray<FHitResult> CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult,
		StartLocation, EndLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(50.f), param);

	
#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

		DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
			mPlayerInfo.AttackDistance / 2.f,
			50.f,
			FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
			DrawColor, false, 0.5f);
#endif



	if (CollisionEnable)
	{
		int32 Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AParticleCascade* ImpactParticle =
				GetWorld()->SpawnActor<AParticleCascade>(
					CollisionResult[i].ImpactPoint,
					CollisionResult[i].ImpactNormal.Rotation(),
					SpawnParam);

			ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BladeSiphon/FX/p_Countess_BladeSiphon_HitFX.p_Countess_BladeSiphon_HitFX'"));

			int32 OptionCount = mSkillDataArray[SkillNum].SkillOptionArray.Num();
			float DamageRatio = 0.f;

			for (int32 j = 0; j < OptionCount; ++j)
			{
				if (mSkillDataArray[SkillNum].SkillOptionArray[j].Type == ESkillOptionType::Damage)
					DamageRatio += mSkillDataArray[SkillNum].SkillOptionArray[j].Option;
			}

			CollisionResult[i].GetActor()->TakeDamage((float)mPlayerInfo.AttackPoint * DamageRatio,
				FDamageEvent(), GetController(), this);
		}
	}
}

void ACountess::SkillQKey()
{
	if (mDeath || mTargeting)
		return;

	mCountessSkillType = ECountessSkillType::BlinkStrike;

	if (!mReturnEnable)
	{
		mReturnLocation = GetActorLocation();
		mReturnRotation = GetActorRotation().Vector();

		const FVector ForwardDir = GetActorRotation().Vector();
		LaunchCharacter(ForwardDir * 20.f * 600.f, true, true);

		mAnimInst->UseSkill(0);
	}

	else
	{
		//SetActorLocation(mReturnPos);

		mReturnEnable = false;
		mCurTime = 0.f;

		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetWorld()->GetFirstPlayerController(), mReturnPos);
		
		LaunchCharacter(-mReturnRotation * 20.f * 700.f, true, true);

		//const FVector ForwardDir = GetActorRotation().Vector();
		//LaunchCharacter(-ForwardDir * 20.f * 700.f, true, true);

		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* TeleportParticle =
			GetWorld()->SpawnActor<AParticleCascade>(
				mReturnLocation, GetActorRotation(), SpawnParam);

		TeleportParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/P_Countess_TeleportBegin.P_Countess_TeleportBegin'"));
	

		AParticleCascade* TeleportArriveParticle =
			GetWorld()->SpawnActor<AParticleCascade>(
				mReturnLocation, GetActorRotation(), SpawnParam);

		TeleportArriveParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/P_Countess_TeleportArrive.P_Countess_TeleportArrive'"));
	}
}

void ACountess::SkillEKey()
{
	if (mDeath || mTargeting)
		return;

	mCountessSkillType = ECountessSkillType::RollingDark;

}

void ACountess::SkillRKey()
{
	if (mDeath || mTargeting)
		return;

	mCountessSkillType = ECountessSkillType::Ultimate;
	mAnimInst->Ultimate();
}

void ACountess::SkillRMKey()
{
	if (mDeath || mTargeting)
		return;

	mCountessSkillType = ECountessSkillType::BladeSiphon;

}

void ACountess::SkillETargeting(float Scale)
{
	if (Scale == 0.f)
		return;

	mTargeting = true;
	mAnimInst->SkillETarget();
}

void ACountess::SkillEKeyUp()
{
	if (mDeath)
		return;
	
	mTargeting = false;
	mAnimInst->UseSkill(1);
}

void ACountess::CheckCameraZoom()
{
	if (mTargeting)
	{
		mSpringArm->TargetArmLength += 10.f;

		if (mSpringArm->TargetArmLength >= 700.f)
			mSpringArm->TargetArmLength = 700.f;
	}

	else if (!mTargeting && mSpringArm->TargetArmLength > 500.f)
	{
		mSpringArm->TargetArmLength -= 10.f;

		//PrintViewport(1.f, FColor::Red, TEXT("Targeting End"));
	}
}

void ACountess::CheckReturn(float DeltaTime)
{
	if (mReturnEnable)
	{
		mCurTime += DeltaTime;

		if (mCurTime >= mReturnTime)
		{
			mReturnEnable = false;
			mCurTime = 0.f;
		}
	}
}

void ACountess::SkillQStart()
{
	SetHidden(true);

	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BlinkStrike/FX/P_Countess_TeleportBegin.P_Countess_TeleportBegin'"));
	
}

void ACountess::SkillRStart()
{
	PrintViewport(1.f, FColor::Red, TEXT("SkillR Start"));
	
	CustomTimeDilation = 0.7f;

	SetCameraZoom(true, 1.f, false, 500.f, 700.f);
}

void ACountess::SkillRCast()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* Particle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Ultimate/FX/p_CountessUlt_ApplicationFX.p_CountessUlt_ApplicationFX'"));

	AParticleCascade* GroundParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	GroundParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Ultimate/FX/p_CountessUlt_GroundImpactFX.p_CountessUlt_GroundImpactFX'"));

}

void ACountess::SkillRMCast()
{
	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* TrailParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	TrailParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BladeSiphon/FX/P_Countess_BladeSiphon_Trails.P_Countess_BladeSiphon_Trails'"));

	AParticleCascade* RingParticle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(), GetActorRotation(), SpawnParam);

	RingParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BladeSiphon/FX/P_Countess_BladeSiphon_RingFX.P_Countess_BladeSiphon_RingFX'"));

}

void ACountess::StartSkill()
{
	switch (mCountessSkillType)
	{
	case ECountessSkillType::BlinkStrike:	// Q
		SkillQStart();
		break;
	case ECountessSkillType::RollingDark:	// E
		break;
	case ECountessSkillType::Ultimate:		// R
		SkillRStart();
		break;
	case ECountessSkillType::BladeSiphon:	// RM
		break;
	}
}

void ACountess::SavePlayer()
{
	Super::SavePlayer();
}

void ACountess::SkillEEnd(ASkillActor* SkillActor, const FHitResult& Hit)
{
	//Hit.GetActor()->TakeDamage(100.f, FDamageEvent(), GetController(), this);

	//SkillActor->Destroy();
}
