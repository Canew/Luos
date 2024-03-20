


#include "GameplayActor/LuosProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LuosProjectile)

ALuosProjectile::ALuosProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionProfileName(TEXT("LuosProjectile"));
	RootComponent = ProjectileCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetGenerateOverlapEvents(false);
	ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ProjectileMesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
	ProjectileMovement->Velocity = FVector::ZeroVector;
}

void ALuosProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ProjectileBeginOverlapCallback);
}

void ALuosProjectile::AddImpactGameplayEffectSpec(FGameplayEffectSpecHandle ImpactGameplayEffectSpec)
{
	if (ImpactGameplayEffectSpec.Data)
	{
		ImpactGameplayEffectSpecs.Add(ImpactGameplayEffectSpec);
	}
}

void ALuosProjectile::ProjectileBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !IsValid(OtherActor) || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	bool bShouldApplyEffect = true;
	IGenericTeamAgentInterface* OwnerTeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetOwner());
	if (OwnerTeamAgentInterface)
	{
		ETeamAttitude::Type TeamAttitude = OwnerTeamAgentInterface->GetTeamAttitudeTowards(*OtherActor);
		if (TeamAttitude == ETeamAttitude::Friendly)
		{
			bShouldApplyEffect = false;
		}
	}

	if (!bShouldApplyEffect)
	{
		return;
	}

	UAbilitySystemComponent* OwnerAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	UAbilitySystemComponent* OtherAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);

	if (OwnerAbilitySystemComponent)
	{
		if (OtherAbilitySystemComponent)
		{
			for (FGameplayEffectSpecHandle ImpactGameplayEffectSpec: ImpactGameplayEffectSpecs)
			{
				OwnerAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*ImpactGameplayEffectSpec.Data, OtherAbilitySystemComponent);
			}
		}

		if (ImpactGameplayCueTag.IsValid())
		{
			FGameplayCueParameters GameplayCueParameters;
			GameplayCueParameters.EffectContext = OwnerAbilitySystemComponent->MakeEffectContext();
			GameplayCueParameters.Location = (bFromSweep ? SweepResult.ImpactPoint : FVector_NetQuantize(GetActorLocation()));
			GameplayCueParameters.Normal = SweepResult.ImpactNormal;
			GameplayCueParameters.PhysicalMaterial = SweepResult.PhysMaterial;
			OwnerAbilitySystemComponent->ExecuteGameplayCue(ImpactGameplayCueTag, GameplayCueParameters);
		}
	}

	Destroy();
}
