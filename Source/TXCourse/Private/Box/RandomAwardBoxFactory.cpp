// Fill out your copyright notice in the Description page of Project Settings.


#include "Box/RandomAwardBoxFactory.h"

#include "Box/RandomAwardBox.h"


URandomAwardBoxFactory::URandomAwardBoxFactory()
{
	ReadXml();
}

void URandomAwardBoxFactory::SpawnAwardBox()
{
	if(GWorld)
	{
		ARandomAwardBox* AwardBoxInst = GWorld->SpawnActor<ARandomAwardBox>();
		if(AwardBoxInst==nullptr) return;
		int RandomIndex = FMath::RandRange(0,AwardBoxInfos.Num());
		FAwardBoxStruct* RandomBoxInfo = AwardBoxInfos[RandomIndex];
		AwardBoxInst->InitBox(RandomBoxInfo->AwardType,RandomBoxInfo->ObjectName,RandomBoxInfo->Capacity,RandomBoxInfo->Weight,RandomBoxInfo->Order);
	}
}

void URandomAwardBoxFactory::SpawnAwardBox(FTransform Transform)
{
	if(GWorld)
	{
		ARandomAwardBox* AwardBoxInst = GWorld->SpawnActor<ARandomAwardBox>();
		if(AwardBoxInst==nullptr) return;
		AwardBoxInst->SetActorTransform(Transform);
		int RandomIndex = FMath::RandRange(0,AwardBoxInfos.Num()-1);
		FAwardBoxStruct* RandomBoxInfo = AwardBoxInfos[RandomIndex];
		AwardBoxInst->InitBox(RandomBoxInfo->AwardType,RandomBoxInfo->ObjectName,RandomBoxInfo->Capacity,RandomBoxInfo->Weight,RandomBoxInfo->Order);
	}
}

void URandomAwardBoxFactory::SpawnAwardBox(FTransform Transform, FString BoxName)
{
	if(GWorld)
	{
		ARandomAwardBox* AwardBoxInst = GWorld->SpawnActor<ARandomAwardBox>();
		if(AwardBoxInst==nullptr) return;
		AwardBoxInst->SetActorTransform(Transform);
		if(BoxNameIndex.Find(BoxName)==nullptr)
		{
			UE_LOG(LogActor,Warning,TEXT("%s does not exist in awardBox list"),*BoxName);
			return;
		}
		int Index = BoxNameIndex[BoxName];
		FAwardBoxStruct* BoxInfo = AwardBoxInfos[Index];
		AwardBoxInst->InitBox(BoxInfo->AwardType,BoxInfo->ObjectName,BoxInfo->Capacity,BoxInfo->Weight,BoxInfo->Order);
	}
}

void URandomAwardBoxFactory::ReadXml()
{
	FString filePath = FPaths::Combine(FPaths::ProjectContentDir(),FString("AwardBox"),FString("AwardBox.xml"));
	FXmlFile*xml = new FXmlFile();
	xml->LoadFile(filePath);
	check(xml->IsValid());
	FXmlNode* RootNode = xml->GetRootNode();
	TArray<FXmlNode*> XmlNodes = RootNode->GetChildrenNodes();
	for(FXmlNode* Node : XmlNodes)
	{
		FAwardBoxStruct* AwardBoxInfo = new FAwardBoxStruct();
		BoxNameIndex.Add(Node->FindChildNode("BoxName")->GetContent(),AwardBoxInfos.Num());
		AwardBoxInfo->Capacity = FCString::Atoi(*(Node->FindChildNode("Capacity")->GetContent()));
		AwardBoxInfo->AwardType = FCString::Atoi(*(Node->FindChildNode("AwardType")->GetContent()));
		AwardBoxInfo->ObjectName = Node->FindChildNode("ObjectName")->GetContent();
		AwardBoxInfo->Weight = FCString::Atoi(*(Node->FindChildNode("Weight")->GetContent()));
		AwardBoxInfo->Order = FCString::Atoi(*(Node->FindChildNode("Order")->GetContent()));
		AwardBoxInfos.Push(AwardBoxInfo);
	}
}
