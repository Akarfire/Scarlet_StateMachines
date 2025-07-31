# Scarlet State Machines

Scarlet State Machines is an Unreal Engine 5 Plugin, implementing simplistic but flexible explicit state machines, that are easy to use with both C++ and Blueprints.

### Features:

* **Modularity** - same states can be reused between multiple state machines.

* **Independent Condition Handling** - Conditions for inter-state transitions are handled separately from state logic, further improving modularity.

---
# Usage

*Plugin includes example state machines for both C++ and Blueprints.*

### Writing State Machine Code

#### State Names

States are identified using `uint8` `(Byte)` values, it is recommended to create an `Enum` to handle them using predefined names.

**IMPORTANT: 0 - Must always be a NONE, since State Machines interpret it as an absence of current state (before any actual state was set)!**


Example of a C++ state enum:

```c++
UENUM(BlueprintType)
enum class ECodeExampleStateEnum : uint8
{
	None UMETA(DisplayName="None"), // <= MUST ALWAYS BE THIS WAY!
	One UMETA(DisplayName="One"),
	Two UMETA(DisplayName="Two"),
	Three UMETA(DisplayName="Three")
};
```

Example of a Blueprint state enum:

!(BlueprintEnum)[./Images/BlueprintEnum.png]

#### States

To define states, that are going to be used in a state machine, one must create classes for each of them, inheriting `USSM_StateBase` class.

In the new class, the following methods can and probably should be overridden:

* `EnterState_Implementation` - is called once this state becomes active;

* `ExitState_Implementation` - is called once the state stops being active;

* `UpdateState_Implementation` - is called every time the state machine is updated, if the state is active.

Using the methods above, one can implement state's logic.


Example in C++:

```c++
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
```

Example in Blueprints:

![[BlueprintState.png]]


#### State Machine

To create the state machine itself, one must create a class derived from `USSM_StateMachine`.


##### Defining Transitions

Inside the state machine, condition functions for inter-state transitions are defined:

For each possible transition (direction matters) one should create a function of the following signature:

```c++
UFUNCTION()
bool Condition_State1_State2() { return TestValue > 0.f; }
```

**IMPORTANT: Condition function MUST be a UFUNCTION regardless of any circumstances!**

Blueprint example:

![[BlueprintConditionFunction.png]]


##### Virtual Methods

To continue the setup, one should override the following methods:

* `OnInitStateMachine_Implementation` - is called once the state machine is initialized, should contain state and transition registration;

* `OnUpdateStateMachine_Implementation` - is called every time the state machine is updated;


##### Registering States

Adding initial states to the state machine is performed inside `OnInitStateMachine_Implementation` using one of the following functions:

1. *Creates and adds a new state, of a specified class, the new state is assigned the `InStateID` identifier:*
```c++
void AddNewState(uint8 InStateID, TSubclassOf<USSM_StateBase> InStateClass);
```

2. *Adds an already existing state object to the state machine, assigning the `InStateID` identifier to it:*
```c++
void AddNewStateExisting(uint8 InStateID, USSM_StateBase* InState);
```

*Note: States and Transitions can be added and removed even in runtime.*


##### Registering Transitions

Next, one needs to register the conditions, this can be accomplished using the following functions:

1. *Registers a transition, described in `FStateTransition` structure:*
```c++
void RegisterTransition(const FStateTransition& InStateTransition);
```

```c++
struct FStateTransition
{
	// State, where the transition originates from
	uint8 OriginState;

	// New state, taken by the state machine if the condition of the transition is matched
	uint8 TargetState;

	// Name of the condition function (returns bool, no parameters)
	FName ConditionFunctionName;

	// Object, that owns the condtion function (if set to nullptr, THIS will be used)
	UObject* CondtionFunctionOwner;
};
```

*Note: condition function can be located outside of the state machine (in any other `UObject`), but it is generally convenient to keep them inside the state machine .*

2. *Registers multiple transitions, described with `FStateTransition` structures:*
```c++
void RegisterMultipleTransitions(const TArray<FStateTransition>& InStateTransitions);
```

3. *A more convenient transition registration method, that can only register transitions that are located inside the State Machine:
```c++
void RegisterTransitionLocal(uint8 InOriginState, uint8 InTargetState, FName InConditionFunctionName);
```

4. *Attempts to automatically find existing condition functions inside the state machine, with the specified naming convention:*
```c++
void AutoTransitionRegistration(const TArray<FString> StateNames, const FString& ConditionFunctionNamePrefix = "Condition_", const FString& ConditionFunctionNameStateConnector = "_");
```
*Example:* `Condition_StateOne_StateTwo` means this is a condition function for a transition from `StateOne` to `StateTwo`.


##### Setting Initial State

To set the initial state of the state machine, use the following method:
```c++
void ForceCallStateTransition(uint8 InNewState);
```

*Note: This method can also be used by any state to switch to a different state, bypassing the conditional transition mechanism, but it is not recommended!*


##### Examples

Example in C++:

```c++
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
```

Example in Blueprints:

![[BlueprintStateMachine.png]]

---
### Using State Machines

To use a state machine, one must first construct it. If using Blueprints, one can use `ScarletStateMachines_Utilities` function library (Option 1):

![[BlueprintCreatingStateMachines.png]]


*Note: Usually state machines can be initialized right after being created, but if your case requires later initialization, it is achievable by unchecking `AutoInit` flag.*


In C++, the same result is achieved with the following code:

```c++
USSM_StateMachine* NewStateMachine = NewObject<USSM_StateMachine>(this, StateMachineClass);

// Initializing state machine
if (NewStateMachine)
	NewStateMachine->InitStateMachine();
```

**IMPORTANT: State machines must always be initialized using `InitStateMachine` before actual use!**


To function, state machine must be updated, for example, you can update your state machine every Tick:

```c++
// Called every frame
void USomeClass::Tick(float DeltaTime, ...)
{
	Super::Tick(DeltaTime, ...);

	if (StateMachine)
		StateMachine->UpdateStateMachine(DeltaTime);

	...
}
```

![[BlueprintUpdatingStateMachine.png]]

Updating the state machine evaluates state transitions and calls `UpdateState` on the active state.



### Additional Tips

#### State Locking

States can use the following method to prevent the state machine from switching to a different state based on the transition-conditions mechanism:

```c++
void SetStateLocked(bool InLocked);
```

*Note: While the active state is locked no transition-conditions are being checked, this might help save up performance if condition logic is heavy.*
