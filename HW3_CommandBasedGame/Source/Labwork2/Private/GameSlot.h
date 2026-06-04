// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameSlot.generated.h"

class AUnitBase;
class ATBPlayerController;

USTRUCT(BlueprintType)
struct FSGridPosition
{
	GENERATED_USTRUCT_BODY();

	FSGridPosition() { }
	FSGridPosition(int col, int row) : Col(col), Row(row) { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Col;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Row;
};

UENUM(BlueprintType)
enum EGridState
{
	GS_Default,
	GS_Highlighted,
	GS_Offensive,
	GS_Supportive,
};

UCLASS()
class AGameSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameSlot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Plane;

	UPROPERTY()
	UMaterialInterface* DefaultMaterial;

	UPROPERTY()
	UMaterialInterface* HighlightMaterial;

	UPROPERTY()
	UMaterialInterface* OffensiveMaterial;

	UPROPERTY()
	UMaterialInterface* SupportiveMaterial;

	UPROPERTY(BlueprintReadWrite)
	FSGridPosition GridPosition;

	EGridState GridState;

	UPROPERTY(VisibleAnywhere)
	AUnitBase* Unit;

	void SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass);

	UFUNCTION()
	void SetState(EGridState NewState);

	UFUNCTION()
	void OnGridClicked(AActor* TouchedActor, FKey ButtonPressed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
