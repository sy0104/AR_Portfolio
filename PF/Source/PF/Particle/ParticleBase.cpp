// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleBase.h"

AParticleBase::AParticleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	mAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	SetRootComponent(mAudio);
}

void AParticleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AParticleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleBase::SetSound(const FString& Path, bool Play)
{
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *Path);
}

void AParticleBase::SetSound(USoundBase* Sound, bool Play)
{
	if (IsValid(Sound))
	{
		mAudio->SetSound(Sound);

		if (Play)
			mAudio->Play();
	}
}

void AParticleBase::SetParticle(UParticleSystem* Particle)
{
}

void AParticleBase::SetParticle(UNiagaraSystem* Particle)
{
}

void AParticleBase::SetParticle(const FString& Path)
{
}

