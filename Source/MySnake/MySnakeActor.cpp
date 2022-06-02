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

    //создаем компонент для звука поедания яблока
    OnEatAppleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("EatAudio"));
    OnEatAppleSound->bAutoActivate = false;
    OnEatAppleSound->SetupAttachment(MyRootComponent);
    static ConstructorHelpers::FObjectFinder<USoundBase> soundBaseapple(
        TEXT("SoundWave'/Game/SnakeContent/Sounds/hrum_jabloko.hrum_jabloko'"));
    OnEatAppleSound->SetSound(soundBaseapple.Object);

    //создаем компонент для звука получения бонуса
    OnGetBonusSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BonusAudio"));
    OnGetBonusSound->bAutoActivate = false;
    OnGetBonusSound->SetupAttachment(MyRootComponent);
    static ConstructorHelpers::FObjectFinder<USoundBase> soundBasebonus(
        TEXT("SoundWave'/Game/SnakeContent/Sounds/BonusGet.BonusGet'"));
    OnGetBonusSound->SetSound(soundBasebonus.Object);

    //создаем компонент для звука появления змейки
    SnakeAppearSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SnakeAppearAudio"));
    SnakeAppearSound->bAutoActivate = false;
    SnakeAppearSound->SetupAttachment(MyRootComponent);
    static ConstructorHelpers::FObjectFinder<USoundBase> soundBaseAppear(
        TEXT("SoundWave'/Game/SnakeContent/Sounds/poyavlenie_personaga.poyavlenie_personaga'"));
    SnakeAppearSound->SetSound(soundBaseAppear.Object);

    CreateSnakeBody();
    //подписываемся на делегат попадания в Component
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
    //двигаемся если буфер переполнился
    if (TimeBufer > StepDelay)
    {
        MoveSnake();
        //обнуляем буфер
        TimeBufer = 0;
    }
}

//функция создания тела змейки
void AMySnakeActor::CreateSnakeBody()
{
    //берем стандартную сферу движка
    class UStaticMesh* SnakeChankMesh;
    SnakeChankMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

    //берем материалы созданные в движке для тела и головы
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

    //переменная для расчета координат змейки
    FVector NextPoint = GetActorLocation();

    //объявляем название для уникальной части змейки
    FName NameChank;
    //переменная для конверта имени
    FString TheString;

    //создаем части змейки
    for (int32 i = 0; i < SnakeSize; i++)
    {
        //собираем уникальное имя
        TheString = "Chanks" + FString::FromInt(i);

        //заполняем имя преобразуя данные в текст
        NameChank = FName(*TheString);

        //создаем компонент части змейки
        class UStaticMeshComponent* BodyChank = CreateDefaultSubobject<UStaticMeshComponent>(NameChank);

        //присваиваем компонент части змейки сфере
        BodyChank->SetStaticMesh(SnakeChankMesh);

        //задаем координаты части змейки
        BodyChank->SetRelativeLocation(NextPoint);

        //добавляем части змейки в массив
        SnakeBody.Add(BodyChank);

        //смещение по оси Х
        NextPoint.X -= StepSnake;

        //добавляем в корень модели
        BodyChank->SetupAttachment(MyRootComponent);

        //задаем цвет части змейки
        BodyChank->SetMaterial(0, BodyColor);

        //задаем цвет голове змейки и добавляем реакцию на коллизии
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
            //вкоючаем реакцию на коллизии у всего
            BodyChank->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            BodyChank->SetCollisionObjectType(ECC_WorldStatic);
            BodyChank->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
            // BodyChank->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
    //прячем элементы змейки не входящие в диапазон
    SetVisibleChank();

    SnakeAppearSound->Play();
}

//функция попадания в TriggerSphere
void AMySnakeActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //проверяем что в TriggerSphere попала сама змейка
    if (OtherActor && (OtherActor == this) && OtherComp)
    {
        //если попал видимый диапазон с 4 и до хвоста, наносим урон
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
        //если попало яблоко. поедание бонусов работает через GetOverlappingActors в них самих
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

//функция делает видимыми части змейки
void AMySnakeActor::SetVisibleChank()
{
    //перебираем все части змейки
    for (int32 IndexShow = 0; IndexShow < SnakeBody.Num(); IndexShow++)
    {
        //если часть змейки входит в видиммый диапазон то показываем ее
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

//функция движения змейки по координатам
void AMySnakeActor::MoveSnake()
{
    //если есть движение то продолжаем, если нет стоим
    if ((DirectionMoveSnake.X != 0) || (DirectionMoveSnake.Y != 0))
    {
        //двигаем все элементы змейки с хвоста
        for (int Chank = SnakeBody.Num() - 1; Chank > 0; Chank--)
        {
            //получаем координаты предыдущей части и записываем в переменную V
            FVector V = SnakeBody[Chank - 1]->GetRelativeLocation();
            //встаем на новые координаты
            SnakeBody[Chank]->SetRelativeLocation(V);
        }
        //получаем координаты первой точки
        FVector StartPoint = SnakeBody[0]->GetRelativeLocation();

        //если на правление по Х > 0 то двигаем змейку в одну сторону
        if (DirectionMoveSnake.X > 0)
            StartPoint.X -= StepSnake;
        if (DirectionMoveSnake.X < 0)
            StartPoint.X += StepSnake;
        //двигаем в другую сторону
        if (DirectionMoveSnake.Y > 0)
            StartPoint.Y += StepSnake;
        if (DirectionMoveSnake.Y < 0)
            StartPoint.Y -= StepSnake;
        //ставим голову в новую позицию
        SnakeBody[0]->SetRelativeLocation(StartPoint);
    }
}

void AMySnakeActor::HaveDamage()
{
    WhoPawn->SnakeDestroy();
}
