// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S05_TestingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

class UActorPool;

/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API AInfiniteTerrainGameMode : public AS05_TestingGroundsGameMode
{
	GENERATED_BODY()

public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bounds Pool")
		UActorPool* NavMeshBoundVolumePool;

protected:


private:
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);
};
