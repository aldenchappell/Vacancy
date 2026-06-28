// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Subsystems/Gameplay/VacancyFadeSubsystem.h"

#include "Blueprint/UserWidget.h"

UVacancyFadeSubsystem::UVacancyFadeSubsystem()
{
	
}

void UVacancyFadeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVacancyFadeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UVacancyFadeSubsystem::StartFade(const FVacancyFadeState& NewFadeState)
{
	if (IsFading())
	{
		InterruptFade();
	}

	if (!NewFadeState.WidgetToFade)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFade called with null WidgetToFade."));
		return;
	}

	if (NewFadeState.FadeDuration <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFade called with non-positive FadeDuration."));
		return;
	}

	if (NewFadeState.FadeType == EVacancyFadeType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFade called with FadeType set to None."));
		return;
	}

	CurrentFadeState = NewFadeState;
	
	CurrentFadeState.WidgetToFade->SetVisibility(ESlateVisibility::Visible);
	CurrentFadeState.WidgetToFade->SetRenderOpacity(CurrentFadeState.FadeType == EVacancyFadeType::FadeIn ? 0.f : 1.f);
	CurrentFadeState.FadeElapsedTime = 0.f;

	StartFadingOut(CurrentFadeState, CurrentFadeState.FadeAlpha);
}

void UVacancyFadeSubsystem::InterruptFade()
{
}

bool UVacancyFadeSubsystem::IsFading() const
{
	return false;
}

float UVacancyFadeSubsystem::GetFadeAlpha() const
{
	return 0.f;
}

void UVacancyFadeSubsystem::StartFadingOut(const FVacancyFadeState& NewFadeState, const float CurrentAlpha)
{
	if (!NewFadeState.WidgetToFade)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFadingOut called with null WidgetToFade."));
		return;
	}

	if (const bool bIsFadingIn = NewFadeState.FadeType == EVacancyFadeType::FadeIn; bIsFadingIn && CurrentAlpha >= 1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFadingOut called for FadeIn but CurrentAlpha is already 1."));
		return;
	}
	else if (!bIsFadingIn && CurrentAlpha <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartFadingOut called for FadeOut but CurrentAlpha is already 0."));
		return;
	}

	//NewFadeState.FadeElapsedTime += CurrentAlpha * NewFadeState.FadeDuration;
	
}

void UVacancyFadeSubsystem::UpdateFade(float DeltaTime)
{
	
}
