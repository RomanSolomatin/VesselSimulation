// Fill out your copyright notice in the Description page of Project Settings.

#include "VesselSimulation.h"
#include "Vessel.h"
#include "VesselSimLib/Ship.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

// Sets default values
AVessel::AVessel()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurVisibleComponent->AttachTo(RootComponent);
}

AVessel::~AVessel() {
	if (m_ship != nullptr) delete m_ship;
}

// Called when the game starts or when spawned
void AVessel::BeginPlay()
{
	Super::BeginPlay();
	
	if(m_ship == nullptr){
		m_ship = new Ship();

		FRotator rot = GetActorRotation();
		m_ship->init(GetActorLocation(), FVector(rot.Roll, rot.Pitch, rot.Yaw));
	}
}

// Called every frame
void AVessel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Send inputs
	m_ship->setRudderDirection(rudder_input_dir);

	if (engine_input != 0) {
		if (engine_input == 1) m_ship->incrementEngineOrder();
		else if (engine_input == -1) m_ship->decrementEngineOrder();
		engine_input = 0;
	}

	// Update
	m_ship->update(DeltaTime);

	// Get position
	SetActorLocation(m_ship->getPosition());

	// Get rotation
	FVector euler_rot = m_ship->getRotation();
	SetActorRotation(FRotator(euler_rot.Y, euler_rot.Z, euler_rot.X));


// LOGS
	//"MyCharacter's Location is %s"
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString("Rudder Angle: ") + FString::SanitizeFloat(m_ship->getRequestedRudderAngle()) +
		FString("  |  ") + FString::SanitizeFloat(m_ship->getCurrentRudderAngle()));
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString("Thrust Power: ") + FString::SanitizeFloat(m_ship->getRequestedThrustPower()) +
		FString("  |  ") + FString::SanitizeFloat(m_ship->getCurrentThrustPower()));
}

// Called to bind functionality to input
void AVessel::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("IncrementEngineStep", IE_Pressed, this, &AVessel::EngineUp);
	InputComponent->BindAction("DecrementEngineStep", IE_Pressed, this, &AVessel::EngineDown);

	InputComponent->BindAction("RudderInputRight", IE_Pressed, this, &AVessel::RudderInputRight);
	InputComponent->BindAction("RudderInputRight", IE_Released, this, &AVessel::RudderInputCancelRight);
	InputComponent->BindAction("RudderInputLeft", IE_Pressed, this, &AVessel::RudderInputLeft);
	InputComponent->BindAction("RudderInputLeft", IE_Released, this, &AVessel::RudderInputCancelLeft);
}

void AVessel::RudderInputLeft() { rudder_input_dir = -1; }
void AVessel::RudderInputCancelLeft() { if (rudder_input_dir == -1) rudder_input_dir = 0; }
void AVessel::RudderInputRight() { rudder_input_dir = 1; }
void AVessel::RudderInputCancelRight() { if (rudder_input_dir == 1) rudder_input_dir = 0; }
void AVessel::EngineUp() { engine_input = 1; }
void AVessel::EngineDown() { engine_input = -1; }