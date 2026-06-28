// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/Items/Tools/BaseTool.h"
#include "BasePlayerProgressionComponent.generated.h"

struct FPlayerToolAttachmentStateInfo;
class ABaseTool;

USTRUCT(BlueprintType)
struct FPlayerProgressionComponentToolState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Progression Component")
	bool bIsToolEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Progression Component")
	bool bStartUnlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Progression Component")
	FName ComponentSocketAttachName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Progression Component")
	TSubclassOf<ABaseTool> ComponentToolClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Progression Component")
	TObjectPtr<ABaseTool> ComponentToolInstance = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerProgressionToolStateChanged, const FPlayerProgressionComponentToolState&, NewToolState);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UBasePlayerProgressionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBasePlayerProgressionComponent();

	bool ToggleComponentToolState(const bool bEnable);
	
	void UnlockProgressionComponentTool();

	UPROPERTY(BlueprintAssignable, Category="Player Progression Component")
	FOnPlayerProgressionToolStateChanged OnPlayerProgressionToolStateChanged;

	UFUNCTION(Category = "Player Progression Component")
	bool EquipProgressionComponentTool();
protected:
	virtual void BeginPlay() override;

	virtual bool Internal_ToggleComponentToolState(const bool bEnable);

	UFUNCTION(BlueprintNativeEvent, Category="Player Progression Component")
	void OnComponentToolStateUnlocked();

	UFUNCTION(BlueprintNativeEvent, Category="Player Progression Component")
	void OnComponentToolStateInitialized();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Progression Component")
	FPlayerProgressionComponentToolState ComponentToolState;

private:

	bool AttachToolToDesiredSocket();
	bool DetachToolFromSocket(const AActor* ToolActor) const;
	void HandleToolAlreadyEquipped(const bool bForceSwap = false) const;

	void ConstructNewProgressionInfo(FPlayerToolAttachmentStateInfo& NewToolState, ABaseTool*& NewToolInstance) const;

	void UnlockState();
	static bool DebugState();
};
