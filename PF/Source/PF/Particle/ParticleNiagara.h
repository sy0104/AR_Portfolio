// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleBase.h"
#include "ParticleNiagara.generated.h"


UCLASS()
class PF_API AParticleNiagara : public AParticleBase
{
	GENERATED_BODY()
	
public:
	AParticleNiagara();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetParticle(UNiagaraSystem* Particle);
	virtual void SetParticle(const FString& Path);

public:
	UFUNCTION()
	void ParticleFinish(UNiagaraComponent* Particle);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent*		mParticle;
};
