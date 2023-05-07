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

	mSkillNameArray.Add(TEXT("IceGruxSkill1"));
	mSkillNameArray.Add(TEXT("IceGruxSkill2"));
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
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


				AParticleCascade* Particle =
					GetWorld()->SpawnActor<AParticleCascade>(
						CollisionResult[i].ImpactPoint, CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

				Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_Melee_SucessfulImpact.P_Grux_Melee_SucessfulImpact'"));

				CollisionResult[i].GetActor()->TakeDamage((float)mMonsterInfo.AttackPoint, FDamageEvent(), GetController(), this);
			}
		}
	}
}

void AIceGrux::Skill1()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	int32 SkillNum = (int32)EIceGruxSkill::Skill1;
	mMonsterInfo.AttackDistance = mSkillDataArray[SkillNum].Distance;

	mAttackDelayTime = 2.f;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

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
}

void AIceGrux::Skill2()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	int32 SkillNum = (int32)EIceGruxSkill::Skill1;
	mMonsterInfo.AttackDistance = mSkillDataArray[SkillNum].Distance;

	mAttackDelayTime = 2.f;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Skill
	ASkillProjectile* Skill =
		GetWorld()->SpawnActor<ASkillProjectile>(
			GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);

	Skill->SetNiagara(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Environment/NS_Environment_Tornado_2.NS_Environment_Tornado_2'"));
	Skill->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	Skill->SetCollisionProfile(TEXT("MonsterAttack"));
	Skill->SetLifeSpan(4.f);
	Skill->GetProjectile()->InitialSpeed = 1000.f;
	Skill->mOnSkillEnd.AddDynamic(this, &AIceGrux::Skill2End);
}

void AIceGrux::Skill3()
{
	Skill2();
}

void AIceGrux::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AIceGrux::UnPossessed()
{
	Super::UnPossessed();
}

void AIceGrux::Skill1End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	PrintViewport(1.f, FColor::Red, TEXT("Skill1End"));

	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			Hit.GetActor()->GetActorLocation(), Hit.GetActor()->GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Explosion/NS_Explosion_Rocket_2.NS_Explosion_Rocket_2'"));

	// Damage
	int32 SkillNum = (int32)EIceGruxSkill::Skill1;

	int32 Count = mSkillDataArray[SkillNum].SkillOptionArray.Num();
	float DamageRatio = 0.f;

	for (int32 i = 0; i < Count; ++i)
	{
		if (mSkillDataArray[SkillNum].SkillOptionArray[i].Type == ESkillOptionType::Damage)
			DamageRatio += mSkillDataArray[SkillNum].SkillOptionArray[i].Option;
	}

	Hit.GetActor()->TakeDamage((float)mMonsterInfo.AttackPoint * DamageRatio,
		FDamageEvent(), GetController(), this);

	SkillActor->Destroy();
}

void AIceGrux::Skill2End(ASkillActor* SkillActor, const FHitResult& Hit)
{
	PrintViewport(1.f, FColor::Red, TEXT("Skill2End"));

	// Effect
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			Hit.GetActor()->GetActorLocation(), Hit.GetActor()->GetActorRotation(), SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Explosion/NS_Explosion_Grenade_2.NS_Explosion_Grenade_2'"));

	// Damage
	int32 SkillNum = (int32)EIceGruxSkill::Skill2;

	int32 Count = mSkillDataArray[SkillNum].SkillOptionArray.Num();
	float DamageRatio = 0.f;

	for (int32 i = 0; i < Count; ++i)
	{
		if (mSkillDataArray[SkillNum].SkillOptionArray[i].Type == ESkillOptionType::Damage)
			DamageRatio += mSkillDataArray[SkillNum].SkillOptionArray[i].Option;
	}
	
	Hit.GetActor()->TakeDamage((float)mMonsterInfo.AttackPoint * DamageRatio, 
		FDamageEvent(), GetController(), this);

	SkillActor->Destroy();
}
