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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PSO Precaching")
	FName NextLevelToLoad;

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
