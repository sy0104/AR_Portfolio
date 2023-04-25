// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckSkill3.generated.h"

/**
 * 
 */
UCLASS()
class PF_API UBTDecorator_CheckSkill3 : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckSkill3();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
