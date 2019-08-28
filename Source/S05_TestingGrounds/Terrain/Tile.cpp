// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/NavigationSystemBase.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000, 0, 0);

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Pool == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s EndPlay] Pool is null."), *GetName());
		return;
	}
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s EndPlay] NavMeshBoundsVolume is null."), *GetName());
		return;
	}
	Pool->Return(NavMeshBoundsVolume);
	Super::EndPlay(EndPlayReason);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	if (ToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s PlaceActors] ToSpawn parameter not specified in BP_Tile event graph."), *GetName());
	}
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool bFound = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (bFound)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

template<class T>
void ATile::PlaceActor(TSubclassOf<T> ToSpawn, const FSpawnPosition SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	T* Spawned = GetWorld()->SpawnActor<T>(ToSpawn, SpawnPosition.Location, Rotation);
	if (Spawned == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s PlaceActor<T>] SpawnActor returns null."), *GetName());
		return;
	}
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);
	if (Spawned == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s PlaceActor<APawn>] SpawnActor returns null."), *GetName());
		return;
	}
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SpawnDefaultController();
	Spawned->Tags.Add(FName("Enemy"));
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);
	const size_t MAX_ATTEMPTS = 10;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}

bool ATile::CanSpawnAtLocation(FVector LocalLocation, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(LocalLocation);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation, 
		GlobalLocation,
		FQuat::Identity, // Rotation
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	int32 Segments = 20;
	bool bPersistentLines = true;
	float LifeTime = 100.f;
	float HalfHeight = 0.f;
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	// DrawDebugSphere(GetWorld(), GlobalLocation, Radius, Segments, ResultColor, bPersistentLines, LifeTime);
	// DrawDebugCapsule(GetWorld(), GlobalLocation, HalfHeight, Radius, FQuat::Identity, ResultColor, bPersistentLines, LifeTime);
	return !HasHit;
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPool(UActorPool * PoolToSet)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] SetPool with %s"), *(this->GetName()), *(PoolToSet->GetName()));
	Pool = PoolToSet;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	FNavigationSystem::Build(*GetWorld());
}
