// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VacancyFadeSubsystem.generated.h"


UENUM(BlueprintType)
enum class EVacancyFadeType : uint8
{
	None,
	FadeIn,
	FadeOut
};

USTRUCT(BlueprintType)
struct FVacancyFadeState
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	bool bIsFading = false;

	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	float FadeDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	float FadeElapsedTime = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	bool bIsFadingIn = false;

	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	float FadeAlpha = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	UUserWidget* WidgetToFade = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Fade")
	EVacancyFadeType FadeType = EVacancyFadeType::None;
};

UCLASS()
class THEAPARTMENT_API UVacancyFadeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UVacancyFadeSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFade(const FVacancyFadeState& NewFadeState);
	void InterruptFade();

	UFUNCTION(BlueprintCallable, Category = "Fade")
	bool IsFading() const;

	UFUNCTION(BlueprintCallable, Category = "Fade")
	float GetFadeAlpha() const;

private:
	
	FVacancyFadeState CurrentFadeState;

	void StartFadingOut(const FVacancyFadeState& NewFadeState, const float CurrentAlpha);
	static void UpdateFade(float DeltaTime);
};
