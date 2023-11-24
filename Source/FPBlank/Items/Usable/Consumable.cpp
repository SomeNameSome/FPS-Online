#include "Consumable.h"
#include "Components/BoxComponent.h"


AConsumable::AConsumable()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	StaticMeshComponent->SetGenerateOverlapEvents(false);

	BoxCollisionOutside->SetupAttachment(StaticMeshComponent);
	BoxCollisionInside->SetupAttachment(StaticMeshComponent);
}

void AConsumable::IOnInteraction()
{
	Destroy();
}