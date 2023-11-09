// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/AnimNoteAttack.h"
#include "Weapon/WeaponBase.h"
#include "Character/PlayerCharacter.h"

void UAnimNoteAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if(Character)
	{
		Character->weapon_r->StartAttack();
	}
}

void UAnimNoteAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if(Character)
	{
		Character->weapon_r->EndAttack();
	}
}
