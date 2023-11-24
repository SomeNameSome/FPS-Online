#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/DamageableInterface.h"
#include "Character_Base.generated.h"


UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterState : uint8
{
	None = 0 UMETA(Hidden),
	BattleMode = 1 UMETA(DisplayName = "BattleMode"),
	Idle = 2 UMETA(DisplayName = "Idle"),
	Walking = 4 UMETA(DisplayName = "Walking"),
	Runing = 8 UMETA(DisplayName = "Runing"),
	Firing = 16 UMETA(DisplayName = "Firing"),
	Aiming = 32 UMETA(DisplayName = "Aiming"),
	PlayingAnim = 64 UMETA(DisplayName = "PlayingAnim")
};



class AHUD_Base;
class AWeapon_Base;

UCLASS()
class FPBLANK_API ACharacter_Base : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(BlueprintReadOnly);
	AHUD_Base* CurrentHUD;

	UPROPERTY(EditDefaultsOnly);
	USoundBase* HitReactionSound;

	UPROPERTY();
	FTimerHandle Timer;

	UPROPERTY(Replicated, BlueprintReadOnly);
	FRotator ControlRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	FName ArmedSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TArray<FName> UnarmedSockets;

	UPROPERTY(Replicated);
	float Health;

	UPROPERTY(BlueprintReadOnly);
	float WalkSpeed;

	UPROPERTY(BlueprintReadOnly);
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "ECharacterState"), Replicated);
	uint8 CharacterState;


public:
	ACharacter_Base();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const FTransform GetPlayerViewPoint() const;
	virtual void Aiming() {};

	FORCEINLINE FTimerHandle& GetTimer() { return Timer; }

	FORCEINLINE AHUD_Base* GetCharacterHUD() const { return CurrentHUD; }

	void SetBattleIdleBasePose(UAnimSequenceBase* NewPose);

	virtual void Tick(float DeltaTime) override;

	virtual void ITakeDamage(float damage, FVector InLocation, FVector InNormal) override;
	bool IIsDying() override final { return Health <= 0.f; }
	virtual void IAddDeath() override {};

	virtual void AttachWeapon(AWeapon_Base* WeaponToAttach, FName Destination) {};

	UFUNCTION(BlueprintCallable)
	void SetControlRotation();

	UFUNCTION(BlueprintCallable)
	void UpdateAmmoInWeaponWidget(int32 currentAmmo);

	UFUNCTION(Client, Unreliable)
	void CauseDamage(bool isHeadshot);

	UFUNCTION(Client, Reliable)
	void TakeDamageClient(float NewHealth);

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		bool State(ECharacterState State) const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
		void AddState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
		void RemoveState(ECharacterState StateToRemove);

	virtual void Restart() override;

	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass);

	virtual void AddKill() {};


protected:
	virtual void BeginPlay() override;
	virtual void DestroyCharacter();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastCallback(FName FunctionCallbackName);

	UFUNCTION(Server, Reliable)
		void ServerCallback(FName FunctionCallbackName, bool Multicast = false);

	UFUNCTION(Client, Reliable)
		void ClientCallback(FName FunctionCallbackName);

private:
	void SetHealthBar(float NewHealth);

	UFUNCTION()
	void EnableRagdoll();

	UFUNCTION(Server, Reliable)
	void ServerRefreshState(uint8 currentCharacterState);
};
