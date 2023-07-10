#include "MemoryComponent.h"
#include "PerceptionComponent.h"

UMemoryComponent::UMemoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMemoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UMemoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString a = FString::SanitizeFloat(Awareness) +" | " + Target->GetName();
	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Cyan, a);
}

void UMemoryComponent::RefreshMemory(float AddAware)
{
	Awareness += AddAware;
	GetWorld()->GetTimerManager().ClearTimer(LifeTimer);
	
	GetWorld()->GetTimerManager().SetTimer(LifeTimer, this, &UMemoryComponent::Forget, MemoryTime, false);
}

void UMemoryComponent::Forget()
{
	if(Target)
	{
		PerceptionComp->AddInterestingLocation(Target->GetActorLocation());
		PerceptionComp->MemComps.Remove(this);
	}
	
	DestroyComponent();
}

