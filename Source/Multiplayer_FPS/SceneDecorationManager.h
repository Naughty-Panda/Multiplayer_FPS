// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SceneDecorationManager.generated.h"

UCLASS()
class MULTIPLAYER_FPS_API ASceneDecorationManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneDecorationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
