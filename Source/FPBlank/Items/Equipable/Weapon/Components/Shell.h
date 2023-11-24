#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shell.generated.h"

UCLASS()
class FPBLANK_API AShell : public AActor
{
	GENERATED_BODY()
	
public:	
	AShell();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UStaticMeshComponent* ShellMeshComponent;

protected:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
