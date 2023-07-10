// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MemoryComponent.generated.h"

class UPerceptionComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TCOM_API UMemoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMemoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RefreshMemory(float AddAware);
	
	void Forget();

	float MemoryTime = 5.f;

	float Awareness;

	UPROPERTY(BlueprintReadOnly)
	UPerceptionComponent* PerceptionComp;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* Target;
	
	FTimerHandle LifeTimer;
};
