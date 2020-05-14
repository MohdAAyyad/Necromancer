// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestNPC.h"

AQuestNPC::AQuestNPC():ANPC()
{
	questIndex = 0;
	questDescription = "A stranger who introduced himself as Kay-Hen asked Leah\nto execute the prisoner held in the Steel Army's camp.\nHis motivation is unclear.";

	//dialogueStageIndex = 0 --> Before giving quest (gives the quest)
	//dialogueStageIndex = 1 --> After giving quest but before quest is done (Reminds player of the quest)
	//dialogueStageIndex = 2 --> After quest is done (Immediate post-quest dialogue)
	//dialogueStageIndex = 3 --> After quest is done (Quest is done, a "thank you" post quest dialogue)
}
void AQuestNPC::GetNextLineAndName(FString& name_, FString& line_)
{
	if (dialogueStageIndex == 1) //If we're in stage 1, then we've given the quest
	{
		if (QuestManager::GetInstance()->IsQuestCompleted(questName)) //Is the quest completed?
		{
			//If yes, move to stage 2
			dialogueStageIndex = 2;
		}
	}
		if (lineIndex < names[dialogueStageIndex].linesArray.Num() && lineIndex < lines[dialogueStageIndex].linesArray.Num())
		{
			name_ = names[dialogueStageIndex].linesArray[lineIndex];
			line_ = lines[dialogueStageIndex].linesArray[lineIndex];
			if (lineIndex == questIndex && dialogueStageIndex == 0) //Is this the line that invokes the quest? And have we not given the quest yet?
			{
				QuestManager::GetInstance()->AddQuest(questName, questDescription);
				dialogueStageIndex = 1; //Move on to the next dialogue stage
			}
			lineIndex++; //Adding 1 to the line index is not a problem even if we get a new stage. This is because "ResetNPC" is called from the dialogue handler once the conversation ends
		}
		//We will go to stage 3 through ResetNPC function
}