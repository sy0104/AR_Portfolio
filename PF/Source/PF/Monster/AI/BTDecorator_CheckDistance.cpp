// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../../Player/PlayerCharacter.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// Monster AI Controller
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Controller))
		return false;

	// Monster
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());

	if (!IsValid(Monster))
		return false;

	// Blackboard에 저장된 Target
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	
	if (!IsValid(Target))
		return false;

	if (Cast<APlayerCharacter>(Target)->GetDeath())
		return false;

	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	// 몬스터 & 타겟 거리 계산
	FVector MonsterLoc = Monster->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();

	MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	float Distance = FVector::Distance(MonsterLoc, TargetLoc);
	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float Check = 0.f;

	switch (mCheckType)
	{
	case ECheckDistanceType::Attack:
		Check = Info.AttackDistance;
		break;
	case ECheckDistanceType::Trace:
		Check = Info.TraceDistance;
		break;
	}

	return Distance <= Check;
}
