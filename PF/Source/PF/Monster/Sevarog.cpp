// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "../Skill/SkillProjectile.h"

ASevarog::ASevarog()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(160.f);
	GetCapsuleComponent()->SetCapsuleRadius(45.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -160.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("Sevarog");

	FConvertMaterial Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void ASevarog::BeginPlay()
{
	Super::BeginPlay();
}

void ASevarog::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ASevarog::UnPossessed()
{
	Super::UnPossessed();
}

void ASevarog::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 1.f;
	mMonsterInfo.AttackDistance = 250.f;

	if (IsValid(Target))
	{
		// Collision
		FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
		FVector EndLocation = StartLocation + GetActorForwardVector() * mMonsterInfo.AttackDistance;

		FCollisionQueryParams param(NAME_None, false, this);

		TArray<FHitResult>	CollisionResult;
		bool CollisionEnable = GetWorld()->SweepMultiByChannel(
			CollisionResult, StartLocation,
			EndLocation, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel7,
			FCollisionShape::MakeSphere(50.f),
			param);
//
//#if ENABLE_DRAW_DEBUG
//		FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;
//
//		DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
//			mMonsterInfo.AttackDistance / 2.f,
//			50.f,
//			FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
//			DrawColor, false, 0.5f);
//#endif

		if (CollisionEnable)
		{
			int32 Count = CollisionResult.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				FActorSpawnParameters	SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


				AParticleCascade* Particle =
					GetWorld()->SpawnActor<AParticleCascade>(
						CollisionResult[i].ImpactPoint,
						CollisionResult[i].ImpactNormal.Rotation(),
						SpawnParam);

				Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Primary/FX/P_Sevarog_Melee_SucessfulImpact.P_Sevarog_Melee_SucessfulImpact'"));
				//Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

				CollisionResult[i].GetActor()->TakeDamage(
					(float)mMonsterInfo.AttackPoint,
					FDamageEvent(), GetController(), this);
			}
		}
	}
}


//#if ENABLE_DRAW_DEBUG
//		FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;
//
//		DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
//			mMonsterInfo.AttackDistance / 2.f,
//			25.f,
//			FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
//			DrawColor, false, 0.5f);
//#endif

void ASevarog::Skill1()
{
	mSkillType = ESevarogSkill::Skill1;

	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 1.5f;
	mMonsterInfo.AttackDistance = 1000.f;

	if (IsValid(Target))
	{
		// Collision
		FVector StartLocation = CastLoc + CastForward;
		FVector EndLocation = StartLocation + CastForward * 50.f;

		FCollisionQueryParams param(NAME_None, false, this);

		TArray<FHitResult>	CollisionResult;
		bool CollisionEnable = GetWorld()->SweepMultiByChannel(
			CollisionResult, StartLocation, EndLocation, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel7, FCollisionShape::MakeSphere(50.f), param);

		if (CollisionEnable)
		{
			int32 Count = CollisionResult.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				CollisionResult[i].GetActor()->TakeDamage(
					(float)mMonsterInfo.AttackPoint,
					FDamageEvent(), GetController(), this);
			}
		}
	}
}

void ASevarog::Skill2()
{
	mSkillType = ESevarogSkill::Skill2;

	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 1.f;
	mMonsterInfo.AttackDistance = 250.f;

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(Target))
	{
		// Impact
		AParticleCascade* ImpactParticle =
			GetWorld()->SpawnActor<AParticleCascade>(
				Target->GetActorLocation(), Target->GetActorRotation(), SpawnParam);
		ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/SoulSiphon/FX/P_SiphonImpact.P_SiphonImpact'"));

		// Collision
		FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
		FVector EndLocation = StartLocation + GetActorForwardVector() * mMonsterInfo.AttackDistance;

		FCollisionQueryParams param(NAME_None, false, this);

		TArray<FHitResult>	CollisionResult;
		bool CollisionEnable = GetWorld()->SweepMultiByChannel(
			CollisionResult, StartLocation,
			EndLocation, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel7,
			FCollisionShape::MakeSphere(50.f),
			param);

//#if ENABLE_DRAW_DEBUG
//		FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;
//
//		DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
//			mMonsterInfo.AttackDistance / 2.f,
//			50.f,
//			FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
//			DrawColor, false, 0.5f);
//#endif

		if (CollisionEnable)
		{
			int32 Count = CollisionResult.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				// Target °¨½Î´Â ÀÌÆåÆ®
				AParticleCascade* TargetParticle =
					GetWorld()->SpawnActor<AParticleCascade>(
						CollisionResult[i].GetActor()->GetActorLocation(), 
						CollisionResult[i].GetActor()->GetActorRotation(), SpawnParam);

				TargetParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/SoulSiphon/FX/P_SoulSwirlsBody.P_SoulSwirlsBody'"));

				CollisionResult[i].GetActor()->TakeDamage(
					(float)mMonsterInfo.AttackPoint,
					FDamageEvent(), GetController(), this);
			}
		}
	}
}

void ASevarog::Skill3()
{
	mSkillType = ESevarogSkill::Skill3;

	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 1.f;
	mMonsterInfo.AttackDistance = 400.f;

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(Target))
	{
		// Attack Effect
		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				Target->GetActorLocation(), Target->GetActorRotation(), SpawnParam);
		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_UltMeshArc_Expanding.P_UltMeshArc_Expanding'"));

		// Collision
		FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
		FVector EndLocation = StartLocation + GetActorForwardVector() * mMonsterInfo.AttackDistance;

		FCollisionQueryParams param(NAME_None, false, this);

		TArray<FHitResult>	CollisionResult;
		bool CollisionEnable = GetWorld()->SweepMultiByChannel(
			CollisionResult, StartLocation,
			EndLocation, FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel7,
			FCollisionShape::MakeSphere(50.f),
			param);

//#if ENABLE_DRAW_DEBUG
//		FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;
//
//		DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
//			mMonsterInfo.AttackDistance / 2.f,
//			100.f,
//			FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
//			DrawColor, false, 0.5f);
//#endif

		if (CollisionEnable)
		{
			int32 Count = CollisionResult.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				// Target °¨½Î´Â ÀÌÆåÆ®
				AParticleCascade* ImpactParticle =
					GetWorld()->SpawnActor<AParticleCascade>(
						CollisionResult[i].GetActor()->GetActorLocation(),
						CollisionResult[i].GetActor()->GetActorRotation(), SpawnParam);

				ImpactParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_UltActivate.P_UltActivate'"));

				CollisionResult[i].GetActor()->TakeDamage(
					(float)mMonsterInfo.AttackPoint,
					FDamageEvent(), GetController(), this);
			}
		}
	}
}

void ASevarog::CastSkill1()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(
		MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		CastLoc = Target->GetActorLocation();
		CastForward = Target->GetActorForwardVector();

		// Effect
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				Target->GetActorLocation(), GetActorRotation(), SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Subjugate/FX/P_SubjugateSwirls.P_SubjugateSwirls'"));
	}

	PlayBossSkillCameraShake(Target->GetActorLocation());
}

void ASevarog::CastSkill2()
{
	//CustomTimeDilation = 4.f;

	// Soul Siphon - GhostHand ÀÌÆåÆ® »ý¼º
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		// Effect
		FActorSpawnParameters	SpawnParam;

		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				GetActorLocation(), GetActorRotation(), SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/SoulSiphon/FX/P_GhostHand.P_GhostHand'"));
		//Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));
	}
}

void ASevarog::SkillStart()
{
	switch (mSkillType)
	{
	case ESevarogSkill::Skill1:
		break;
	case ESevarogSkill::Skill2:
		Skill2Start();
		break;
	case ESevarogSkill::Skill3:
		break;
	}
}

void ASevarog::Skill2Start()
{
	CustomTimeDilation = 0.1f;
}

