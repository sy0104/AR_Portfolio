// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Monster AI Controller
	AMonsterAIController* Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	
	if (!IsValid(Controller))
		return EBTNodeResult::Failed;

	// Monster
	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	// Target 있으면 Attack, 없으면 Idle
	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();
	AActor* Target = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	// Target X
	if (!IsValid(Target))
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);

		return EBTNodeResult::Failed;
	}

	// Target O
	Anim->ChangeAnim(EMonsterAnimType::Attack);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (Monster->GetMonsterPhase() != EMonsterPhase::NormalAttack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UMonsterAnimInstance* Anim = Monster->GetMonsterAnimInst();

	ACharacter* Target = Cast<ACharacter>(Controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Monster->GetAttackDelay())
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}


	if (!IsValid(Target))
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	// 공격이 끝났는지 판단하고
	// 공격이 끝난 후 AttackDistance 안에 타겟이 있다면 타겟이 있는 방향으로 회전시켜준다.
	if (Monster->GetAttackEnd())
	{
		const FMonsterInfo& Info = Monster->GetMonsterInfo();

		FVector MonsterLoc = Monster->GetActorLocation();
		FVector TargetLoc = Target->GetActorLocation();

		MonsterLoc = MonsterLoc - FVector(0.f, 0.f, Monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		TargetLoc = TargetLoc - FVector(0.f, 0.f, Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		float Distance = FVector::Distance(MonsterLoc, TargetLoc);

		Distance -= Monster->GetCapsuleComponent()->GetScaledCapsuleRadius();
		Distance -= Target->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (Distance > Info.AttackDistance)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

		// 방향을 타겟쪽으로 변경
		else
		{
			FVector Dir = TargetLoc - MonsterLoc;
			Dir.Z = 0.f;
			Dir.Normalize();

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}

		Monster->SetAttackEnd(false);
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
