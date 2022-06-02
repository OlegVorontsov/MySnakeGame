// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "MySnakeActor.h"

#include "MyAppleActor.generated.h"

UCLASS()
class MYSNAKE_API AMyAppleActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMyAppleActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //объ€вл€ем сферу коллизии дл€ €блока
    class USphereComponent* MyRootComponent;

    //объ€вл€ем объект дл€ €блока
    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    class UStaticMesh* AppleMesh;

    //объ€вл€ем объект еды
    UPROPERTY(EditAnyWhere)
    class UStaticMeshComponent* EatChank;

    //объ€вл€ем цвет дл€ €блока
    class UMaterialInstance* AppleColor;

    //ограничение времени жизни €блок
    float StepDelay = 10.f;
    //накопитель вермени
    float TimeBufer = 0;
};
