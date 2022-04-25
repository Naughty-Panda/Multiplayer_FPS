// Fill out your copyright notice in the Description page of Project Settings.

#include "PingPongPlatform.h"

#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

// Sets default values
APingPongPlatform::APingPongPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Platform Collision"));
	SetRootComponent(PlatformCollision);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	PlatformMesh->SetIsReplicated(true);

	bReplicates = true;
	//SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APingPongPlatform::BeginPlay()
{
	Super::BeginPlay();

	LoadPlatformMesh();
}

// Called every frame
void APingPongPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APingPongPlatform::LoadPlatformMesh()
{
	FStreamableDelegate LoadMeshDelegate;
	LoadMeshDelegate.BindUObject(this, &APingPongPlatform::OnPlatformMeshLoaded);

	UAssetManager& AssetManager = UAssetManager::Get();
	FStreamableManager& StreamableManager = AssetManager.GetStreamableManager();

	AssetHandle = StreamableManager.RequestAsyncLoad(PlatformMeshRef.ToSoftObjectPath(), LoadMeshDelegate);
}

// Called when asset is loaded
void APingPongPlatform::OnPlatformMeshLoaded()
{
	if (UStaticMesh* LoadedMesh = Cast<UStaticMesh>(AssetHandle.Get()->GetLoadedAsset()))
	{
		PlatformMesh->SetStaticMesh(LoadedMesh);
	}
}

void APingPongPlatform::Server_MoveRight_Implementation(float AxisValue)
{
	const FVector CurrentLocation = GetActorLocation();
	const FVector NextLocation = CurrentLocation + GetActorRightVector() * MoveSpeed * AxisValue;

	if (!SetActorLocation(NextLocation, true))
	{
		SetActorLocation(CurrentLocation);
	}
}

bool APingPongPlatform::Server_MoveRight_Validate(float AxisValue)
{
	return AxisValue != 0.f;
}
