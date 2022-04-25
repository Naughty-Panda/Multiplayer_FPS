// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPongPlatform.generated.h"

class UBoxComponent;
struct FStreamableHandle;

UCLASS()
class MULTIPLAYER_FPS_API APingPongPlatform : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* PlatformCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> PlatformMeshRef;

	TSharedPtr<FStreamableHandle> AssetHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100.f;

public:
	// Sets default values for this actor's properties
	APingPongPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void LoadPlatformMesh();
	void OnPlatformMeshLoaded();
};
