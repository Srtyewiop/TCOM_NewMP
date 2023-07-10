#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoiseComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TCOM_API UNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNoiseComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void MakeNoise(FVector Location, float Power, float Radius);

		
};
