// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/RecorderTool.h"


// Sets default values
ARecorderTool::ARecorderTool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARecorderTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARecorderTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

