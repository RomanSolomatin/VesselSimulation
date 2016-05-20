// Fill out your copyright notice in the Description page of Project Settings.

#include "VesselSimulation.h"
#include "VesselActor.h"
#include "VesselSimLib/Ship.h"
#include <algorithm>

#include "Runtime/Core/Public/Containers/UnrealString.h"


// Sets default values
AVesselActor::AVesselActor() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create and position a mesh component so we can see where our vessel is
	UStaticMeshComponent* VesselModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	VesselModel->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VesselModelAsset(TEXT("/Game/3d-model"));
	if (VesselModelAsset.Succeeded()) {
		VesselModel->SetStaticMesh(VesselModelAsset.Object);
		VesselModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VesselModel->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		VesselModel->SetWorldScale3D(FVector(600.0f));
	}
}

// Called when the game starts or when spawned
void AVesselActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AVesselActor::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}

int AVesselActor::getId() {
	return id;
}

void AVesselActor::setId(int _id) {
	id = _id;
}