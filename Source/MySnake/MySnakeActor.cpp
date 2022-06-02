// Fill out your copyright notice in the Description page of Project Settings.

#include "MySnakeActor.h"
#include "MyPawnCamera.h"
#include "MyAppleActor.h"
#include "BonusLife.h"

DEFINE_LOG_CATEGORY_STATIC(MySnakeActorLog, All, All);

// Sets default values
AMySnakeActor::AMySnakeActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MyRoot"));
    RootComponent = MyRootComponent;

    FVector Position = GetActorLocation();
    MyRootComponent->SetRelativeLocation(Position);

    //������� ��������� ��� ����� �������� ������
    OnEatAppleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("EatAudio"));
    OnEatAppleSound->bAutoActivate = false;
    OnEatAppleSound->SetupAttachment(MyRootComponent);
    static ConstructorHelpers::FObjectFinder<USoundBase> soundBaseapple(
        TEXT("SoundWave'/Game/SnakeContent/Sounds/hrum_jabloko.hrum_jabloko'"));
    OnEatAppleSound->SetSound(soundBaseapple.Object);

    //������� ��������� ��� ����� ��������� ������
    OnGetBonusSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BonusAudio"));
    OnGetBonusSound->bAutoActivate = false;
    OnGetBonusSound->SetupAttachment(MyRootComponent);
    static ConstructorHelpers::FObjectFinder<USoundBase> soundBasebonus(
        TEXT("SoundWave'/Game/SnakeContent/Sounds/BonusGet.BonusGet'"));
    OnGetBonusSound->SetSound(soundBasebonus.Object);

    //������� ��������� ��� ����� ��������� ������
    SnakeAppearSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SnakeAppearAudio"));
    SnakeAppearSound->bAutoActivate = false;
    SnakeAppearSound->SetupAttachment(MyRootComponent);
    static ConstructorHelpers::FObjectFinder<USoundBase> soundBaseAppear(
        TEXT("SoundWave'/Game/SnakeContent/Sounds/poyavlenie_personaga.poyavlenie_personaga'"));
    SnakeAppearSound->SetSound(soundBaseAppear.Object);

    CreateSnakeBody();
    //������������� �� ������� ��������� � Component
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AMySnakeActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AMySnakeActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMySnakeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SetVisibleChank();

    TimeBufer += DeltaTime;
    //��������� ���� ����� ������������
    if (TimeBufer > StepDelay)
    {
        MoveSnake();
        //�������� �����
        TimeBufer = 0;
    }
}

//������� �������� ���� ������
void AMySnakeActor::CreateSnakeBody()
{
    //����� ����������� ����� ������
    class UStaticMesh* SnakeChankMesh;
    SnakeChankMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

    //����� ��������� ��������� � ������ ��� ���� � ������
    class UMaterialInstance* BodyColor;
    BodyColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "M_Tech_Hex_Tile_Inst.M_Tech_Hex_Tile_Inst'"))
                    .Get();
    class UMaterialInstance* HeadColor;
    HeadColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "M_Tech_Hex_Tile_Pulse_Inst.M_Tech_Hex_Tile_Pulse_Inst'"))
                    .Get();

    //���������� ��� ������� ��������� ������
    FVector NextPoint = GetActorLocation();

    //��������� �������� ��� ���������� ����� ������
    FName NameChank;
    //���������� ��� �������� �����
    FString TheString;

    //������� ����� ������
    for (int32 i = 0; i < SnakeSize; i++)
    {
        //�������� ���������� ���
        TheString = "Chanks" + FString::FromInt(i);

        //��������� ��� ���������� ������ � �����
        NameChank = FName(*TheString);

        //������� ��������� ����� ������
        class UStaticMeshComponent* BodyChank = CreateDefaultSubobject<UStaticMeshComponent>(NameChank);

        //����������� ��������� ����� ������ �����
        BodyChank->SetStaticMesh(SnakeChankMesh);

        //������ ���������� ����� ������
        BodyChank->SetRelativeLocation(NextPoint);

        //��������� ����� ������ � ������
        SnakeBody.Add(BodyChank);

        //�������� �� ��� �
        NextPoint.X -= StepSnake;

        //��������� � ������ ������
        BodyChank->SetupAttachment(MyRootComponent);

        //������ ���� ����� ������
        BodyChank->SetMaterial(0, BodyColor);

        //������ ���� ������ ������ � ��������� ������� �� ��������
        if (i == 0)
        {
            BodyChank->SetMaterial(0, HeadColor);
            // BodyChank->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            BodyChank->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            BodyChank->SetCollisionObjectType(ECC_WorldDynamic);
            BodyChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

            TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
            TriggerSphere->InitSphereRadius(60.0f);
            // TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
            TriggerSphere->SetupAttachment(BodyChank);
            TriggerSphere->SetVisibility(true, true);
            TriggerSphere->SetHiddenInGame(true, true);

            TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            TriggerSphere->SetCollisionObjectType(ECC_PhysicsBody);
            TriggerSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
        }
        else
        {
            //�������� ������� �� �������� � �����
            BodyChank->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            BodyChank->SetCollisionObjectType(ECC_WorldStatic);
            BodyChank->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
            // BodyChank->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
    //������ �������� ������ �� �������� � ��������
    SetVisibleChank();

    SnakeAppearSound->Play();
}

//������� ��������� � TriggerSphere
void AMySnakeActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //��������� ��� � TriggerSphere ������ ���� ������
    if (OtherActor && (OtherActor == this) && OtherComp)
    {
        //���� ����� ������� �������� � 4 � �� ������, ������� ����
        for (int32 IndexHit = 4; IndexHit <= VisibleBodyChank; IndexHit++)
        {
            if (OtherComp == SnakeBody[IndexHit])
            {
                WhoPawn->SnakeDestroy();
            }
        }
    }
    else
    {
        //���� ������ ������. �������� ������� �������� ����� GetOverlappingActors � ��� �����
        AMyAppleActor* const Test = Cast<AMyAppleActor>(OtherActor);
        if (Test)
        {
            this->VisibleBodyChank++;
            this->Score++;
            OnEatAppleSound->Play();
            Test->Destroy(true, true);
        }
        else
        {
            // UE_LOG(MySnakeActorLog, Display, TEXT("Actor overlap: %s"), *OtherActor->GetName());
        }
    }
}

//������� ������ �������� ����� ������
void AMySnakeActor::SetVisibleChank()
{
    //���������� ��� ����� ������
    for (int32 IndexShow = 0; IndexShow < SnakeBody.Num(); IndexShow++)
    {
        //���� ����� ������ ������ � �������� �������� �� ���������� ��
        if (IndexShow < VisibleBodyChank)
        {
            SnakeBody[IndexShow]->SetVisibility(true, true);
        }
        else
        {
            SnakeBody[IndexShow]->SetVisibility(false, true);
        }
    }
}

//������� �������� ������ �� �����������
void AMySnakeActor::MoveSnake()
{
    //���� ���� �������� �� ����������, ���� ��� �����
    if ((DirectionMoveSnake.X != 0) || (DirectionMoveSnake.Y != 0))
    {
        //������� ��� �������� ������ � ������
        for (int Chank = SnakeBody.Num() - 1; Chank > 0; Chank--)
        {
            //�������� ���������� ���������� ����� � ���������� � ���������� V
            FVector V = SnakeBody[Chank - 1]->GetRelativeLocation();
            //������ �� ����� ����������
            SnakeBody[Chank]->SetRelativeLocation(V);
        }
        //�������� ���������� ������ �����
        FVector StartPoint = SnakeBody[0]->GetRelativeLocation();

        //���� �� ��������� �� � > 0 �� ������� ������ � ���� �������
        if (DirectionMoveSnake.X > 0)
            StartPoint.X -= StepSnake;
        if (DirectionMoveSnake.X < 0)
            StartPoint.X += StepSnake;
        //������� � ������ �������
        if (DirectionMoveSnake.Y > 0)
            StartPoint.Y += StepSnake;
        if (DirectionMoveSnake.Y < 0)
            StartPoint.Y -= StepSnake;
        //������ ������ � ����� �������
        SnakeBody[0]->SetRelativeLocation(StartPoint);
    }
}

void AMySnakeActor::HaveDamage()
{
    WhoPawn->SnakeDestroy();
}
