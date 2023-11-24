#include "Magazine.h"

// Sets default values
AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MagazineMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->bReplicatePhysicsToAutonomousProxy = false;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMeshComponent->SetGenerateOverlapEvents(false);

	this->InitialLifeSpan = 15.f;
}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagazine::SetEnablePhysics(bool simulatePhysics)
{
	StaticMeshComponent->SetSimulatePhysics(simulatePhysics);
}

// Called every frame
void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

