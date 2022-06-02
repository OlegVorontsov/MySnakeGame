// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusLife.h"
#include "MyPawnCamera.h"

DEFINE_LOG_CATEGORY_STATIC(BonusLifeLog, All, All);

// Sets default values
ABonusLife::ABonusLife()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //������� root ��������� ��� ������
    MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootBonus"));
    //��������� root ���������
    RootComponent = MyRootComponent;

    //����� ����������� ����� ������
    BonusLifeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

    //������� ��������� ��� ������
    BonusLifeChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus"));

    //��������� ����� ������ � ���������
    BonusLifeChank->SetStaticMesh(BonusLifeMesh);

    //��������� ���� ��� ������ �� ������
    BonusLifeColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "M_Tech_Hex_Tile_Inst.M_Tech_Hex_Tile_Inst'"))
                         .Get();

    //������ ��������� ������� ��� ������
    BonusLifeChank->SetRelativeLocation(FVector(0, 0, 0));
    //������ ���� ������
    BonusLifeChank->SetMaterial(0, BonusLifeColor);
    //������ ����� � root
    BonusLifeChank->SetupAttachment(MyRootComponent);
    //�������� ��������� ������
    BonusLifeChank->SetSimulatePhysics(true);

    //��������� ������� �� ��������
    BonusLifeChank->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BonusLifeChank->SetCollisionObjectType(ECC_WorldDynamic);
    BonusLifeChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called when the game starts or when spawned
void ABonusLife::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABonusLife::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeBufer += DeltaTime;
    if (TimeBufer > StepDelay)
    {
        Destroy(true, true);
    }
    CollectBonusLife();
}

//������� ����� ������
void ABonusLife::CollectBonusLife()
{
    //�������� ������ ���� ����� �������� ���� � ��� �����������
    TArray<AActor*> CollectedActors;
    //���������� ���� ������� � ��� �����������
    GetOverlappingActors(CollectedActors);

    //���������� ������ � ����������� �������� ������
    for (int32 i = 0; i < CollectedActors.Num(); ++i)
    {
        AMySnakeActor* const Test = Cast<AMySnakeActor>(CollectedActors[i]);
        if (Test)
        {
            //���������� ����� ������
            Test->Lives++;

            //����������� ���������� �������� �������
            Test->BonusLivesCount++;

            //���� ��������� ������
            Test->OnGetBonusSound->Play();

            //���������� �����
            Destroy(true, true);
            break; // ������������� ���� ��� ��� ����� �����
        }
    }
}
