// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckSkill2.h"
#include "../MonsterAIController.h"
#include "../Monster.h"


UBTDecorator_CheckSkill2::UBTDecorator_CheckSkill2()
{
}

bool UBTDecorator_CheckSkill2::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	if (Monster->GetAttackDelay())
		return false;

	// Blackboard에 저장된 Target
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
		return false;


	return Monster->GetMonsterPhase() == EMonsterPhase::Skill2;
}
