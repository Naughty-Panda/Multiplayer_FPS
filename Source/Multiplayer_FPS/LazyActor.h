// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LazyActor.generated.h"

class USphereComponent;

UCLASS()
class MULTIPLAYER_FPS_API ALazyActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TSoftObjectPtr<UStaticMesh> LazyMeshRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USphereComponent* LoadRadius;

private:
	UPROPERTY()
	UStaticMeshComponent* LazyMesh;

public:
	// Sets default values for this actor's properties
	ALazyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadMesh();

protected:
	UFUNCTION()
	void OnEnteringLoadRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
