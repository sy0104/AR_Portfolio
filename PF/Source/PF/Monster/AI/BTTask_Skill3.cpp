// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Skill3.h"
#include "../MonsterAIController.h"
#include "../Monster.h"
#include "../MonsterAnimInstance.h"

#include "../../Player/PlayerCharacter.h"

UBTTask_Skill3::UBTTask_Skill3()
{
	NodeName = TEXT("Skill3");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Skill3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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


	// Target ������ Attack, ������ Idle
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
	Anim->ChangeAnim(EMonsterAnimType::Skill3);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Skill3::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Skill3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	if (Monster->GetMonsterPhase() != EMonsterPhase::Skill3)
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

	if (Cast<APlayerCharacter>(Target)->GetDeath())
	{
		Controller->StopMovement();
		Anim->ChangeAnim(EMonsterAnimType::Idle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	// ������ �������� �Ǵ��ϰ�
	// ������ ���� �� AttackDistance �ȿ� Ÿ���� �ִٸ� Ÿ���� �ִ� �������� ȸ�������ش�.
	if (Monster->GetSkillEnd())
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

		// ������ Ÿ�������� ����
		else
		{
			FVector Dir = TargetLoc - MonsterLoc;
			Dir.Z = 0.f;
			Dir.Normalize();

			Monster->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
		}

		Monster->SetSkillEnd(false);
	}
}

void UBTTask_Skill3::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
