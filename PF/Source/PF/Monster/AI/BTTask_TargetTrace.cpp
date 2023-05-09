// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetTrace.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

#include "../../Player/PlayerCharacter.h"

UBTTask_TargetTrace::UBTTask_TargetTrace()
{
	NodeName = TEXT("TargetTrace");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Monster AI Controller
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(Controller))
		return EBTNodeResult::Failed;

	// Monster Info
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	// Target ������ Trace, ������ Idle
	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	// Target X
	if (!IsValid(Target))
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);

		return EBTNodeResult::Failed;
	}

	// Target O -> Ÿ�� ��ġ�� �̵�
	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Target);
	Anim->ChangeAnim(EMonsterAnimType::Run);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_TargetTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(Controller))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	
	if (!IsValid(Monster))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();
	
	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(Target))
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	if (Cast<APlayerCharacter>(Target)->GetDeath())
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	// �̵� �� Ÿ�ٰ��� �Ÿ��� üũ�Ͽ� ���ݰŸ� �ȿ� ���� ��� �������� ��ȯ
	const FMonsterInfo& Info = Monster->GetMonsterInfo();

	FVector MonsterLoc = Monster->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();

	MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	float Distance = FVector::Distance(MonsterLoc, TargetLoc);
	Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
	Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();
	
	if (Distance <= Info.AttackDistance)
	{
		Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_TargetTrace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
