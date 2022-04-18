// Fill out your copyright notice in the Description page of Project Settings.

#include "PingPongGameModeBase.h"

#include "PingPongBall.h"
#include "PingPongPawn.h"
#include "PingPongPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

APingPongGameModeBase::APingPongGameModeBase()
{
	DefaultPawnClass = APingPongPawn::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
}

void APingPongGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APingPongGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* CurrentWorld = GetWorld();

	if (!CurrentWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode error: No current UWorld!"));
		return;
	}

	/*if (Player1Start && Player2Start)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode error: Player Start already exist!"));
		return;
	}*/

	if (!Player1Start || !Player2Start)
	{
		TArray<AActor*> FoundPlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(CurrentWorld, APlayerStart::StaticClass(), FoundPlayerStarts);

		if (FoundPlayerStarts.Num() > 1)
		{
			Player1Start = Cast<APlayerStart>(FoundPlayerStarts[0]);
			Player2Start = Cast<APlayerStart>(FoundPlayerStarts[1]);
		}
	}

	APingPongPlayerController* CurrentPlayer = nullptr;
	APlayerStart* StartPosition = nullptr;

	APingPongPlayerController* NewPlayerController = Cast<APingPongPlayerController>(NewPlayer);
	if (!NewPlayerController)
	{
		return;
	}

	if (!Player1)
	{
		Player1 = NewPlayerController;
		CurrentPlayer = Player1;
		StartPosition = Player1Start;
		Player1->OnBallMissed.AddUObject(this, &APingPongGameModeBase::OnBallMissed_Player1);
		UE_LOG(LogTemp, Warning, TEXT("PingPong GameMode: Player 1 initialized!"));
	}
	else if (!Player2)
	{
		Player2 = NewPlayerController;
		CurrentPlayer = Player2;
		StartPosition = Player2Start;
		Player2->OnBallMissed.AddUObject(this, &APingPongGameModeBase::OnBallMissed_Player2);
		UE_LOG(LogTemp, Warning, TEXT("PingPong GameMode: Player 2 initialized!"));
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT("PingPong GameMode: There are two players in the game already! No new connections allowed!"));
		return;
	}

	APingPongPawn* NewPawn = Cast<APingPongPawn>(NewPlayer->GetPawn());
	if (!NewPawn)
	{
		NewPawn = CurrentWorld->SpawnActor<APingPongPawn>(DefaultPawnClass);
	}

	if (StartPosition && NewPawn)
	{
		NewPawn->SetActorLocation(StartPosition->GetActorLocation());
		NewPawn->SetActorRotation(StartPosition->GetActorRotation());
		NewPlayer->SetPawn(NewPawn);

		CurrentPlayer->SetStartTransform(StartPosition->GetActorTransform());
		CurrentPlayer->Initialize();
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("GameMode error: No StartPosition in PingPong GameMode!"));
}

void APingPongGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (ensure(DefaultBallClass))
	{
		Ball = GetWorld()->SpawnActor<APingPongBall>(DefaultBallClass);

		if (Ball)
		{
			Ball->SetActorLocation(FVector(0, 0, 140));
			Ball->SetActorRotation(FRotator(0, 55, 0));
			Ball->StartMove();
		}
	}
	else
	{
		UE_LOG(LogGameMode, Error, TEXT("Error: Incorrect Default Ball Class!"));
		ReturnToMainMenuHost();
	}
}

void APingPongGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	const APingPongPlayerController* ExitingController = Cast<APingPongPlayerController>(Exiting);

	if (ExitingController == Player1)
	{
		Player1 = nullptr;

		if (Player2)
		{
			Player2->OnOtherPlayerDisconnected();
		}
	}
	else if (ExitingController == Player2)
	{
		Player2 = nullptr;

		if (Player1)
		{
			Player1->OnOtherPlayerDisconnected();
		}
	}
	else
	{
		return;
	}

	if (Ball)
	{
		Ball->StopMove();
		Ball->Reset();
	}

	UE_LOG(LogGameMode, Warning, TEXT("Controller %s removed!"), *ExitingController->GetName());
}

void APingPongGameModeBase::OnBallMissed_Player1(int32 BallCharge)
{
	UE_LOG(LogTemp, Warning, TEXT("Player 1 missed the ball!"));

	if (Player2)
	{
		Player2->AddPlayerScore(BallCharge);
		Ball->ResetBallCharge();
	}
}

void APingPongGameModeBase::OnBallMissed_Player2(int32 BallCharge)
{
	UE_LOG(LogTemp, Warning, TEXT("Player 2 missed the ball!"));

	if (Player1)
	{
		Player1->AddPlayerScore(BallCharge);
		Ball->ResetBallCharge();
	}
}
