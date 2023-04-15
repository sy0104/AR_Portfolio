// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
#include "Monster.h"
#include "AI/MonsterPatrolPoint.h"

AMonsterSpawnPoint::AMonsterSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mPatrolSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));

	SetRootComponent(mRoot);
	mPatrolSpline->SetupAttachment(mRoot);
	mRoot->bVisualizeComponent = true;

	mSpawnTime = 3.f;
	mTime = 0.f;
	mSpawnCount = 1;

	mPatrolType = EPatrolType::Point;

	mDivideCount = 5;
	mDivideMeshVisible = true;
	mSplineLength = 0.f;
	mCellDistance = 0.f;
	mPrevDivideCount = 0;
	mPrevLength = 0.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DivideMesh(TEXT("StaticMesh'/Game/Stylized_Egypt/Meshes/stones/SM_stone_02.SM_stone_02'"));

	if (DivideMesh.Succeeded())
		mDivideMesh = DivideMesh.Object;
}

void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	mSplineLength = mPatrolSpline->GetSplineLength();
	mCellDistance = mSplineLength / mDivideCount;

	// SpawnPoint 위치 추가
	mPatrolPointLocationArray.Add(GetActorLocation());

	int32 PatrolPointCount = mPatrolPointArray.Num();

	for (int32 i = 0; i < PatrolPointCount; ++i)
		mPatrolPointLocationArray.Add(mPatrolPointArray[i]->GetActorLocation());

	// 생성할 몬스터 크래스가 있을 경우에만 진행
	if (IsValid(mSpawnClass))
	{
		mSpawnCount = mSpawnCount < 1 ? 1 : mSpawnCount;

		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMonster* SpawnMonster = GetWorld()->SpawnActor<AMonster>(mSpawnClass,
			GetActorLocation(), GetActorRotation(), SpawnParam);

		SpawnMonster->SetSpawnPoint(this);
		SpawnMonster->SetPatrolPointLocation(mPatrolPointLocationArray);
		SpawnMonster->SetPatrolDir(mPatrolDir);
		SpawnMonster->SetPatrolType(mPatrolType);
		SpawnMonster->SetPatrolSplineLength(mSplineLength);
		SpawnMonster->SetPatrolCellDistance(mCellDistance);
		SpawnMonster->SetPatrolSplineCount(mDivideCount);

		mMonsterArray.Add(SpawnMonster);
	}
}

void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(mSpawnClass))
	{
		if (mMonsterArray.Num() < mSpawnCount)
		{
			mTime += DeltaTime;

			if (mTime >= mSpawnTime)
			{
				mTime -= mSpawnTime;

				FActorSpawnParameters SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AMonster* SpawnMonster = GetWorld()->SpawnActor<AMonster>(mSpawnClass,
					GetActorLocation(), GetActorRotation(), SpawnParam);

				SpawnMonster->SetSpawnPoint(this);
				SpawnMonster->SetPatrolPointLocation(mPatrolPointLocationArray);
				SpawnMonster->SetPatrolDir(mPatrolDir);
				SpawnMonster->SetPatrolType(mPatrolType);
				SpawnMonster->SetPatrolSplineLength(mSplineLength);
				SpawnMonster->SetPatrolCellDistance(mCellDistance);
				SpawnMonster->SetPatrolSplineCount(mDivideCount);

				mMonsterArray.Add(SpawnMonster);

				if (mMonsterArray.Num() == mSpawnCount)
					mTime = 0.f;
			}
		}
	}
}

void AMonsterSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ComputeSpline();
}

void AMonsterSpawnPoint::ComputeSpline()
{
	if (mDivideMeshVisible)
	{
		if (mDivideCount != mPrevDivideCount)
		{
			int32 ComCount = mMeshArray.Num();

			for (int32 i = 0; i < ComCount; ++i)
			{
				if (mMeshArray[i])
					mMeshArray[i]->DestroyComponent();
			}

			if (mMeshArray.Num() > 0)
				mMeshArray.RemoveAll([](UStaticMeshComponent*) { return true; });
		}
	}

	else
	{
		int32 ComCount = mMeshArray.Num();

		for (int32 i = 0; i < ComCount; ++i)
		{
			if (mMeshArray[i])
				mMeshArray[i]->DestroyComponent();
		}

		if (mMeshArray.Num() > 0)
			mMeshArray.RemoveAll([](UStaticMeshComponent*) { return true; });

		mSplinePoint.RemoveAll([](FVector) { return true; });
		mSplineRotation.RemoveAll([](FRotator) { return true; });

		mSplineLength = mPatrolSpline->GetSplineLength();
		mCellDistance = mSplineLength / mDivideCount;

		FTransform ActorTr = GetActorTransform();

		for (int32 i = 0; i <= mDivideCount; ++i)
		{
			FVector Point = mPatrolSpline->GetLocationAtDistanceAlongSpline(
				mCellDistance * i, ESplineCoordinateSpace::World);

			mSplinePoint.Add(Point);

			FVector LocalPoint = ActorTr.InverseTransformPosition(Point);

			FRotator Rot = mPatrolSpline->GetRotationAtDistanceAlongSpline(
				mCellDistance * i, ESplineCoordinateSpace::World);

			mSplineRotation.Add(Rot);

			if (mDivideMeshVisible)
			{
				if (mDivideCount != mPrevDivideCount)
				{
					UStaticMeshComponent* MeshCom = NewObject<UStaticMeshComponent>(this);
					FTransform	MeshTr = MeshCom->GetRelativeTransform();

					MeshTr.SetLocation(LocalPoint);
					MeshTr.SetScale3D(FVector(0.1f, 0.1f, 0.1f));

					MeshCom->SetRelativeTransform(MeshTr);
					MeshCom->SetWorldRotation(Rot);
					MeshCom->SetStaticMesh(mDivideMesh);
					MeshCom->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					mMeshArray.Add(MeshCom);

					MeshCom->SetupAttachment(mPatrolSpline);
				}

				else
				{
					mMeshArray[i]->SetRelativeLocation(LocalPoint);
					mMeshArray[i]->SetWorldRotation(Rot);
				}

			}
		}
	}

	mPrevDivideCount = mDivideCount;
	mPrevLength = mSplineLength;
}

