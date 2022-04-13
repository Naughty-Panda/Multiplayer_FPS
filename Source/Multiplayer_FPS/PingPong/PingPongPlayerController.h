// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"

class APingPongPlatform;
class APingPongGoal;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBallMissedDelegate, int32);

/**
 * 
 */
UCLASS()
class MULTIPLAYER_FPS_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FTransform StartTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APingPongPlatform> PlatformClass;

	UPROPERTY()
	APingPongPlatform* Platform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APingPongGoal> GoalClass;

	UPROPERTY()
	APingPongGoal* Goal;

private:
	UPROPERTY(Replicated)
	int32 PlayerScore = 0;

public:
	/** Missed ball delegate */
	FOnBallMissedDelegate OnBallMissed;

public:
	APingPongPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SetStartTransform(const FTransform& NewTransform);

	UFUNCTION(Server, Reliable, WithValidation)
	void Initialize();

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlatform(TSubclassOf<APingPongPlatform> NewPlatformClass);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnGoal(TSubclassOf<APingPongGoal> NewGoalClass);

	virtual void SetupInputComponent() override;

	void AddPlayerScore(int32 Value) { PlayerScore += Value; }

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerScore() const { return PlayerScore; }

protected:
	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable)
	void OnGoalBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
