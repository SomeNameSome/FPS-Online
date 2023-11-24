#include "Shell.h"

// Sets default values
AShell::AShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	ShellMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ShellMeshComponent"));
	RootComponent = ShellMeshComponent;
	ShellMeshComponent->bReplicatePhysicsToAutonomousProxy = false;
	ShellMeshComponent->SetSimulatePhysics(true);
	ShellMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ShellMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ShellMeshComponent->SetGenerateOverlapEvents(false);
	ShellMeshComponent->SetNotifyRigidBodyCollision(true);

	FScriptDelegate delegate;
	delegate.BindUFunction(this, FName("OnComponentHit"));
	ShellMeshComponent->OnComponentHit.Add(delegate);

	this->InitialLifeSpan = 10.f;
}

// Called when the game starts or when spawned
void AShell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShell::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ShellMeshComponent->OnComponentHit.Clear();
}

