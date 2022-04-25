// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneDecoration.h"
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
}

// Called when the game starts or when spawned
void ASceneDecoration::BeginPlay()
{
	Super::BeginPlay();
}

void ASceneDecoration::LoadResources()
{
	Multicast_LoadResources();
}

void ASceneDecoration::Client_LoadResources_Implementation()
{
	DecorationMesh->SetStaticMesh(DecorationMeshRef.IsPending() ? DecorationMeshRef.LoadSynchronous() : DecorationMeshRef.Get());
}

void ASceneDecoration::Multicast_LoadResources_Implementation()
{
	DecorationMesh->SetStaticMesh(DecorationMeshRef.IsPending() ? DecorationMeshRef.LoadSynchronous() : DecorationMeshRef.Get());
}

void ASceneDecoration::Server_LoadResources_Implementation()
{
	DecorationMesh->SetStaticMesh(DecorationMeshRef.IsPending() ? DecorationMeshRef.LoadSynchronous() : DecorationMeshRef.Get());
}
