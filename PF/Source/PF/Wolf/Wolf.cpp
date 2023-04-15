// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"
#include "WolfAnimInstance.h"
#include "../Particle/ParticleCascade.h"

// Sets default values
AWolf::AWolf()
{
	PrimaryActorTick.bCanEverTick = true;

	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WolfAsset(
		TEXT("SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Shinbi_Wolf/Meshes/Shinbi_Wolf.Shinbi_Wolf'"));
	
	if (WolfAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(WolfAsset.Object);
	}

	GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);
	GetCapsuleComponent()->SetCapsuleRadius(40.f);

	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// Anim Instance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(
		TEXT("AnimBlueprint'/Game/Wolf/ABWolf.ABWolf_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerAttack"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	mProjectile->SetUpdatedComponent(RootComponent);

	mProjectile->InitialSpeed = 800.f;
	mProjectile->ProjectileGravityScale = 0.1f;
	mProjectile->Bounciness = 0.6f;

	//mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	//mParticle->SetupAttachment(GetMesh());

	mAttackPoint = 0;
	mAttackDistance = 0.f;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();
	
	PrintViewport(1.f, FColor::Red, TEXT("Wolf::BeginPlay()"));

	mAnimInst = Cast<UWolfAnimInstance>(GetMesh()->GetAnimInstance());
	mAnimInst->SetAnimType(mWolfAnimType);

	// 스킬 값 세팅

	// 파티클
	//const FString& Path = TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/AttackWolves/FX/P_Mudang_AttackWolves_Impact.P_Mudang_AttackWolves_Impact'");
	//UParticleSystem* Particle = LoadObject<UParticleSystem>(nullptr, *Path);
	////mParticle->SetTemplate(Particle);

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mParticle = GetWorld()->SpawnActor<AParticleCascade>(
		GetActorLocation(), GetActorRotation(), SpawnParam);

	mParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/AttackWolves/FX/P_Mudang_AttackWolves_Impact.P_Mudang_AttackWolves_Impact'"));
}

// Called every frame
void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Attack();
}

// Called to bind functionality to input
void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWolf::Attack()
{
	//switch (mWolfSkillType)
	//{
	//case EWolfSkillType::LeapAttack:
	//	PrintViewport(1.f, FColor::Red, TEXT("Wolf - LeapAttack"));
	//	break;
	//case EWolfSkillType::RunAround:

	//	break;
	//}
}

