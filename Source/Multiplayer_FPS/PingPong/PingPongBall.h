// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPongBall.generated.h"

class USphereComponent;

UCLASS()
class MULTIPLAYER_FPS_API APingPongBall : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	USphereComponent* BallCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float MoveSpeed = 100.f;

	UPROPERTY(ReplicatedUsing=BallCharge_OnRepNotify)
	int32 BallCharge = 1;

	UPROPERTY(Replicated)
	uint8 bIsMoving:1;

public:
	// Sets default values for this actor's properties
	APingPongBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void StartMove();

	UFUNCTION(BlueprintCallable)
	void StopMove();

	// Executes only on client
	UFUNCTION(BlueprintNativeEvent)
	void BallCharge_OnRepNotify();

	UFUNCTION(BlueprintPure)
	int32 GetBallCharge() const { return BallCharge; }
	void ResetBallCharge() { BallCharge = 1; }

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartMove();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopMove();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_HitEffect();
};
