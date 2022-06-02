// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawnCamera.h"
#include "MySnakeActor.h"
#include "MyAppleActor.h"
#include "BonusSpeedHigher.h"
#include "BonusSpeedLower.h"
#include "BonusLife.h"
#include "MyDeathActor.h"

DEFINE_LOG_CATEGORY_STATIC(MyPawnCameraLog, All, All);

// Sets default values
AMyPawnCamera::AMyPawnCamera()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MyRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootModel"));
    RootComponent = MyRootComponent;

    CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
    CameraSpring->SetRelativeLocation(FVector(0, 0, 0));
    CameraSpring->AddRelativeRotation(FRotator(-90.f, 0, 0));
    CameraSpring->TargetArmLength = 1700.0f;
    CameraSpring->bDoCollisionTest = false;
    CameraSpring->SetupAttachment(MyRootComponent);

    MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    MyCamera->SetupAttachment(CameraSpring, USpringArmComponent::SocketName);

    //������������� ���������� �� 0 ������
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawnCamera::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyPawnCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //���� ������� ����� > 0 �� ��������� ������/������
    if (GameMode > 0)
    {
        BuferTimeApple += DeltaTime;
        BuferTimeBonus += DeltaTime;

        if (BuferTimeApple > DelaySpawnApple)
        {
            //�������� � ����� �� ����
            if (!GamePause)
            {
                AddRandomApple();
                BuferTimeApple = 0;
            }
        }
        if (BuferTimeBonus > DelaySpawnBonus)
        {
            //�������� � ����� �� ����
            if (!GamePause)
            {
                AddRandomBonus();
                BuferTimeBonus = 0;
            }
        }
    }
}

// Called to bind functionality to input
void AMyPawnCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    InputComponent->BindAction("Pause", IE_Pressed, this, &AMyPawnCamera::GameOnPause);

    //�������� ������� ��� ������� ������ W S A D
    InputComponent->BindAxis("KeyMapMove", this, &AMyPawnCamera::FMove);
}

//������� ��������� ������ �� �����
void AMyPawnCamera::AddSnakeToMap()
{
    //�������� ��������� ������� ��� ����������� ������ ���� (RootModel)
    FVector StartPoint = GetActorLocation();
    //�������� ������� ����� �����
    FRotator StartPointRotation = GetActorRotation();

    //���� ����� ���������� �� ������ ���� ���� ������
    if (GetWorld())
    {
        MySnakePlayer = GetWorld()->SpawnActor<AMySnakeActor>(StartPoint, StartPointRotation);

        GameMode = 1;
        //��������� ��� ����������� ������ - �����
        MySnakePlayer->WhoPawn = this;

        MySnakePlayer->Lives = MySnakePlayer->Lives + LivesAdd - LivesGetter;
        // UE_LOG(MyPawnCameraLog, Display, TEXT("Lives: %d"), GetLives());
    }
}

//������� ������������ ������� ������� �������
void AMyPawnCamera::FMove(float ButtonValue)
{
    //�������� ����� ����� �� ������
    int32 Key = ButtonValue;

    //���� ������� ����� > 0 �� �������� ������� �� ������
    if (GameMode > 0)
    {
        //�������� � ����� �� ����. ���� �� ������������� ������
        if (!GamePause)
        {
            //��������� ��� ����� �����
            switch (Key)
            {
                // W
            case 1:
                if (WSAD.X != 1) //�������� �� �������� � �������� �������
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.X = -1;
                }
                break;
                // S
            case 2:
                if (WSAD.X != -1)
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.X = 1;
                }
                break;
                // A
            case 3:
                if (WSAD.Y != -1)
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.Y = 1;
                }
                break;
                // D
            case 4:
                if (WSAD.Y != 1)
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.Y = -1;
                }
                break;
            }
            //��������� �� ������ �� ������
            if (MySnakePlayer)
            {
                MySnakePlayer->DirectionMoveSnake = WSAD;
            }
        }
        else
        {
            MySnakePlayer->DirectionMoveSnake = FVector2D(0, 0);
        }
    }
}

//������� �������� ����� � ��������� ������ �����
void AMyPawnCamera::AddRandomApple()
{
    //������� �������� ��� ������
    FRotator StartPointRotation = FRotator(0, 0, 0);

    //�������� ��������� ����� �� � ��� �������� ������
    float SpawnX = FMath::FRandRange(MinX, MaxX);
    //�������� ��������� ����� �� Y ��� �������� ������
    float SpawnY = FMath::FRandRange(MinY, MaxY);

    //������� ����� ��� �������� ������
    FVector StartPoint = FVector(SpawnX, SpawnY, SpawnZ);

    //���� ����� ���������� �� ������� ������
    if (MySnakePlayer)
    {
        //��������� ���������� �� �����
        if (GetWorld())
        {
            GetWorld()->SpawnActor<AMyAppleActor>(StartPoint, StartPointRotation);
        }
    }
}

//������� �������� ������� � ��������� ������ �����
void AMyPawnCamera::AddRandomBonus()
{
    //������� �������� ��� ������
    FRotator StartPointRotation = FRotator(0, 0, 0);

    //�������� ��������� ����� �� � ��� �������� �������
    float SpawnXFirst = FMath::FRandRange(MinX, MaxX);
    float SpawnXSecond = FMath::FRandRange(MinX, MaxX);
    float SpawnXThird = FMath::FRandRange(MinX, MaxX);

    //�������� ��������� ����� �� Y ��� �������� �������
    float SpawnYFirst = FMath::FRandRange(MinY, MaxY);
    float SpawnYSecond = FMath::FRandRange(MinY, MaxY);
    float SpawnYThird = FMath::FRandRange(MinY, MaxY);

    //������� ����� ��� �������� �������
    FVector StartPointFirst = FVector(SpawnXFirst, SpawnYFirst, SpawnZ);
    FVector StartPointSecond = FVector(SpawnXSecond, SpawnYSecond, SpawnZ);
    FVector StartPointThird = FVector(SpawnXThird, SpawnYThird, SpawnZ);

    //���� ����� ���������� �� ������� �����
    if (MySnakePlayer)
    {
        //��������� ���������� �� �����
        if (GetWorld())
        {
            GetWorld()->SpawnActor<ABonusSpeedHigher>(StartPointFirst, StartPointRotation);
            GetWorld()->SpawnActor<ABonusSpeedLower>(StartPointSecond, StartPointRotation);
            GetWorld()->SpawnActor<ABonusLife>(StartPointThird, StartPointRotation);
        }
    }
}

//������� ���������� ���������� ��������� �����
int32 AMyPawnCamera::GetScore()
{
    if (MySnakePlayer)
    {
        return MySnakePlayer->Score;
    }
    return 0;
}

//������� ���������� ���������� ��������� ������� �� �����
int32 AMyPawnCamera::GetBonusLivesCount()
{
    if (MySnakePlayer)
    {
        return MySnakePlayer->BonusLivesCount;
    }
    return 0;
}

//������� ���������� ���������� ������ ������
int32 AMyPawnCamera::GetLives()
{
    if (MySnakePlayer)
    {
        return MySnakePlayer->Lives;
    }
    return 0;
}

//������� �������� ���� � �����
void AMyPawnCamera::GameOnPause()
{
    GamePause = !GamePause;
}

//������� ��������� ����� �������
void AMyPawnCamera::SnakeDestroy()
{
    //���������� ������ ���� ��� ���������� � ���� �� �������� ������
    if ((MySnakePlayer) && (GetLives() <= 0))
    {
        //��������� ���� � ����� ������ ����
        GameMode = 0;
        MySnakePlayer->Destroy(true, true);
    }
    else
    {
        LivesGetter++;
        LivesAdd = MySnakePlayer->BonusLivesCount;
        MySnakePlayer->Destroy(true, true);
        AddSnakeToMap();
    }
}
