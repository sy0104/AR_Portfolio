// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerShape.h"

ATriggerShape::ATriggerShape()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	mShape = ETriggerShape::Box;
}

void ATriggerShape::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATriggerShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerShape::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (mBox && mShape == ETriggerShape::Box)
		return;

	else if (mSphere && mShape == ETriggerShape::Capsule)
		return;

	else if (mCapsule && mShape == ETriggerShape::Sphere)
		return;

	if (mBox)
	{
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
	}

	if (mSphere)
	{
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
	}

	if (mCapsule)
	{
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
	}

	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
		mBox->RegisterComponent();
		mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Sphere"));
		mSphere->RegisterComponent();
		mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Capsule"));
		mCapsule->RegisterComponent();
		mCapsule->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}

	ChangeCollisionProfile(TEXT("PlayerTrigger"));
}

void ATriggerShape::TriggerBegin(const FHitResult& SweepResult)
{
}

void ATriggerShape::TriggerEnd()
{
}

void ATriggerShape::ChangeShape(ETriggerShape Shape)
{
	if (mShape == Shape)
		return;

	if (mBox)
	{
		mBox->DetachFromParent();
		mBox->DestroyComponent();
		mBox = nullptr;
	}

	else if (mSphere)
	{
		mSphere->DetachFromParent();
		mSphere->DestroyComponent();
		mSphere = nullptr;
	}

	else if (mCapsule)
	{
		mCapsule->DetachFromParent();
		mCapsule->DestroyComponent();
		mCapsule = nullptr;
	}

	switch (Shape)
	{
	case ETriggerShape::Box:
		mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
		mBox->RegisterComponent();
		mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Sphere:
		mSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Sphere"));
		mSphere->RegisterComponent();
		mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mSphere->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	case ETriggerShape::Capsule:
		mCapsule = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Capsule"));
		mCapsule->RegisterComponent();
		mCapsule->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

		mCapsule->OnComponentBeginOverlap.AddDynamic(this, &ATriggerShape::BeginOverlap);
		mCapsule->OnComponentEndOverlap.AddDynamic(this, &ATriggerShape::EndOverlap);
		break;
	}
}

void ATriggerShape::ChangeCollisionProfile(const FString& Name)
{
	switch (mShape)
	{
	case ETriggerShape::Box:
		mBox->SetCollisionProfileName(*Name);
		break;
	case ETriggerShape::Sphere:
		mSphere->SetCollisionProfileName(*Name);
		break;
	case ETriggerShape::Capsule:
		mCapsule->SetCollisionProfileName(*Name);
		break;
	}
}

void ATriggerShape::EnableCollision(bool Enable)
{
	switch (mShape)
	{
	case ETriggerShape::Box:
		if (!Enable)
			mBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else
			mBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ETriggerShape::Sphere:
		if (!Enable)
			mSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else
			mSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case ETriggerShape::Capsule:
		if (!Enable)
			mCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		else
			mCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void ATriggerShape::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerBegin(SweepResult);

}

void ATriggerShape::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TriggerEnd();
}

