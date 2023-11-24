#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

UCLASS()
class FPBLANK_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagazine();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UStaticMeshComponent* StaticMeshComponent;

public:
	UFUNCTION()
	void SetEnablePhysics(bool simulatePhysics);
};
