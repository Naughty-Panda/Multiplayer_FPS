// Fill out your copyright notice in the Description page of Project Settings.

#include "PingPongPlayerController.h"

#include "PingPongBall.h"
#include "PingPongGoal.h"
#include "PingPongPlatform.h"
#include "Net/UnrealNetwork.h"

APingPongPlayerController::APingPongPlayerController()
{
	//SetReplicates(true);
	bReplicates = true;
}

void APingPongPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APingPongPlayerController, PlayerScore, COND_OwnerOnly);
}

void APingPongPlayerController::SetStartTransform(const FTransform& NewTransform)
{
	StartTransform = NewTransform;
}

void APingPongPlayerController::Initialize_Implementation()
{
	if (Platform)
	{
		Platform->Destroy();
	}

	if (Goal)
	{
		Goal->Destroy();
	}

	SpawnPlatform(PlatformClass);
	SpawnGoal(GoalClass);
}

bool APingPongPlayerController::Initialize_Validate()
{
	return true;
}

void APingPongPlayerController::SpawnPlatform_Implementation(TSubclassOf<APingPongPlatform> NewPlatformClass)
{
	Platform = GetWorld()->SpawnActor<APingPongPlatform>(PlatformClass);

	if (Platform)
	{
		Platform->SetActorLocation(StartTransform.GetLocation());
		Platform->SetActorRotation(StartTransform.GetRotation());
	}
}

bool APingPongPlayerController::SpawnPlatform_Validate(TSubclassOf<APingPongPlatform> NewPlatformClass)
{
	return PlatformClass != nullptr;
}

void APingPongPlayerController::SpawnGoal_Implementation(TSubclassOf<APingPongGoal> NewGoalClass)
{
	Goal = GetWorld()->SpawnActor<APingPongGoal>(GoalClass);

	if (Goal)
	{
		Goal->SetActorLocation(StartTransform.GetLocation());
		Goal->SetActorRotation(StartTransform.GetRotation());
		Goal->OnActorBeginOverlap.AddDynamic(this, &APingPongPlayerController::OnGoalBeginOverlap);
	}
}

bool APingPongPlayerController::SpawnGoal_Validate(TSubclassOf<APingPongGoal> NewGoalClass)
{
	return GoalClass != nullptr;
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move", this, &APingPongPlayerController::MoveRight);
}

void APingPongPlayerController::MoveRight(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("MoveRight()"));
	Server_MoveRight(AxisValue);
}

void APingPongPlayerController::Server_MoveRight_Implementation(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_MoveRight()"));
	Platform->Server_MoveRight(AxisValue);
}

bool APingPongPlayerController::Server_MoveRight_Validate(float AxisValue)
{
	return Platform != nullptr;
}

void APingPongPlayerController::OnGoalBeginOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	if (APingPongBall* Ball = Cast<APingPongBall>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, TEXT("Controller: Goal registered!"));
		OnBallMissed.Execute();
	}
}
