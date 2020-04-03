// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonBase.h"

// Sets default values
ASummonBase::ASummonBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	summonBT = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
	sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sense"));
	sense->SetPeripheralVisionAngle(60.0f);
	sense->bOnlySensePlayers = false;
	attackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	attackHitBox->SetupAttachment(RootComponent);
	attackHitBox->SetCollisionProfileName("ZombieProjectile");
	aiController = nullptr;
	target = nullptr;
	animInstance = nullptr;
}

// Called when the game starts or when spawned
void ASummonBase::BeginPlay()
{
	Super::BeginPlay();
	animInstance = Cast<USummonAnimInstance>(GetMesh()->GetAnimInstance());
	aiController = Cast<AEnemyAIController>(GetController());
	
	if (sense)
	{
		sense->OnSeePawn.AddDynamic(this, &ASummonBase::OnSeeEnemy);
		UE_LOG(LogTemp, Warning, TEXT("Sense bound"));
	}
	if (attackHitBox)
	{
		attackHitBox->OnComponentBeginOverlap.AddDynamic(this, &ASummonBase::OnAttackBoxOverlap);
		attackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetWorld()->GetTimerManager().SetTimer(dieTimer, this, &ASummonBase::Die, timeToDie, false);
}

// Called every frame
void ASummonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASummonBase::OnSeeEnemy(APawn* pawn_)
{
	if (bActive) //This is to prevent the enemy from doing AI logic before finishing the summon animation
	{
		 if (target && aiController)
		{
			 UE_LOG(LogTemp, Warning, TEXT("We already have a target...Apparently %s"), *target->GetName());
			if (target->IsDead() || target->bZombie) //If the target is dead or a zombie, look for another
			{
				 target = nullptr;
			}

			if(target!=aiController->GetPlayer()) //Check if the target has changed
				aiController->SetSeenTarget(target);

			if (GetDistanceToEnemy() <= attackDistance && !bIsReloading) //Check if we're close enough to attack
			{
				Attack();
			}
		}
		else if (!target)
		{
			target = Cast<AEnemyBase>(pawn_);
			UE_LOG(LogTemp, Warning, TEXT("Enemy has been seen"));
		}

	}
}

void ASummonBase::TakeDamage(float damage_)
{
	hp -= damage_;
	if (hp <= 0.5f)
	{
		Die(); //Enable death animation
	}
}
void ASummonBase::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
	//Call Attack Animation
	if (animInstance)
	{
		animInstance->NextAttack();
	}

	if (aiController)
	{
		aiController->SetAttack(true);
		aiController->SetReload(false);
	}
}

void ASummonBase::EnableAttackHitBox()
{
	if (attackHitBox)
	{
		attackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ASummonBase::DisableAttackHitBox()
{
	if (attackHitBox)
	{
		attackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &ASummonBase::EndReload, reloadTime, false);
		bIsReloading = true;
	}

	if (aiController)
	{
		aiController->SetAttack(false);
		aiController->SetReload(true);
	}
}

void ASummonBase::EndReload()
{
	//Disable reload in animation and in BT
	bIsReloading = false;
	if (animInstance)
	{
		animInstance->ResetReload();
	}
	if (aiController)
	{
		aiController->SetReload(false);
	}
	UE_LOG(LogTemp, Warning, TEXT("ReloadEnd"));
}

void ASummonBase::OnAttackBoxOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		//UGameplayStatics::SpawnDecalAttached(decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetRootComponent(),NAME_None,sweepResult_.ImpactPoint,sweepResult_.Normal.Rotation());


		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
			if (!enemy->IsDead() && !enemy->bZombie)
			{
				enemy->TakeSpellDamage(baseDamage * damageModifier);
				if (impact)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

			}
		}
	}
}

void ASummonBase::Die()
{
	bActive = false;
	if (animInstance)
	{
		animInstance->Death();
	}
	if (aiController)
	{
		aiController->SetDead();
	}
}

void ASummonBase::LeaveScene()
{
	//Called from animation after death animation

	switch (level) //At level 2 and 3, summons leave behind objects (2: miasma cloud, 3: Havn't decided yet)
	{
	case 2:
		if (deathObjects[0])
		{
			GetWorld()->SpawnActor<ASummonBase>(deathObjects[0], GetActorLocation(), FRotator::ZeroRotator);
		}
		break;
	case 3:
		if (deathObjects[1])
		{
			GetWorld()->SpawnActor<ASummonBase>(deathObjects[1], GetActorLocation(), FRotator::ZeroRotator);
		}
		break;
	default:
		break;
	}

	GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &ASummonBase::DestroySummon, 0.5f, false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ASummonBase::GetDistanceToEnemy()
{
	if (aiController)
		return ((aiController->GetPlayerLocation() - GetActorLocation()).Size());
	else
		return 10000.0f; 
}

void ASummonBase::SetSummonLevel(int level_)
{
	//Called from the conjuror
	//Level depends on player base damage
	//Level affects the damage modifier
	level = level_;

	if (level == 2)
		damageModifier = 1.5f;
	if (level == 3)
		damageModifier = 2.0f;
}

void ASummonBase::ActivateSummon()
{
	bActive = true;
	UE_LOG(LogTemp, Warning, TEXT("Active"));
}

void ASummonBase::DestroySummon()
{
	Destroy();
}