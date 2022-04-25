// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SceneDecoration.generated.h"

UCLASS()
class MULTIPLAYER_FPS_API ASceneDecoration : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TSoftObjectPtr<UStaticMesh> DecorationMeshRef;

private:
	UPROPERTY()
	UStaticMeshComponent* DecorationMesh;

public:
	// Sets default values for this actor's properties
	ASceneDecoration();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void LoadResources();

private:
	UFUNCTION(Client, Reliable)
	void Client_LoadResources();

	UFUNCTION(Server, Reliable)
	void Server_LoadResources();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LoadResources();
};
