// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSM_StateMachine.h"
#include "SSM_CodeExampleStateMachine.generated.h"

/*
 * Enum for the States of the State Machine
 * First value must always be None, as its considered as non-existance by the state machine
*/
UENUM(BlueprintType)
enum class ECodeExampleStateEnum : uint8
{
	None UMETA(DisplayName="None"),
	One UMETA(DisplayName="One"),
	Two UMETA(DisplayName="Two"),
	Three UMETA(DisplayName="Three")
};


// STATES

/**
 * Example State One
 */
UCLASS()
class SCARLETSTATEMACHINES_API USSM_CodeExampleStateOne : public USSM_StateBase
{
	GENERATED_BODY()
	
public:

	// Constructor
	USSM_CodeExampleStateOne() {}

	// Called when the state is made active
	virtual void EnterState_Implementation() override
	{
		// Some code here
	}

	// Called once the active state is changed to a different one
	virtual void ExitState_Implementation() override
	{
		// Some code here
	}

	// Called every state machine update when the state is active
	virtual void UpdateState_Implementation(float DeltaTime) override
	{
		// Some code here
	}
};

/**
 * Example State Two
 */
UCLASS()
class SCARLETSTATEMACHINES_API USSM_CodeExampleStateTwo : public USSM_StateBase
{
	GENERATED_BODY()

public:	

	// Constructor
	USSM_CodeExampleStateTwo() {}

	// Called when the state is made active
	virtual void EnterState_Implementation() override
	{
		// Some code here
	}

	// Called once the active state is changed to a different one
	virtual void ExitState_Implementation() override
	{
		// Some code here
	}

	// Called every state machine update when the state is active
	virtual void UpdateState_Implementation(float DeltaTime) override
	{
		// Some code here
	}
};

/**
 * Example State Three
 */
UCLASS()
class SCARLETSTATEMACHINES_API USSM_CodeExampleStateThree : public USSM_StateBase
{
	GENERATED_BODY()
	
public:

	// Constructor
	USSM_CodeExampleStateThree() {}

	// Called when the state is made active
	virtual void EnterState_Implementation() override
	{
		// Some code here
	}

	// Called once the active state is changed to a different one
	virtual void ExitState_Implementation() override
	{
		// Some code here
	}

	// Called every state machine update when the state is active
	virtual void UpdateState_Implementation(float DeltaTime) override
	{
		// Some code here
	}
};



// STATE MACHINE

/**
 * The State Machine in question
 */
UCLASS()
class SCARLETSTATEMACHINES_API USSM_CodeExampleStateMachine : public USSM_StateMachine
{
	GENERATED_BODY()

public:

	// Some random example value, that will be used for conditions
	UPROPERTY(BlueprintReadWrite)
	float TestValue;

public:

	// Constructor
	USSM_CodeExampleStateMachine() {}

	// Called when the state machine is initialized (after the main init)
	virtual void OnInitStateMachine_Implementation() override
	{
		// Registering States
		AddNewState( (uint8)ECodeExampleStateEnum::One, USSM_CodeExampleStateOne::StaticClass() );
		AddNewState( (uint8)ECodeExampleStateEnum::Two, USSM_CodeExampleStateTwo::StaticClass() );
		AddNewState( (uint8)ECodeExampleStateEnum::Three, USSM_CodeExampleStateThree::StaticClass() );

		// Registering Transtions: Origin state, Target state, Condition Function NAME
		RegisterMultipleTransitions( {

			FStateTransition((uint8)ECodeExampleStateEnum::One, 	(uint8)ECodeExampleStateEnum::Two, 		"Condition_One_Two", 	this), 
			FStateTransition((uint8)ECodeExampleStateEnum::One, 	(uint8)ECodeExampleStateEnum::Three, 	"Condition_One_Three", 	this),
			FStateTransition((uint8)ECodeExampleStateEnum::Two, 	(uint8)ECodeExampleStateEnum::One, 		"Condition_Two_One", 	this),
			FStateTransition((uint8)ECodeExampleStateEnum::Three, 	(uint8)ECodeExampleStateEnum::One, 		"Condition_Three_One", 	this)
		
		});

		//Alternatively you can use automatic Transition Registration:
		//AutoTransitionRegistration({"None", "One", "Two", "Three"}, "Condition_", "_");

		// Set Default State
		ForceCallStateTransition( (uint8)ECodeExampleStateEnum::One ); // Can just use "1" instead of "(uint8)ECodeExampleStateEnum::One", but this way it has a name
	}

	// Called every time the state machine is updated (after the main update)
	virtual void OnUpdateStateMachine_Implementation(float DeltaTime) override {}

	// TRANSITION CONDITIONS
	// Must be a UFUNCTION, will not work otherwise, and no, it will not crash, you will just be stuck there with no real signs of errors, so yeah

	// Condition for transition from state One to state Two
	UFUNCTION()
	bool Condition_One_Two() { return TestValue > 0; }
	// ...
	UFUNCTION()
	bool Condition_One_Three() { return TestValue < 0; }
	UFUNCTION()
	bool Condition_Two_One() { return TestValue <= 0; }
	UFUNCTION()
	bool Condition_Three_One() { return TestValue >= 0; }
};
