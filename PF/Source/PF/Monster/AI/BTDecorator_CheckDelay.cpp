// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDelay.h"
#include "../MonsterAIController.h"
#include "../Monster.h"

UBTDecorator_CheckDelay::UBTDecorator_CheckDelay()
{
}

bool UBTDecorator_CheckDelay::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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


	return Monster->GetAttackDelay() == false;
}
