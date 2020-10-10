// Copyright Epic Games, Inc. All Rights Reserved.

#include "FCM_AlphaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "PickUp.h"
#include "DrawDebugHelpers.h"
#include "Templates/Casts.h"
#include "GameFramework/Controller.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Rotator.h"

AFCM_AlphaCharacter::AFCM_AlphaCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetUsingAbsoluteRotation(false); // Don't want arm to rotate when character does
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetRelativeRotation(FRotator(60.0f, 0.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	/*
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//TopDownCameraComponent->SetupAttachment(ACharacter::GetMesh());
	TopDownCameraComponent->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	*/

	DirectionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DirectionCamera"));
	DirectionCamera->SetupAttachment(ACharacter::GetMesh());
	DirectionCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 800.0f));
	DirectionCamera->bUsePawnControlRotation = true;

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 100.0f));
	HoldingComponent->SetupAttachment(ACharacter::GetMesh());

	CurrentItem = NULL;
	bCanMove = true;
	bInspecting = false;
}

void AFCM_AlphaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Start = DirectionCamera->GetComponentLocation();
	ForwardVector = DirectionCamera->GetForwardVector();
	End = ((ForwardVector * 200.0f) + Start);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);             //后面四个参数什么意思？

	if (!bHoldingItem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%d"), bHoldingItem);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParams))
		{
			//UE_LOG(LogTemp, Warning, TEXT("11111111111"));
			if (Hit.GetActor()->GetClass()->IsChildOf(APickUp::StaticClass()))
			{
				//UE_LOG(LogTemp, Warning, TEXT("222222222222"));
				CurrentItem = Cast<APickUp>(Hit.GetActor());
			}
		}
		else
		{
			CurrentItem = NULL;
		}
	}

	/**/
	if (bInspecting)
	{
		//UE_LOG(LogTemp, Warning, TEXT("11111111111"));
		if (bHoldingItem)
		{
			//UE_LOG(LogTemp, Warning, TEXT("222222222222"));
			DirectionCamera->SetFieldOfView(FMath::Lerp(DirectionCamera->FieldOfView, 90.0f, 0.f));
			HoldingComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 100.0f));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.90000002f;
			CurrentItem->RotateActor();
		}
		else
		{
			DirectionCamera->SetFieldOfView(FMath::Lerp(DirectionCamera->FieldOfView, 45.0f, 0.1f));
		}
	}
	else
	{
		DirectionCamera->SetFieldOfView(FMath::Lerp(DirectionCamera->FieldOfView, 90.0f, 0.f));     //这个操作有什么用？

		if (bHoldingItem)
		{
			//UE_LOG(LogTemp, Warning, TEXT("222222222222"));
			HoldingComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 100.0f));
		}
	}

}

void AFCM_AlphaCharacter::BeginPlay()
{
	Super::BeginPlay();

	//PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	//PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;

}

void AFCM_AlphaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AFCM_AlphaCharacter::OnAction);
	PlayerInputComponent->BindAction("Strike", IE_Pressed, this, &AFCM_AlphaCharacter::Strike);

	//PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AFCM_TEST1Character::OnInspect);
	//PlayerInputComponent->BindAction("Inspect", IE_Released, this, &AFCM_TEST1Character::OnInspectReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFCM_AlphaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFCM_AlphaCharacter::MoveRight);


}


void AFCM_AlphaCharacter::OnInspect()
{
	if (bHoldingItem)
	{
		LastRotation = GetControlRotation();
		ToggleMovement();
	}
	else
	{
		bInspecting = true;
	}
}

void AFCM_AlphaCharacter::OnInspectReleased()
{
	if (bInspecting && bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	}
	else
	{
		bInspecting = false;
	}

}

void AFCM_AlphaCharacter::OnAction()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnAction"));
	
	if (CurrentItem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CurrentItem"));
		ToggleItemPickup();
	}
	
}

void AFCM_AlphaCharacter::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	DirectionCamera->bUsePawnControlRotation = !DirectionCamera->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AFCM_AlphaCharacter::ToggleItemPickup()
{
	//UE_LOG(LogTemp, Warning, TEXT("ToggleItemPickup"));
	
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->PickUp();

		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
	
}

void AFCM_AlphaCharacter::MoveForward(float Value)
{
	//AddMovementInput(GetActorForwardVector(), Value);
	//AddMovementInput(Controller->GetActorForwardVector(), Value);

	FRotator Rotation = Controller->GetControlRotation();
	FRotator PitchRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector Direction = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);


}

void AFCM_AlphaCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector(), Value);
	//AddControllerYawInput(Value);

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);

}

void AFCM_AlphaCharacter::Strike()
{
	
	if (CurrentItem)
	{
		CurrentItem->MyMesh->AddForce(ForwardVector * 100000 * CurrentItem->MyMesh->GetMass());
	}
	
}


