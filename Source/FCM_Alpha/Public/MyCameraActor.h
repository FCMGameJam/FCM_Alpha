// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class FCM_ALPHA_API AMyCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	APlayerController* PC;
};
