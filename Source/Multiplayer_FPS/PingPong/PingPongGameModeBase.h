// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameModeBase.generated.h"

class APingPongPlayerController;
class APlayerStart;
class APingPongBall;

/**
 * 
 */
UCLASS()
class MULTIPLAYER_FPS_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	APingPongPlayerController* Player1 = nullptr;

	UPROPERTY()
	APingPongPlayerController* Player2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player1Start = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player2Start = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APingPongBall> DefaultBallClass;

	UPROPERTY()
	APingPongBall* Ball = nullptr;

public:
	APingPongGameModeBase();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;

protected:
	UFUNCTION()
	void OnBallMissed_Player1(int32 BallCharge);

	UFUNCTION()
	void OnBallMissed_Player2(int32 BallCharge);
};
