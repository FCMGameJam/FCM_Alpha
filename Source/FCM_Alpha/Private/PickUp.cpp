// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
APickUp::APickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	MyMesh->SetSimulatePhysics(true);
	RootComponent = MyMesh;

	bHolding = false;
	bGravity = true;

}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;

	MyCharacter->GetComponents(Components);

	UE_LOG(LogTemp, Warning, TEXT("Components %d"), Components.Num());

	if (Components.Num() > 0)
	{

		for (auto& Comp : Components)
		{
			if (Comp->GetName() == "HoldingComponent")
			{
				HoldinComp = Cast<USceneComponent>(Comp);    //???
			}
		}
	}

}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding && HoldinComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("PositionChange"));
		SetActorLocationAndRotation(HoldinComp->GetComponentLocation(), HoldinComp->GetComponentRotation());
	}

}

void APickUp::RotateActor()
{
}

void APickUp::PickUp()
{
	UE_LOG(LogTemp, Warning, TEXT("PickUpChange"));

	bHolding = !bHolding;
	bGravity = !bGravity;

	UE_LOG(LogTemp, Warning, TEXT("bHolding %d"), bHolding);
	UE_LOG(LogTemp, Warning, TEXT("HoldinComp %d"), HoldinComp);

	MyMesh->SetEnableGravity(bGravity);
	MyMesh->SetSimulatePhysics(bHolding ? false : true);
	MyMesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (!bHolding)
	{
		MyMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ForwardVector = PlayerCamera->GetForwardVector();
	}

}

void APickUp::StrikeToFly()
{
}
