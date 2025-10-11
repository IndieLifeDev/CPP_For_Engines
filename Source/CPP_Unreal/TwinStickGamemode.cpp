// Fill out your copyright notice in the Description page of Project Settings.


#include "TwinStickGamemode.h"

#include "MyCheckpoint.h"

ATwinStickGamemode::ATwinStickGamemode()
{
	
}

void ATwinStickGamemode::RegisterCheckpoint(AMyCheckpoint* checkpointActor)
{
	checkpointActor->OnEndGameDelegate.AddDynamic(this, &ATwinStickGamemode::FinishGame);
}

void ATwinStickGamemode::FinishGame(FString EndMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *EndMessage);
}
