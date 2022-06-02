// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//подклюаем необходимые библитеки
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstance.h"
#include "Components/StaticMeshComponent.h"

#include "MyDeathActor.generated.h"

UCLASS()
class MYSNAKE_API AMyDeathActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMyDeathActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //объ€вл€ем компонент дл€ стены
    UPROPERTY(EditAnyWhere)
    class UBoxComponent* MyRootComponent;

    //объвл€ем материал дл€ стены
    UPROPERTY(EditAnyWhere)
    class UMaterialInstance* WallColor;

    //объ€вл€ем статичный блок дл€ стены
    UPROPERTY(EditAnyWhere)
    class UStaticMeshComponent* WallChank;

    //функци€ оверлапа в стену
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
