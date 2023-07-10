#include "NoiseComponent.h"

#include "PerceptionComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UNoiseComponent::UNoiseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNoiseComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UNoiseComponent::MakeNoise(FVector Location, float Power, float Radius)
{

	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	TArray<AActor*> IgnoreList;
	TArray<AActor*> ActorList;
	TArray<FHitResult> Results;
	IgnoreList.Add(GetOwner());
	Types.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Location, Location + FVector(0, 0, 10),
		Radius, Types, false, IgnoreList,EDrawDebugTrace::ForOneFrame, Results, true);
	for(FHitResult Result : Results)
	{
		ActorList.AddUnique(Result.GetActor());
	}

	for(AActor* Actor : ActorList)
	{
		if(UPerceptionComponent* PercComp= Actor->GetComponentByClass<UPerceptionComponent>())
		{
			PercComp->TriggerSense(Location, Power);
		}
	}
	
	
}

