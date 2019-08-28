// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AIController.h"
#include "PatrolRoute.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the patrol route
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) return EBTNodeResult::Failed;
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The guard is missing the patrol points"));
		return EBTNodeResult::Failed;
	}

	// Set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto IndexLocal = BlackboardComp->GetValueAsInt(Index.SelectedKeyName);
	BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName, PatrolPoints[IndexLocal]);

	// Cycle the index
	auto NextIndex = (IndexLocal + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(Index.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
