// Fill out your copyright notice in the Description page of Project Settings.


#include "CastWolfOne.h"
#include "../../Wolf/Wolf.h"

ACastWolfOne::ACastWolfOne()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACastWolfOne::BeginPlay()
{
	Super::BeginPlay();

	PrintViewport(1.f, FColor::Red, TEXT("CastWolfOne::BeginPlay()"));

	//AWolf* Wolf = NewObject<AWolf>(this, AWolf::StaticClass());
	//Wolf->SetWolfAnimType(mSkillInfo.WolfAnimType);

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWolf* Wolf = GetWorld()->SpawnActor<AWolf>(mSpawnLocation, mSpawnRotator, SpawnParam);
	Wolf->SetWolfAnimType(mSkillInfo.WolfAnimType);
}

void ACastWolfOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
