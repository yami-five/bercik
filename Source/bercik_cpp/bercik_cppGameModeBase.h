// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseTile.h"
#include "MyCharacter.h"
#include "bercik_cppGameModeBase.generated.h"

UCLASS()
class BERCIK_CPP_API ACPP_bercik_cppGameMode : public AGameModeBase
{
	GENERATED_BODY()

		UPROPERTY()
		TSubclassOf<class ABaseTile> SubbercikSpawner;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SubPassage;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SubWall;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SubBlood;

	UPROPERTY()
		TSubclassOf<class AMyCharacter> SubCharacter;

	UFUNCTION(BlueprintCallable)
		void Spawn();

public:
	ACPP_bercik_cppGameMode();

	TArray<FCharArray> GenMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMaze(TArray<FCharArray> maze);

	void SpawnPlayer(TArray<FCharArray> maze);

	bool CheckMaze(TArray<FCharArray> maze);

	TArray<FVector> berciksLocations;
};

USTRUCT()
struct FIntArray {
	GENERATED_BODY()
public:

	TArray<int32> Ints;

	int32 operator[] (int32 i) {
		return Ints[i];
	}

	void Add(int32 number) {
		Ints.Add(number);
	}
};

USTRUCT()
struct FCharArray {
	GENERATED_BODY()
public:

	TArray<char> Chars;

	char& operator[] (int32 i) {
		return Chars[i];
	}

	void Add(char chr) {
		Chars.Add(chr);
	}
};