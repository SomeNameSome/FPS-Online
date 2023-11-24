#include "Character_Base.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "HUD_Base.h"
#include "PC_Base.h"
#include "Weapon_Base.h"
#include "../Primary/AnimInstancePrimary.h"

DEFINE_LOG_CATEGORY_STATIC(CharacterLog, All, All);

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	Health = 100.f;
	WalkSpeed = 460.f;
	RunSpeed = 800.f;

	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -98.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(FName("CharacterMesh"));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	ArmedSocket = "hand_r_wsock";
	UnarmedSockets.Add("clavicle_r_wsock");
	UnarmedSockets.Add("clavicle_l_wsock");
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacter_Base, CharacterState);
	DOREPLIFETIME_CONDITION(ACharacter_Base, ControlRotation, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ACharacter_Base, Health, COND_AutonomousOnly);
}

void ACharacter_Base::SetControlRotation()
{
	if (Controller)
	{
		if (HasAuthority() || IsLocallyControlled())
		{
			ControlRotation = Controller->GetControlRotation();
		}
	}
}

void ACharacter_Base::CauseDamage_Implementation(bool isHeadshot)
{
	CurrentHUD->PlayHitMarker(isHeadshot);
}

void ACharacter_Base::ITakeDamage(float damage, FVector InLocation, FVector InNormal)
{
	UE_LOG(CharacterLog, Warning, TEXT("TakeDamage"));
	Health += damage;
	if (Health <= 0.f)
	{
		DestroyCharacter();
	}
}

void ACharacter_Base::TakeDamageClient_Implementation(float NewHealth)
{
	SetHealthBar(NewHealth);
	UGameplayStatics::PlaySound2D(GetWorld(), HitReactionSound);
}

void ACharacter_Base::DestroyCharacter()
{
	MulticastCallback(FName("EnableRagdoll"));
}

void ACharacter_Base::EnableRagdoll()
{
	if (IsLocallyControlled())
		DisableInput(nullptr);

	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ACharacter_Base::SetHealthBar(float NewHealth)
{
	if (NewHealth < 0.f)
		NewHealth = 0.f;

	if (CurrentHUD)
		CurrentHUD->SetHealthBarValue(NewHealth);
	else
		UE_LOG(CharacterLog, Warning, TEXT("!CurrentHUD"));
}


void ACharacter_Base::UpdateAmmoInWeaponWidget(int32 currentAmmo)
{
	CurrentHUD->SetAmmoInMagazine(currentAmmo);
}

void ACharacter_Base::ClientCallback_Implementation(FName FunctionCallbackName)
{
	FSimpleDelegate ptrFunc;
	ptrFunc.BindUFunction(this, FunctionCallbackName);
	if (ptrFunc.IsBound())
	{
		UE_LOG(CharacterLog, Warning, TEXT("ptrFunc.IsBound()"));
		ptrFunc.Execute();
	}
	else
	{
		UE_LOG(CharacterLog, Warning, TEXT("!ptrFunc.IsBound()"));
	}
}

void ACharacter_Base::ServerCallback_Implementation(FName FunctionCallbackName, bool Multicast)
{
	UE_LOG(CharacterLog, Warning, TEXT("ServerCallback_Implementation()"));
	if (Multicast)
	{
		MulticastCallback(FunctionCallbackName);
	}
	else
	{
		FSimpleDelegate ptrFunc;
		ptrFunc.BindUFunction(this, FunctionCallbackName);
		if (ptrFunc.IsBound())
		{
			UE_LOG(CharacterLog, Warning, TEXT("ptrFunc.IsBound()"));
			ptrFunc.Execute();
		}
		else
		{
			UE_LOG(CharacterLog, Warning, TEXT("!ptrFunc.IsBound()"));
		}
	}
}

void ACharacter_Base::MulticastCallback_Implementation(FName FunctionCallbackName)
{
	FSimpleDelegate ptrFunc;
	ptrFunc.BindUFunction(this, FunctionCallbackName);
	if (ptrFunc.IsBound())
	{
		UE_LOG(CharacterLog, Warning, TEXT("ptrFunc.IsBound()"));
		ptrFunc.Execute();
	}
	else
	{
		UE_LOG(CharacterLog, Warning, TEXT("!ptrFunc.IsBound()"));
	}
}

void ACharacter_Base::SetBattleIdleBasePose(UAnimSequenceBase* NewPose)
{
	UE_LOG(CharacterLog, Display, TEXT("ACharacter_Base::SetBattleIdleBasePose"));
	UAnimInstancePrimary* AnimInstance = Cast<UAnimInstancePrimary>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetNewBasePose(NewPose);
	}
	else
	{
		UE_LOG(CharacterLog, Error, TEXT("!AnimInstance"));
	}
}

const FTransform ACharacter_Base::GetPlayerViewPoint() const
{
	FVector InLocation;
	FRotator InRotation;
	GetController()->GetPlayerViewPoint(InLocation, InRotation);
	
	return FTransform(InRotation, InLocation);
}

bool ACharacter_Base::State(ECharacterState State) const
{
	return CharacterState & static_cast<uint8>(State);
}

void ACharacter_Base::AddState(ECharacterState NewState)
{
	CharacterState |= static_cast<uint8>(NewState);
	if (!HasAuthority() && IsLocallyControlled())
		ServerRefreshState(CharacterState);
}

void ACharacter_Base::RemoveState(ECharacterState StateToRemove)
{
	CharacterState &= ~static_cast<uint8>(StateToRemove);
	if (!HasAuthority() && IsLocallyControlled())
		ServerRefreshState(CharacterState);
}

void ACharacter_Base::ServerRefreshState_Implementation(uint8 currentCharacterState)
{
	this->CharacterState = currentCharacterState;
}

void ACharacter_Base::Restart()
{
	Super::Restart();
	if (IsLocallyControlled())
	{
		CurrentHUD = Cast<AHUD_Base>(GetController<APlayerController>()->MyHUD);
		if (CurrentHUD)
		{
			UE_LOG(CharacterLog, Error, TEXT("CurrentHUD Restart"));
		}
		else
		{
			UE_LOG(CharacterLog, Error, TEXT("!CurrentHUD Restart"));
		}
	}
}

void ACharacter_Base::PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass)
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}
	else
	{
		UE_LOG(CharacterLog, Warning, TEXT("!PC PlayCameraShake"));
	}
}