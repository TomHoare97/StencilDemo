#include "Globals.h"


std::vector<Scene> scenes;


void CreateScene(std::string name)
{
	//for the love of bytes do not load before materials/textures



}
struct LightObject
{
	std::vector<Task*> tasks;
	Light light;

	void Update(float totalTime, float dTime)
	{
		for (Task* task : tasks)
		{
			task->Update(totalTime, dTime);
		};
	}

};
struct GameObject
{
	std::vector<Task*> tasks;
	std::vector<RenderItem> bodyparts;

	void Update(float totalTime, float dTime)
	{
		for (Task* task : tasks)
		{
			task->Update(totalTime, dTime);
		};
		if (tasks.size() > 0)
		{
			for (RenderItem body : bodyparts)
				body.NumFramesDirty = 3;
		}
	}
};


struct Scene
{
	bool updateInBackGround = false;

	std::vector<LightObject> directional;
	std::vector<LightObject> point;
	std::vector<LightObject> spot;

	std::vector<GameObject*> gameObjects;
	std::vector<GameObject> lights;

	GameObject* AddGameObject(GameObject* object)
	{
		gameObjects.push_back(object);

		return(object);
	}

	void AddPointLight()
	{

	}
	
	void AddDirectionLight()
	{

	}

	void AddSpotLight()
	{
		
	}

	void Update(float totalTime, float dTime)
	{
		for(auto e: gameObjects)
		{
		e->Update(totalTime, dTime);
		
		}
	}
	XMFLOAT3 LightCount()
	{

		return { 1,1,1 };
	}

	std::vector<Light> GetLights()
	{
		std::vector<Light> lights;
		for (auto e : directional)
			lights.push_back(e.light);
		for (auto e : point)
			lights.push_back(e.light);
		for (auto e : spot)
			lights.push_back(e.light);
		return lights;
	}
};


void CreateScenes()
{
	Scene* baseScene = scenes.push_back(new Scene);


	GameObject* thing;

	GameObject* flame = baseScene->AddGameObject(thing);
}

























class Task
{
public:
	virtual void Update(float totalTime, float dTime) = 0;

};


class MoveToPosition:public virtual Task
{
	MoveToPosition(XMFLOAT3* currentPosition, XMFLOAT3 targetPosition)
	{
		currentPosition = &targetPosition;
		delete(this);
	}

};

class FollowObject : public Task
{

	XMFLOAT3* currentPosition;
	GameObject* target;

	FollowObject(XMFLOAT3* currentPosition, GameObject* target)
	{
		this->currentPosition = currentPosition;
		this->target = target;
	}

	void Update(float totalTime, float dTime)
	{
		if (this->currentPosition == nullptr || target == nullptr)
			delete(this);
		else
			this->currentPosition = &this->target->bodyparts.front().Position;

	}

};