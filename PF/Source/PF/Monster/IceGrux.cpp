// Fill out your copyright notice in the Description page of Project Settings.


#include "IceGrux.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "../Skill/SkillProjectile.h"

AIceGrux::AIceGrux()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -120.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("IceGrux");

	FConvertMaterial Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);

}

void AIceGrux::BeginPlay()
{
	Super::BeginPlay();
}

void AIceGrux::Attack()
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

#if ENABLE_DRAW_DEBUG
		FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

		DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f,
			mMonsterInfo.AttackDistance / 2.f,
			50.f,
			FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
			DrawColor, false, 0.5f);
#endif

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

				Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_Melee_SucessfulImpact.P_Grux_Melee_SucessfulImpact'"));
				Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

				CollisionResult[i].GetActor()->TakeDamage(
					(float)mMonsterInfo.AttackPoint,
					FDamageEvent(), GetController(), this);
			}
		}

		//// Particle
		//FActorSpawnParameters SpawnParam;
		//SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//FVector Dir = GetActorLocation() - Target->GetActorLocation();
		//Dir.Normalize();

		//FVector ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		//AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(
		//	ParticleLoc, Dir.Rotation(), SpawnParam);

		//Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_Melee_SucessfulImpact.P_Grux_Melee_SucessfulImpact'"));
		////Particle->SetSound(TEXT(""));

		//Target->TakeDamage((float)mMonsterInfo.AttackPoint, FDamageEvent(), GetController(), this);
	}
}

void AIceGrux::Skill1()
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

	Skill->SetNiagara(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Environment/NS_Environment_Lightning_2.NS_Environment_Lightning_2'"));
	Skill->SetBoxExtent(FVector(50.f, 100.f, 50.f));
	Skill->SetCollisionProfile(TEXT("MonsterAttack"));
	Skill->mOnSkillEnd.AddDynamic(this, &AIceGrux::Skill1End);
	Skill->SetLifeSpan(4.f);

	// Effect
	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Explosion/NS_Explosion_Grenade_2.NS_Explosion_Grenade_2'"));
	//Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));
}

void AIceGrux::SKill2()
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

	Skill->SetNiagara(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Environment/NS_Environment_Tornado_2.NS_Environment_Tornado_2'"));
	Skill->SetCollisionProfile(TEXT("PlayerTrigger"));
	Skill->SetLifeSpan(4.f);
	Skill->GetProjectile()->InitialSpeed = 6000.f;
	
	//// Effect
	//AParticleNiagara* Particle =
	//	GetWorld()->SpawnActor<AParticleNiagara>(
	//		GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);

	//Particle->SetParticle(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Impact/NS_Impact_Metal_1.NS_Impact_Metal_1'"));

}

void AIceGrux::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AMonsterAIController* AI = Cast<AMonsterAIController>(NewController);

	//if (IsValid(AI))
	//{
	//	AI->SetBehaviorTree(TEXT("BehaviorTree'/Game/Monster/AI/BTGrux.BTGrux'"));
	//	AI->SetBlackboard(TEXT("BlackboardData'/Game/Monster/AI/BBGrux.BBGrux'"));
	//}
}

void AIceGrux::UnPossessed()
{
	Super::UnPossessed();
}

void AIceGrux::Skill1End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	PrintViewport(1.f, FColor::Red, TEXT("Skill1End"));

	Hit.GetActor()->TakeDamage(100.f, FDamageEvent(), GetController(), this);

	SkillActor->Destroy();
}

void AIceGrux::Skill2End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	Hit.GetActor()->TakeDamage(100.f, FDamageEvent(), GetController(), this);
}
