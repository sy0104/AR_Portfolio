// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckMonsterPhase.h"
#include "../MonsterAIController.h"
#include "../Monster.h"

UBTDecorator_CheckMonsterPhase::UBTDecorator_CheckMonsterPhase()
{
}

bool UBTDecorator_CheckMonsterPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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


	return false;
}
