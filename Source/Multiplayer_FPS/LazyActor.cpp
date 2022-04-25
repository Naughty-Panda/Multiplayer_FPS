// Fill out your copyright notice in the Description page of Project Settings.

#include "LazyActor.h"
#include "BaseCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ALazyActor::ALazyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LazyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lazy Mesh"));
	LazyMesh->SetupAttachment(RootComponent);

	LoadRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Load Radius"));
	LoadRadius->SetupAttachment(LazyMesh);
	LoadRadius->OnComponentBeginOverlap.AddDynamic(this, &ALazyActor::OnEnteringLoadRadius);
}

// Called when the game starts or when spawned
void ALazyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALazyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALazyActor::LoadMesh()
{
	if (LazyMeshRef.IsPending())
	{
		// Should be used like this!

		// LazyMesh->SetStaticMesh(LazyMeshRef.LoadSynchronous());

		// But anyway...

		UE_LOG(LogActor, Warning, TEXT("LazyMeshRef is pending..."));

		if (UStaticMesh* NewMesh = LoadObject<UStaticMesh>(this, *LazyMeshRef.ToString()))
		{
			UE_LOG(LogActor, Warning, TEXT("Mesh loaded!"));
			LazyMesh->SetStaticMesh(NewMesh);
		}
	}
	else
	{
		UE_LOG(LogActor, Warning, TEXT("LazyMeshRef is not pending..."));

		LazyMesh->SetStaticMesh(LazyMeshRef.Get());
	}
}

void ALazyActor::OnEnteringLoadRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		UE_LOG(LogActor, Warning, TEXT("Overlaping with %s"), *Player->GetName());
		LoadMesh();
	}
}
