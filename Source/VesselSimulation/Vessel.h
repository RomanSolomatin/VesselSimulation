// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "VesselSimLib/IShip.h"
#include "Vessel.generated.h"

UCLASS()
class VESSELSIMULATION_API AVessel : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVessel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* OurVisibleComponent;
	IShip* m_ship;

	//Input functions
	int rudder_input_dir = 0;
	void RudderInputLeft();
	void RudderInputCancelLeft();
	void RudderInputRight();
	void RudderInputCancelRight();

	int engine_input = 0;
	void EngineUp();
	void EngineDown();
};


