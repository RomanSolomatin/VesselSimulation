// Fill out your copyright notice in the Description page of Project Settings.

#include "VesselSimulation.h"
#include "VesselSpawner.h"
#include <algorithm>


// Sets default values
AVesselSpawner::AVesselSpawner() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AVesselSpawner::BeginPlay() {
	Super::BeginPlay();

	// Clear all the actors and real vessels
	m_actors.clear();
	vsl_manager.clearAll();

	// First position and rotation
	FVector start_pos(0, 0, 2000);
	FRotator start_rot(0, 0, 0);

	// Spawn vessels
	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		for (auto i = 0; i < 4; ++i) {
			// Request a new real vessel
			vsl::IShip* new_vessel = vsl_manager.requestNewVessel("Basic Ship");

			// If delivered,
			if(new_vessel){
				// Create an actor for the real vessel
				AVesselActor* new_actor = World->SpawnActor<AVesselActor>(start_pos, start_rot, SpawnParams);
				if (new_actor) {
					new_actor->setId(new_vessel->getId()); // Match real vessel and actor Id's
					m_actors[new_actor->getId()] = new_actor; // Map the id : actor

					// Initialize the real vessel
					FRotator rot = new_actor->GetActorRotation();
					FVector loc = new_actor->GetActorLocation();
					new_vessel->init(vsl::Vector(loc.X, loc.Y, loc.Z), vsl::Vector(rot.Roll, rot.Pitch, rot.Yaw));

					// Change position and rotation for new ships
					start_pos.Y += 10000.0f;
					start_rot.Yaw += 15.0f;
				}
			}
		}
	}
}

// Called every frame
void AVesselSpawner::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	// Send rudder input to all
	if (rudder_input_dir != 0) {
		for (auto it = m_actors.begin(); it != m_actors.end(); ++it) {
			vsl::IShip* sh = vsl_manager.getVessel(it->second->getId());
			sh->setRudderAngle(std::max(std::min(sh->getRequestedRudderAngle() + rudder_input_dir * 100.0f * DeltaTime, sh->getMaxRudderAngle()), -sh->getMaxRudderAngle()));
		}
	}

	// Send engine input to all
	if (engine_input_dir != 0) {
		for (auto it = m_actors.begin(); it != m_actors.end(); ++it) {
			vsl::IShip* sh = vsl_manager.getVessel(it->second->getId());
			sh->setEngineOrder(sh->getEngineOrder() + engine_input_dir);
		}
		engine_input_dir = 0;
	}


	// Update all real vessels
	vsl_manager.update(DeltaTime);

	// Transfer vessel data to actors
	for (auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		AVesselActor* act = it->second;
		vsl::IShip* sh = vsl_manager.getVessel(act->getId());

		// Get position of the actor
		vsl::Vector pos = sh->getPosition();
		act->SetActorLocation(FVector(pos.x, pos.y, pos.z));

		// Get rotation of the actor
		vsl::Vector euler_rot = sh->getRotation();
		act->SetActorRotation(FRotator(euler_rot.y, euler_rot.z, euler_rot.x));
	}
}

// Called to bind functionality to input
void AVesselSpawner::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("IncrementEngineStep", IE_Pressed, this, &AVesselSpawner::EngineUp);
	InputComponent->BindAction("DecrementEngineStep", IE_Pressed, this, &AVesselSpawner::EngineDown);

	InputComponent->BindAction("RudderInputRight", IE_Pressed, this, &AVesselSpawner::RudderInputRight);
	InputComponent->BindAction("RudderInputRight", IE_Released, this, &AVesselSpawner::RudderInputCancelRight);
	InputComponent->BindAction("RudderInputLeft", IE_Pressed, this, &AVesselSpawner::RudderInputLeft);
	InputComponent->BindAction("RudderInputLeft", IE_Released, this, &AVesselSpawner::RudderInputCancelLeft);
}

void AVesselSpawner::RudderInputLeft() { rudder_input_dir = -1; }
void AVesselSpawner::RudderInputCancelLeft() { if (rudder_input_dir == -1) rudder_input_dir = 0; }
void AVesselSpawner::RudderInputRight() { rudder_input_dir = 1; }
void AVesselSpawner::RudderInputCancelRight() { if (rudder_input_dir == 1) rudder_input_dir = 0; }
void AVesselSpawner::EngineUp() { engine_input_dir = 1; }
void AVesselSpawner::EngineDown() { engine_input_dir = -1; }