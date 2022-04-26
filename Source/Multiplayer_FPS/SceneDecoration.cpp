// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneDecoration.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASceneDecoration::ASceneDecoration()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	DecorationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Decoration Mesh"));
	DecorationMesh->SetupAttachment(RootComponent);
	DecorationMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ASceneDecoration::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Synchronous loading
 */
void ASceneDecoration::LoadResourcesSynchronous()
{
	DecorationMesh->SetStaticMesh(DecorationMeshRef.IsPending() ? DecorationMeshRef.LoadSynchronous() : DecorationMeshRef.Get());
}

/**
 * Asynchronous loading
 */
void ASceneDecoration::LoadResourcesAsync()
{
	FStreamableDelegate LoadMeshDelegate;
	LoadMeshDelegate.BindUObject(this, &ASceneDecoration::OnResourcesLoaded);

	UAssetManager& AssetManager = UAssetManager::Get();
	FStreamableManager& StreamableManager = AssetManager.GetStreamableManager();

	AssetHandle = StreamableManager.RequestAsyncLoad(DecorationMeshRef.ToSoftObjectPath(), LoadMeshDelegate);
}

// Called when asset is loaded
void ASceneDecoration::OnResourcesLoaded()
{
	if (UStaticMesh* LoadedMesh = Cast<UStaticMesh>(AssetHandle.Get()->GetLoadedAsset()))
	{
		DecorationMesh->SetStaticMesh(LoadedMesh);
	}
}
