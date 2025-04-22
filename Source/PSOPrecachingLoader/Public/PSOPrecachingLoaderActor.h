// Copyright 2024 - %Y Impact Forge LLC, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "PSOPrecachingLoaderActor.generated.h"

UCLASS()
class PSOPRECACHINGLOADER_API APSOPrecachingLoaderActor : public AActor
{
	GENERATED_BODY()

public:
	APSOPrecachingLoaderActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSO Precaching")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSO Precaching", meta = (AllowedClasses = "World"))
	FSoftObjectPath NextLevelToLoad;

	// Add a helper function to get the level name
	UFUNCTION(BlueprintCallable, Category = "PSO Precaching")
	FName GetLevelNameFromPath() const
	{
		return FName(*FPackageName::GetShortName(NextLevelToLoad.ToString()));
	}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PSO Precaching")
	UUserWidget* LoadingWidget;

	UFUNCTION(BlueprintCallable, Category = "PSO Precaching")
	void StartPSOPrecaching();

protected:
	virtual void BeginPlay() override;

private:
	void OnPSOPrecachingComplete();
	void LoadNextLevel();
};
