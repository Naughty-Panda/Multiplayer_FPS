// Fill out your copyright notice in the Description page of Project Settings.

#include "PingPongGoal.h"
#include "Components/BoxComponent.h"

// Sets default values
APingPongGoal::APingPongGoal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	GoalCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Goal Collision"));
	GoalCollision->SetupAttachment(RootComponent);

	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	GoalMesh->SetupAttachment(GoalCollision);

	bReplicates = true;
}

// Called when the game starts or when spawned
void APingPongGoal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APingPongGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
