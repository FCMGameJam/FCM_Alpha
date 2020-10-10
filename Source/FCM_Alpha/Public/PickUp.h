// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

UCLASS()
class FCM_ALPHA_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickUp();

	UPROPERTY(EditANywhere, BlueprintReadOnly)
	UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* HoldinComp;

	bool bHolding;
	bool bGravity;

	FRotator ControlRotation;
	ACharacter* MyCharacter;
	class UCameraComponent* PlayerCamera;
	FVector ForwardVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void RotateActor();

	UFUNCTION()
	void PickUp();

	UFUNCTION()
	void StrikeToFly();

};
