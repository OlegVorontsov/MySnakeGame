// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MyPawnCamera.generated.h"

UCLASS()
class MYSNAKE_API AMyPawnCamera : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AMyPawnCamera();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere)
    class UBoxComponent* MyRootComponent;

    UPROPERTY(EditAnywhere)
    class USpringArmComponent* CameraSpring;

    UPROPERTY(EditAnywhere)
    class UCameraComponent* MyCamera;

    class AMySnakeActor* MySnakePlayer;
    class ASnakeHeadTrigger* HeadTrigger;

    //������� ��������� ������ �� ������
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    void AddSnakeToMap();

    //����������� �������� ������
    FVector2D WSAD;

    //������� ��� ���������� �������
    void FMove(float ButtonValue);

    //���������� ��������� �������� ���� �����/�������
    float MinY = -1500.f;
    float MaxY = 1500.f;
    float MinX = -800.f;
    float MaxX = 800.f;
    //����� �� ��� Z ��� �������� �����/�������
    float SpawnZ = 50.f;

    //������� �������� ����� � ��������� ������ �����
    void AddRandomApple();
    //������� �������� ������� � ��������� ������ �����
    void AddRandomBonus();

    //�������� ��� �������� �����
    float DelaySpawnApple = 1.f;
    //�������� ��� �������� �������
    float DelaySpawnBonus = 10.f;

    //���������� �������
    float BuferTimeApple = 0;
    float BuferTimeBonus = 0;

    //���������� �������� ����
    int32 GameMode = 0;

    //������� ������� ������������ �������� GameMode
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetGameMode() const { return GameMode; }

    //������� ���������� ���������� ��������� �����
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetScore();

    //������� ���������� ���������� ������ ������
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetLives();

    //������� ���������� ���������� ��������� ������� �� �����
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetBonusLivesCount();

    //���������� ��� ��������� ������ ������
    UPROPERTY(EditAnywhere)
    int32 LivesGetter = 0;

    //���������� ��� ����������� ������ ������ �� ���� ������
    UPROPERTY(EditAnywhere)
    int32 LivesAdd;

    //���������� ��� ������� ����� � ����
    bool GamePause = false;

    //������� �������� ���� � �����
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    void GameOnPause();

    //������� ������� ������������ ������ ����� � blueprint
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetGamePause() const { return GamePause; }

    //������� ��������� ����� �������
    void SnakeDestroy();
};
