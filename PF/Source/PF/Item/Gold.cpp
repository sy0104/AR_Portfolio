// Fill out your copyright notice in the Description page of Project Settings.


#include "Gold.h"

AGold::AGold()
{
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	SetRootComponent(mCapsule);
	mMesh->SetupAttachment(mCapsule);

	const FString& Path = TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Coin_Pile_01f.SM_Coin_Pile_01f'");

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *Path);

	if (IsValid(Mesh))
		mMesh->SetStaticMesh(Mesh);

	mCapsule->SetCollisionProfileName(TEXT("Gold"));
	mCapsule->SetGenerateOverlapEvents(true);
	mCapsule->SetNotifyRigidBodyCollision(true);

	mCapsule->SetCapsuleHalfHeight(60.f);
	mCapsule->SetCapsuleRadius(60.f);

}

void AGold::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

