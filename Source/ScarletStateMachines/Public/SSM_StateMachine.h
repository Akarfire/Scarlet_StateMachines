// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SSM_StateBase.h"
#include "SSM_StateMachine.generated.h"

// Transition Condition function delegate
DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FTransitionConditionDelegate);

// State updated notification delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChangedDelegate, uint8, PreviousState, uint8, NewState);


// Defines a transition between states in a state machine
USTRUCT(BlueprintType)
struct FStateTransition
{
	GENERATED_USTRUCT_BODY()

	// State, where the transition originates from
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 OriginState;

	// New state, taken by the state machine if the condition of the transition is matched
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 TargetState;

	// Name of the condition function (returns bool, no parameters)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ConditionFunctionName;

	// Object, that owns the condtion function (if set to nullptr, THIS will be used)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* CondtionFunctionOwner;

	// Delegate, bound to the Condition Function
	FTransitionConditionDelegate ConditionDelegate;

	FStateTransition(): OriginState(0), TargetState(0), ConditionFunctionName("None"), CondtionFunctionOwner(nullptr) {}

	FStateTransition(uint8 InOriginState, uint8 InTargetState, FName InCOnditionFunctionName, UObject* InConditionFunctionOwner = nullptr): 
		OriginState(InOriginState), TargetState(InTargetState), ConditionFunctionName(InCOnditionFunctionName), CondtionFunctionOwner(InConditionFunctionOwner) {}
};


/**
 * State machine base class
 */
UCLASS(Blueprintable)
class SCARLETSTATEMACHINES_API USSM_StateMachine : public UObject
{
	GENERATED_BODY()
	
protected:

	// Dictionary of all possible states
	UPROPERTY()
	TMap<uint8, USSM_StateBase*> States;

	// The state, that will be made active on the next state machine update, if 0, then no change is made
	uint8 BufferedNewState = 0;

	// Currently active state, what more can I say?
	uint8 ActiveState = 0;

	/* 
	 * Map of all registered transitions in the State Machine
	 * < State -> Array of < TransitionFromThisState > >
	 * These transitions are processed for the active state every update
	 * Uless there alredy is a BufferedNewState value ( != 0)
	*/
	TMap<uint8, TArray<FStateTransition>> TransitionMap;


public:

	// Notification delegate, called when the state is changed to anything else
	UPROPERTY(BlueprintAssignable, Category="Delegates")
	FOnStateChangedDelegate OnStateChanged;

protected:

	// TRANSITIONS

	// Sets a new active state
	void StateTransition(uint8 InNewState);

	// Processes all of the transitions of the given state
	void UpdateTransitions(uint8 InState);

	// Registers transition with condition function located in the ConditionFunctionSourceObject UObject
	void RegisterTransitionSourced(const FStateTransition& InStateTransition);

public:


	// Default constructor
	USSM_StateMachine();


	// STATE MACHINE BASICS
	
	// Call this when the state machine is created
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void InitStateMachine();

	// Call this every time the state machine should be updated (for example every frame)
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void UpdateStateMachine(float DeltaTime);


	// Called when the state machine is initialized (after the main init) (to be overriden)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletStateMachines|StateMachine")
 	void OnInitStateMachine();
	virtual void OnInitStateMachine_Implementation() {}

	// Called every time the state machine is updated (after the main update) (to be overriden)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ScarletStateMachines|StateMachine")
 	void OnUpdateStateMachine(float DeltaTime);
	virtual void OnUpdateStateMachine_Implementation(float DeltaTime) {}



	// STATE MANAGEMENT

	// Adds a new possible state
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void AddNewStateExisting(uint8 InStateID, USSM_StateBase* InState);

	// Creates a new state from
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void AddNewState(uint8 InStateID, TSubclassOf<USSM_StateBase> InStateClass);

	// Removes a state from the state machine
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void RemoveState(uint8 InStateID);

	// Tells the state machine to transition to a new active state
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void ForceCallStateTransition(uint8 InNewState) { BufferedNewState = InNewState; }

	// Returs the ID of the currently active state
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStateMachines|StateMachine")
	uint8 GetActiveState() { return ActiveState; }

	// Returs a pointer to the requested state object
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ScarletStateMachines|StateMachine")
	USSM_StateBase* GetState(uint8 InStateID);


	// TRANSITIONS

	// Registers a new transition between states
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void RegisterTransition(const FStateTransition& InStateTransition) { RegisterTransitionSourced(InStateTransition); }
	
	// Registers a new transition between states in a more stream lined way, condition function must be located in the state machine
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void RegisterTransitionLocal(uint8 InOriginState, uint8 InTargetState, FName InConditionFunctionName) { RegisterTransitionSourced( FStateTransition(InOriginState, InTargetState, InConditionFunctionName, this) ); }

	// Registers multiple new transitions between states
	UFUNCTION(BlueprintCallable, Category = "ScarletStateMachines|StateMachine")
	void RegisterMultipleTransitions(const TArray<FStateTransition>& InStateTransitions);
};
