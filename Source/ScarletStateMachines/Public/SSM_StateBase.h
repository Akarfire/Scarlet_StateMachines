// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SSM_StateBase.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SCARLETSTATEMACHINES_API USSM_StateBase : public UObject
{
	GENERATED_BODY()

protected:

	// ID of the state inside of the state machine
	uint8 StateID;

	// Pointer to the state machine of this state
	class USSM_StateMachine* StateMachine;

	/*
	 * If the state is locked, no registered transitions can change it from being active
	 * Forced transitions can still happen
	*/
	bool Locked = false;
	
public:

	// Constructor
	USSM_StateBase();

	// State main functionality

	// Called when the state is made active
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletStateMachines|State")
 	void EnterState();
	virtual void EnterState_Implementation() {}

	// Called every state machine update when the state is active
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletStateMachines|State")
	void UpdateState(float DeltaTime);
	virtual void UpdateState_Implementation(float DeltaTime) {}

	// Called once the active state is changed to a different one
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletStateMachines|State")
	void ExitState();
	virtual void ExitState_Implementation() {}


	// State ID Management
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|State")
	void SetStateID(uint8 InStateID) { StateID = InStateID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStateMachines|State")
	uint8 GetStateID() { return StateID; }


	// State Machine management

	// Called from the state machine
	void STATEMACHINE_SetStateMachine(class USSM_StateMachine* InStateMachine);
	
	// Must NEVER be called manually. Fires off after the main STATEMACHINE_OnSetStateMachine function has be called. Is used for implemention custom State base classes
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletStateMachines|State|Background")
	void STATEMACHINE_OnSetStateMachine();
	virtual void STATEMACHINE_OnSetStateMachine_Implementation() {}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStateMachines|State")
	class USSM_StateMachine* GetStateMachine() { return StateMachine; }


	// Locking

	// Updates state's Locked status
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|State")
	void SetStateLocked(bool InLocked) { Locked = InLocked; }

	/*
	 * Whether the state is locked or not
	 * If the state is locked, no registered transitions can change it from being active
	 * Forced transitions can still happen
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStateMachines|State")
	bool IsLocked() { return Locked; }
};
