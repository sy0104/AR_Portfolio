// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class PF_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	AShield();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USphereComponent*		mRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent*		mNiagaraEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundBase*				mSound;

public:
	void SetNiagara(UNiagaraSystem* Niagara);
	void SetNiagara(const FString& Path);
};
