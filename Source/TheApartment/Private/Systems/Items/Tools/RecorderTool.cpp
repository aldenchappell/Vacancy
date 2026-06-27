// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/RecorderTool.h"


ARecorderTool::ARecorderTool()
{
	ToolData.ToolType = EToolType::Recorder;
	ToolData.ToolDisplayName = FName("Recorder");
	ToolData.ToolDescription = FText::FromString("A device used to record audio and video evidence.");
}

void ARecorderTool::BeginPlay()
{
	Super::BeginPlay();
	
}

