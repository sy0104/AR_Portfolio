// Fill out your copyright notice in the Description page of Project Settings.


#include "Sevarog.h"

ASevarog::ASevarog()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(160.f);
	GetCapsuleComponent()->SetCapsuleRadius(45.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -160.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("Sevarog");

	FConvertMaterial Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void ASevarog::BeginPlay()
{
	Super::BeginPlay();
}

void ASevarog::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ASevarog::UnPossessed()
{
	Super::UnPossessed();
}

void ASevarog::Attack()
{
}

void ASevarog::Skill1()
{
}

void ASevarog::Skill2()
{
}
