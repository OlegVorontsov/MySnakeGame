// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAppleActor.h"

// Sets default values
AMyAppleActor::AMyAppleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //������� root ��������� ��� ������
    MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootApple"));
    //��������� root ���������
    RootComponent = MyRootComponent;

    //����� ����������� ����� ������
    AppleMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

    //���������� ����� ��� ������
    FVector ScaleApple = FVector(0.5f, 0.5f, 0.5f);

    //������� ��������� ��� ���
    EatChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eat"));

    //��������� ����� ������ � ��������� ���
    EatChank->SetStaticMesh(AppleMesh);

    //��������� ���� ��� ������ �� ������
    AppleColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "M_Metal_Gold_Inst.M_Metal_Gold_Inst'"))
                     .Get();

    //������ ����� ������ ��� ������
    EatChank->SetRelativeScale3D(ScaleApple);
    //������ ��������� ������� ��� ������
    EatChank->SetRelativeLocation(FVector(0, 0, 0));
    //������ ���� ��� ������
    EatChank->SetMaterial(0, AppleColor);
    //������ ������ � root
    EatChank->SetupAttachment(MyRootComponent);
    //�������� ��������� ������
    EatChank->SetSimulatePhysics(true);

    //��������� ������� �� ��������
    EatChank->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EatChank->SetCollisionObjectType(ECC_WorldDynamic);
    EatChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called when the game starts or when spawned
void AMyAppleActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyAppleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeBufer += DeltaTime;
    if (TimeBufer > StepDelay)
    {
        Destroy(true, true);
    }
}
