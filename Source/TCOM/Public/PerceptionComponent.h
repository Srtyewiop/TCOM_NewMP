// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PerceptionComponent.generated.h"

class UMemoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TCOM_API UPerceptionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	FTimerHandle VisionLoop;
	UPerceptionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void VisionSense();
	void TriggerSense(FVector TriggerLocation, float TriggerPower);

	UFUNCTION(BlueprintPure)
	AActor* GetBestTarget();
	void AddInterestingLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	void RemoveImportantLocation();

	void Memorize(AActor* Target, float Awareness);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VisionRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VisionAngle = 45.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SearchTag = FName(TEXT("Player"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* SightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SightUpdateRate = 0.1f;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> ImportantLocations;

	UPROPERTY(BlueprintReadOnly)
	TArray<UMemoryComponent*> MemComps;

	UPROPERTY(EditAnywhere)
	float MemoryLifeTime = 5.f;
};
