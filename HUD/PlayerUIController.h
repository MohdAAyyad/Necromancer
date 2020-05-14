// Fill out your copyright notice in the Description page of Project Settings.
//TODO 
//Add a variable to switch visibility of the spells menu
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../SpellsInventory.h"
#include "../Spells.h"
#include "Engine/Canvas.h"
#include "../PlayerStats.h"
#include "PlayerUIController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API UPlayerUIController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerUIController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	UPlayerStats* stats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bGameIsPaused;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bSkillTreeMenuIsShown;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString highlightedSpellName; //When the mouse hovers over the spells in the menu, it should update the name and description
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString highlightedSpellDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bInDialogue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bQuestMenu;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> spellTextures;

	UFUNCTION(BlueprintCallable)
		float UpdateHealth() const; //Returns health percentage to update UI
	UFUNCTION(BlueprintCallable)
		float UpdateBlood() const; //Returns blood percentage to update UI

	UTexture2D* LookForAimTexture(EAimSpells spell_); //Looks through a switch statement to get the correct texture back
	UTexture2D* LookForBloodTexture(EBloodSpells spell_); //Looks through a switch statement to get the correct texture back
	UTexture2D* LookForInnateTexture(EInnateSpells spell_);

	FString dialogue;
	FString dialogueName;
public:
	void SetPlayerStatsPtr(UPlayerStats* stats_) { stats = stats_; };

	UFUNCTION(BlueprintCallable)
		void EquipInnateSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void PauseGame();
	UFUNCTION(BlueprintCallable)
		void SkillTreeMenu();
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetAimTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEquippedAimTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetBloodTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEquippedBloodTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetInnateTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEquippedInnateTexture(); //Called from the UMG to get the texture

	UFUNCTION(BlueprintCallable)
		void RemoveAimSpell(int index_);

	UFUNCTION(BlueprintCallable)
		void RemoveBloodSpell(int index_);

	UFUNCTION(BlueprintCallable)
		void RemoveInnateSpell();

	//Unlock Spells
	UFUNCTION(BlueprintCallable)
	void UnlockAimSpell(int index_);
	UFUNCTION(BlueprintCallable)
	void UnlockBloodSpell(int index_);
	UFUNCTION(BlueprintCallable)
	void UnlockInnateSpell(int index_);

	UFUNCTION(BlueprintCallable)
		bool IsAimSpellUnlocked(int index_);
	UFUNCTION(BlueprintCallable)
		bool IsBloodSpellUnlocked(int index_);
		

	//EXP
	UFUNCTION(BlueprintCallable)
		int GetCurrentSkillPoints();
	UFUNCTION(BlueprintCallable)
		int GetCurrentEXP();
	UFUNCTION(BlueprintCallable)
		int GetEXPToLevelUp();

	//Name And Description

	UFUNCTION(BlueprintCallable)
		FString GetEquippedAimName(int index_); //Returns unlocked spell name
	UFUNCTION(BlueprintCallable)
		FString GetEquippedAimDescription(int index_);//Returns unlocked spell Description
	UFUNCTION(BlueprintCallable)
		FString GetLockedAimName(int index_);//Returns locked spell name
	UFUNCTION(BlueprintCallable)
		FString GetLockedAimDescription(int index_);//Returns locked spell Description

	UFUNCTION(BlueprintCallable)
		FString GetEquippedBloodName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetEquippedBloodDescription(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedBloodName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedBloodDescription(int index_);

	UFUNCTION(BlueprintCallable)
		FString GetInnateName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetInnateDescription(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedInnateName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedInnateDescription(int index_);

	UFUNCTION(BlueprintCallable)
		void SetName(FString name_);
	UFUNCTION(BlueprintCallable)
		void SetDescription(FString description_);


	//Stats
	UFUNCTION(BlueprintCallable)
		int GetBaseDamage();
	UFUNCTION(BlueprintCallable)
		int GetBaseMagicDamage();
	UFUNCTION(BlueprintCallable)
		int GetMaxHP();
	UFUNCTION(BlueprintCallable)
		int GetMaxBP();

	UFUNCTION(BlueprintCallable)
		void AddBaseDamage();
	UFUNCTION(BlueprintCallable)
		void AddMagicDamage();
	UFUNCTION(BlueprintCallable)
		void AddHP();
	UFUNCTION(BlueprintCallable)
		void AddBP();

	UFUNCTION(BlueprintCallable)
		int GetEquippedAimNum();

	UFUNCTION(BlueprintCallable)
		int GetEquippedBloodNum();

	//Dialogue

	UFUNCTION(BlueprintCallable)
		FString GetDialogue();
	UFUNCTION(BlueprintCallable)
		FString GetName();

	void AppendNextChar(TCHAR nchar_);
	void SetEntireDialogue(FString dialogue_); //Used when skipping dialogue
	void SetDialogueName(FString name);
	void ClearDialogue();
	void EndDialogue();

	//Quest
	UFUNCTION(BlueprintCallable)
		FString GetQuestName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetQuestDescription();
	UFUNCTION(BlueprintCallable)
		FString GetActiveQuestName();
	UFUNCTION(BlueprintCallable)
		void SetActiveQuest();
	UFUNCTION(BlueprintCallable)
		bool GetIsQuestCompleted(int index_);
	UFUNCTION(BlueprintCallable)
		void QuestMenu();
	int shownQuest;
	UFUNCTION(BlueprintCallable)
		void SetShownQuest(int index_);
	UFUNCTION(BlueprintCallable)
		bool GetIsShownQuestCompleted();
	UFUNCTION(BlueprintCallable)
		bool IsThisTheShownQuest(); //Compares shown quest with active quest. If they're not equal, set active quest button should appear

	UFUNCTION(BlueprintCallable)
		void GoBack();

	
	UFUNCTION(BlueprintCallable)
		bool GetCompletedQuestPrompt();
	void PlayCompletedQuestPrompt();

	UFUNCTION(BlueprintCallable)
		bool GetNewQuestPrompt();
	void PlayNewQuestPrompt();

	UFUNCTION(BlueprintCallable)
		bool GetUpdatedQuestPrompt();
	void PlayUpdatedQuestPrompt();

		void ResetPlayQuestPrompt();

	float timeToRemoveScreenPrompt;
	FTimerHandle timeToRemoveScreenPromptHandle;
	int numOfQuests;
	int numOfCompletedQuests;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool playCompletedPrompt;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool playNewPrompt;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool playUpdatedPrompt;

	//Level Up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bHasLeveledUp;
	UFUNCTION(BlueprintCallable)
		bool GetHasLeveledUp();
	void PlayLevelUpPrompt();
	void ResetHasLeveledUp();
	FTimerHandle timeToRemoveLevelUpPromptHandle;
};
