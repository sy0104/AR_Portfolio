// Fill out your copyright notice in the Description page of Project Settings.


#include "FireGrux.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "../Skill/SkillProjectile.h"

AFireGrux::AFireGrux()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -120.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("FireGrux");

	FConvertMaterial Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void AFireGrux::BeginPlay()
{
	Super::BeginPlay();
}

void AFireGrux::Attack()
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

				Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_Melee_ShockwaveImpact.P_Grux_Melee_ShockwaveImpact'"));
				
				CollisionResult[i].GetActor()->TakeDamage(
					(float)mMonsterInfo.AttackPoint,
					FDamageEvent(), GetController(), this);


			}
		}
	}
}

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

void AFireGrux::Skill1()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 1.5f;
	mMonsterInfo.AttackDistance = 250.f;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Effect
	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			GetActorLocation() + GetActorForwardVector() * 150.f, 
			GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Explosion/NS_Explosion_Sand_2.NS_Explosion_Sand_2'"));

	if (IsValid(Target))
	{
		// Collision
		FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 30.f;
		FVector EndLocation = StartLocation + GetActorForwardVector() * mMonsterInfo.AttackDistance;

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

void AFireGrux::Skill2()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 2.f;
	mMonsterInfo.AttackDistance = 1000.f;

	FActorSpawnParameters	SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Skill
	ASkillProjectile* Skill =
		GetWorld()->SpawnActor<ASkillProjectile>(
			GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);

	Skill->SetNiagara(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Laser/NS_Laser_3.NS_Laser_3'"));
	Skill->SetCollisionProfile(TEXT("PlayerTrigger"));
	Skill->SetLifeSpan(4.f);
	Skill->GetProjectile()->InitialSpeed = 500.f;
	Skill->mOnSkillEnd.AddDynamic(this, &AFireGrux::Skill2End);

	//// Effect
	//AParticleNiagara* Particle =
	//	GetWorld()->SpawnActor<AParticleNiagara>(
	//		GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);

	//Particle->SetParticle(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Impact/NS_Impact_Metal_1.NS_Impact_Metal_1'"));
}

void AFireGrux::Skill3()
{
	Skill2();
}

void AFireGrux::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AFireGrux::UnPossessed()
{
	Super::UnPossessed();
}

void AFireGrux::Skill1End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	Hit.GetActor()->TakeDamage(100.f, FDamageEvent(), GetController(), this);

	//SkillActor->Destroy();
}

void AFireGrux::Skill2End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	Hit.GetActor()->TakeDamage(200.f, FDamageEvent(), GetController(), this);

	//SkillActor->Destroy();
}

