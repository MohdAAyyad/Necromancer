// Fill out your copyright notice in the Description page of Project Settings.


#include "NewQuestAutoDialogue.h"
#include "../QuestManager.h"


ANewQuestAutoDialogue::ANewQuestAutoDialogue()
{
	questName = "No longer safe";
	questDescription = "The Ravens had scouts disguised as factory workers.\nTheir last transmission indicated that \nsoldiers from the Steel Army were seen in the vicinity.";
}
void ANewQuestAutoDialogue::GetNextLineAndName(FString& name_, FString& line_)
{
	if (lineIndex < names[dialogueStageIndex].linesArray.Num() && lineIndex < lines[dialogueStageIndex].linesArray.Num())
	{
		name_ = names[dialogueStageIndex].linesArray[lineIndex];
		line_ = lines[dialogueStageIndex].linesArray[lineIndex];
		if (lineIndex == questIndex ) //Is this the line that invokes the quest? And have we not given the quest yet?
		{
			QuestManager::GetInstance()->AddQuest(questName, questDescription);
			dialogueStageIndex = 1; //Move on to the next dialogue stage
		}
		lineIndex++; //Adding 1 to the line index is not a problem even if we get a new stage. This is because "ResetNPC" is called from the dialogue handler once the conversation ends
	}
}