#include "PerceptionComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MemoryComponent.h"

UPerceptionComponent::UPerceptionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(VisionLoop, this, &UPerceptionComponent::VisionSense, SightUpdateRate, true);

}

void UPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPerceptionComponent::VisionSense()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	TArray<AActor*> IgnoreList;
	IgnoreList.Add(GetOwner());
	TArray<FHitResult> Results;
	Types.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetComponentLocation(), GetForwardVector()+GetComponentLocation(),
		VisionRadius, Types, false, IgnoreList,EDrawDebugTrace::None, Results, true);
	for(auto Element : Results)
	{
		if(AActor* Found = Element.GetActor(); Found->ActorHasTag(SearchTag))
		{
			if(FVector Point = Element.ImpactPoint; (FVector::DotProduct(GetComponentLocation() - Point, GetForwardVector()*-1))/
				((GetComponentLocation() - Point).Length()*(GetForwardVector()*-1).Length()) >=
				UKismetMathLibrary::Cos(UKismetMathLibrary::DegreesToRadians(VisionAngle)))
			{
				if(FHitResult Hit; UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetComponentLocation(), Point, 5, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, IgnoreList, EDrawDebugTrace::None, Hit, true))
			{
				if(Hit.GetActor() == Found)
				{
						Memorize(Found, 0.25 * SightUpdateRate * SightCurve->GetFloatValue(FVector::Distance(GetComponentLocation(), Point)/VisionRadius));
				}
			}
			}
		}
	}
	//DrawDebugCone(GetWorld(), GetComponentLocation(), GetForwardVector(), VisionRadius, UKismetMathLibrary::DegreesToRadians(VisionAngle), UKismetMathLibrary::DegreesToRadians(VisionAngle), 40, FColor::Cyan, false, SightUpdateRate, 0, 5);
}

void UPerceptionComponent::TriggerSense(FVector TriggerLocation, float TriggerPower)
{
	if(UKismetMathLibrary::RandomBoolWithWeight(TriggerPower))
	{
		AddInterestingLocation(TriggerLocation);
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Magenta, "Triggered!");
	}
}

AActor* UPerceptionComponent::GetBestTarget()
{
	AActor* BestTarget = nullptr;
	float MinDist = 999999999;
	for(const auto MemComp : MemComps)
	{
		if(MemComp && MemComp->Target->GetDistanceTo(GetOwner()) <= MinDist && MemComp->Awareness >= 1)
		{
	
				BestTarget = MemComp->Target;
				MinDist = MemComp->Target->GetDistanceTo(GetOwner());
		}
	}
	return BestTarget;
}

void UPerceptionComponent::AddInterestingLocation(FVector Location)
{
	TArray<FVector> ToRemove;
	for(auto Vector : ImportantLocations)
	{
		if(FVector::Dist(Location, Vector) <= 500.f)
		{
			ToRemove.AddUnique(Vector);
		}
	}
	
	for(auto Vector : ToRemove)
	{
		ImportantLocations.Remove(Vector);
	}
	
	ImportantLocations.AddUnique(Location);
}

void UPerceptionComponent::RemoveImportantLocation()
{
	if(ImportantLocations.Num() > 0)
	{
		ImportantLocations.Pop();
	}
}

void UPerceptionComponent::Memorize(AActor* Target, float Awareness)
{
	bool bFound = false;
	for(const auto MemComp : MemComps)
	{
		if(MemComp && MemComp->Target == Target)
		{
				bFound = true;
				MemComp->RefreshMemory(Awareness);
			if(MemComp->Awareness >= 1)
			{
				ImportantLocations.Empty();
			}
		}
	}
	if(!bFound)
	{
		UMemoryComponent* Mem = NewObject<UMemoryComponent>(this, UMemoryComponent::StaticClass(), NAME_None, RF_Transient);
		Mem->MemoryTime = MemoryLifeTime;
		Mem->Target = Target;
		Mem->PerceptionComp = this;
		MemComps.Add(Mem);
		GetOwner()->AddInstanceComponent(Mem);
		Mem->RegisterComponent();
		Mem->RefreshMemory(0.f);
	}
}

