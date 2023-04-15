// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"

// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mNiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));
	mNiagaraEffect->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;

	mSound = nullptr;
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(mSound))
		UGameplayStatics::PlaySoundAtLocation(this, mSound, GetActorLocation());

	const FString& Path = TEXT("NiagaraSystem'/Game/Sci-Fi_Starter_VFX_Pack_Niagara/Niagara/Shield/NS_Shield_5.NS_Shield_5'");
	UNiagaraSystem* Niagara = LoadObject<UNiagaraSystem>(nullptr, *Path);

	mNiagaraEffect->SetAsset(Niagara);
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShield::SetNiagara(UNiagaraSystem* Niagara)
{
	if (IsValid(Niagara))
		mNiagaraEffect->SetAsset(Niagara);
}

void AShield::SetNiagara(const FString& Path)
{
	UNiagaraSystem* Niagara = LoadObject<UNiagaraSystem>(nullptr, *Path);

	if (IsValid(Niagara))
		mNiagaraEffect->SetAsset(Niagara);
}

