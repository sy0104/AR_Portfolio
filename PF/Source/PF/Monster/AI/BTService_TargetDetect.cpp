// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetDetect.h"
#include "../MonsterAIController.h"
#include "../Monster.h"

UBTService_TargetDetect::UBTService_TargetDetect()
{
	NodeName = TEXT("TargetDetect");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_TargetDetect::TickNode(UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Monster AI Controller
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(Controller))
		return;

	// Monster
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	
	if (!IsValid(Monster))
		return;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	// Ãæµ¹
	FCollisionQueryParams param(NAME_None, false, Monster);
	TArray<FOverlapResult> ResultArray;

	bool CollisionEnable = GetWorld()->OverlapMultiByChannel(ResultArray,
		Monster->GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(Info.TraceDistance), param);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugSphere(GetWorld(), Monster->GetActorLocation(),
		Info.TraceDistance, 20, DrawColor, false, 0.3f);

#endif

	if (CollisionEnable)
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), ResultArray[0].GetActor());
	}
	else
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}
