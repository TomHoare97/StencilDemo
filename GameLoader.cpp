
#include "GameLoader.h"

RenderItem* floord;

Scene* BuildScene()
{
	//scenes.push_back(new Scene);
	return nullptr;
}

void CreateScenes()
{
	//Scene* baseScene = BuildScene();
}

void Load()
{

	CreateRenderItem("log", "fireGeo", "log", "checkertile", (int)RenderLayer::Opaque);
	floord = CreateRenderItem("floor", "roomGeo", "floor", "checkertile", (int)RenderLayer::Opaque);
}