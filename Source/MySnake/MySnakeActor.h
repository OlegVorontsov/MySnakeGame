// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#include "MySnakeActor.generated.h"

UCLASS()
class MYSNAKE_API AMySnakeActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMySnakeActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    int32 SnakeSize = 50;
    float StepSnake = 50.f;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    TArray<UStaticMeshComponent*> SnakeBody;

    class USphereComponent* MyRootComponent;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    class USphereComponent* TriggerSphere;

    //������� ��������� � TriggerSphere
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void CreateSnakeBody();

    //���-�� ������� ������ ������
    UPROPERTY(EditAnywhere)
    int32 VisibleBodyChank = 3;

    //������� ���������� ������� ��������� �����
    void SetVisibleChank();

    //����������� �������� ������
    UPROPERTY(EditAnywhere)
    FVector2D DirectionMoveSnake;

    //�������� � ��������
    float StepDelay = 0.5f;

    //���������� �������
    float TimeBufer = 0;

    //������� �������� ������ �� �����������
    void MoveSnake();

    //���������� ��� �������� ��������� �����
    int32 Score = 0;

    //���������� ��� �������� ������ ������
    UPROPERTY(EditAnywhere)
    int32 Lives = 5;

    //���������� ��� �������� ���������� ��������� ������� �� �����
    UPROPERTY(EditAnywhere)
    int32 BonusLivesCount = 0;

    //��� �������� ������ � ����
    class AMyPawnCamera* WhoPawn;

    //������� ��������� ����� �������
    void HaveDamage();

    //�������� ���� �������� ������
    UPROPERTY(EditAnyWhere)
    UAudioComponent* OnEatAppleSound;

    //�������� ���� ��������� ������
    UPROPERTY(EditAnyWhere)
    UAudioComponent* OnGetBonusSound;

    //�������� ���� ��������� ������
    UPROPERTY(EditAnyWhere)
    UAudioComponent* SnakeAppearSound;
};
