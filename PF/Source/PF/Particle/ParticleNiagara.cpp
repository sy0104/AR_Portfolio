// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleNiagara.h"

AParticleNiagara::AParticleNiagara()
{
	mParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	mParticle->SetupAttachment(mAudio);
}

void AParticleNiagara::BeginPlay()
{
	Super::BeginPlay();

	mParticle->OnSystemFinished.AddDynamic(this, &AParticleNiagara::ParticleFinish);
}

void AParticleNiagara::SetParticle(UNiagaraSystem* Particle)
{
	if (IsValid(Particle))
		mParticle->SetAsset(Particle);
}

void AParticleNiagara::SetParticle(const FString& Path)
{
	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(nullptr, *Path);

	if (IsValid(Particle))
		mParticle->SetAsset(Particle);
}

void AParticleNiagara::ParticleFinish(UNiagaraComponent* Particle)
{
	Destroy();
}
