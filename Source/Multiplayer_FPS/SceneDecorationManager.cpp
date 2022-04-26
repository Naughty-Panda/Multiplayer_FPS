// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneDecorationManager.h"
#include "SceneDecoration.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASceneDecorationManager::ASceneDecorationManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ASceneDecorationManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == ENetMode::NM_Client)
	{
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ASceneDecoration::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %i decorations"), FoundActors.Num());

		for (auto* Actor : FoundActors)
		{
			if (auto* Decoration = Cast<ASceneDecoration>(Actor))
			{
				//Decoration->LoadResourcesSynchronous();
				Decoration->LoadResourcesAsync();
			}
		}
	}
}
