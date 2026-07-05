// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "VacancyButtonBase.generated.h"

class USoundBase;
class UVacancyButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVacancyButtonEvent, UVacancyButtonBase*, Button);

UCLASS()
class THEAPARTMENT_API UVacancyButtonBase : public UButton
{
	GENERATED_BODY()

public:

	UVacancyButtonBase();

	virtual void SynchronizeProperties() override;
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "Vacancy Button")
	void SetButtonID(FName NewButtonID);

	UFUNCTION(BlueprintPure, Category = "Vacancy Button")
	FName GetButtonID() const { return ButtonID; }

	UFUNCTION(BlueprintCallable, Category = "Vacancy Button")
	void SetButtonInteractable(bool bInteractable);

	UFUNCTION(BlueprintPure, Category = "Vacancy Button")
	bool IsButtonInteractable() const;

	UFUNCTION(BlueprintCallable, Category = "Vacancy Button")
	void SetButtonSelected(bool bNewSelected);

	UFUNCTION(BlueprintPure, Category = "Vacancy Button")
	bool IsButtonSelected() const { return bIsSelected; }

	UPROPERTY(BlueprintAssignable, Category = "Vacancy Button")
	FOnVacancyButtonEvent OnVacancyButtonClicked;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy Button")
	FOnVacancyButtonEvent OnVacancyButtonHovered;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy Button")
	FOnVacancyButtonEvent OnVacancyButtonUnhovered;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy Button")
	FOnVacancyButtonEvent OnVacancyButtonPressed;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy Button")
	FOnVacancyButtonEvent OnVacancyButtonReleased;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy Button")
	void OnButtonClicked();
	virtual void OnButtonClicked_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy Button")
	void OnButtonHovered();
	virtual void OnButtonHovered_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy Button")
	void OnButtonUnhovered();
	virtual void OnButtonUnhovered_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy Button")
	void OnButtonPressed();
	virtual void OnButtonPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy Button")
	void OnButtonReleased();
	virtual void OnButtonReleased_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy Button")
	void OnButtonSelectedChanged(bool bSelected);
	virtual void OnButtonSelectedChanged_Implementation(bool bSelected);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy Button")
	FName ButtonID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy Button")
	bool bStartDisabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy Button")
	bool bDisableAfterClick = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy Button")
	bool bLogButtonEvents = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy Button|Audio")
	TObjectPtr<USoundBase> HoverSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy Button|Audio")
	TObjectPtr<USoundBase> ClickSound = nullptr;

private:

	void BindButtonEvents();
	void UnbindButtonEvents();

	UFUNCTION()
	void HandleButtonClicked();

	UFUNCTION()
	void HandleButtonHovered();

	UFUNCTION()
	void HandleButtonUnhovered();

	UFUNCTION()
	void HandleButtonPressed();

	UFUNCTION()
	void HandleButtonReleased();

	void PlayButtonSound(USoundBase* Sound) const;

	bool bEventsBound = false;
	bool bIsSelected = false;
};