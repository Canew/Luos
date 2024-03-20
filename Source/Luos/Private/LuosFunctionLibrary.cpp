


#include "LuosFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbility/LuosAbilitySystemComponent.h"

bool ULuosFunctionLibrary::ConeOverlapActors(const UObject* WorldContextObject, const FVector ConePos, float ConeRadius, const FVector ConeDirection, float ConeHalfAngleDegree, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ActorClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class AActor*>& OutActors)
{
	OutActors.Empty();

	TArray<UPrimitiveComponent*> OverlapComponents;
	bool bOverlapped = ConeOverlapComponents(WorldContextObject, ConePos, ConeRadius, ConeDirection, ConeHalfAngleDegree, ObjectTypes, NULL, ActorsToIgnore, OverlapComponents);
	if (bOverlapped)
	{
		UKismetSystemLibrary::GetActorListFromComponentList(OverlapComponents, ActorClassFilter, OutActors);
	}

	return (OutActors.Num() > 0);
}

bool ULuosFunctionLibrary::ConeOverlapComponents(const UObject* WorldContextObject, const FVector ConePos, float ConeRadius, const FVector ConeDirection, float ConeHalfAngleDegree, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class UPrimitiveComponent*>& OutComponents)
{
	OutComponents.Empty();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SectorOverlapComponents), false);
	Params.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;

	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		ObjectParams.AddObjectTypesToQuery(Channel);
	}


	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr)
	{
		World->OverlapMultiByObjectType(Overlaps, ConePos, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(ConeRadius), Params);
	}

	for (int32 OverlapIdx = 0; OverlapIdx < Overlaps.Num(); ++OverlapIdx)
	{
		FOverlapResult const& O = Overlaps[OverlapIdx];
		if (O.Component.IsValid())
		{
			if (!ComponentClassFilter || O.Component.Get()->IsA(ComponentClassFilter))
			{
				if (!ConeDirection.IsZero() && !(O.Component->GetComponentLocation() - ConePos).IsZero())
				{
					float DotAngleRadian = FMath::Acos(ConeDirection.CosineAngle2D((O.Component->GetComponentLocation() - ConePos).GetSafeNormal()));
					float DotAngleDegree = FMath::RadiansToDegrees(DotAngleRadian);
					if (DotAngleDegree < ConeHalfAngleDegree)
					{
						OutComponents.Add(O.Component.Get());
					}
				}
			}
		}
	}

	return (OutComponents.Num() > 0);
}

float ULuosFunctionLibrary::GetSetByCallerMagnitude(const FGameplayEffectSpecHandle& EffectSpecHandle, FGameplayTag DataTag, bool WarnIfNotFound, float DefaultIfNotFound)
{
	if (EffectSpecHandle.Data)
	{
		return EffectSpecHandle.Data->GetSetByCallerMagnitude(DataTag, WarnIfNotFound, DefaultIfNotFound);
	}

	return DefaultIfNotFound;
}

bool ULuosFunctionLibrary::CanApplyGameplayEffect(const FGameplayEffectSpecHandle& EffectSpecHandle, const FGameplayAbilityTargetDataHandle& TargetData)
{
	for (TSharedPtr<FGameplayAbilityTargetData> Data : TargetData.Data)
	{
		if (Data.IsValid())
		{
			for (TWeakObjectPtr<AActor>& TargetActor : Data->GetActors())
			{
				ULuosAbilitySystemComponent* TargetAbilitySystemComponent = Cast<ULuosAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor.Get()));
				if (TargetAbilitySystemComponent)
				{
					return TargetAbilitySystemComponent->CanApplyGameplayEffect(EffectSpecHandle);
				}
			}
		}
	}

	return false;
}
