// Fill out your copyright notice in the Description page of Project Settings.


#include "FireMinion.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"

AFireMinion::AFireMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	//SetActorRotation(FRotator(0.f, -180.f, 0.f));

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("FireMinion");

	FConvertMaterial Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void AFireMinion::BeginPlay()
{
	Super::BeginPlay();

}

void AFireMinion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AFireMinion::UnPossessed()
{
	Super::UnPossessed();
}

void AFireMinion::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());
	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	mAttackDelayTime = 2.f;

	if (IsValid(Target))
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Particle
		FVector Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(
			ParticleLoc, Dir.Rotation(), SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
		//Particle->SetSound(TEXT(""));

		Target->TakeDamage((float)mMonsterInfo.AttackPoint, FDamageEvent(), GetController(), this);
	}
}

void AFireMinion::Skill1()
{
	Attack();
}

void AFireMinion::Skill2()
{
	Attack();
}
