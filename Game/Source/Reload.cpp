#include "Reload.h"
#include "App.h"

#include "Box2D/Box2D/Box2D.h"
#include "Log.h"

Reload::Reload() {

}

Reload::~Reload() {

}

bool Reload::Awake(pugi::xml_node& conf)
{
	// TODO Añadir los módulos a recargar desde el xml
	//moduleList.Add((Module*)app->tex);
	//moduleList.Add((Module*)app->audio);
	//moduleList.Add((Module*)app->physics);
	//moduleList.Add((Module*)app->scene);
	//moduleList.Add((Module*)app->map);
	//moduleList.Add((Module*)app->intro);
	//moduleList.Add((Module*)app->entityManager);
	ReloadPreset* presetMap = new ReloadPreset("map", 2, 2);
	presetMap->AddUnload((Module*)app->intro);
	presetMap->AddLoad((Module*)app->map);
	presetMap->AddLoad((Module*)app->scene);
	presetMap->AddLoad((Module*)app->entityManager);
	presetMap->AddLoad((Module*)app->physics);
	presetMap->AddLoad((Module*)app->puntuation);

	presetList.Add(presetMap);

	ReloadPreset* presetLose = new ReloadPreset("lose", 2, 2);
	presetLose->AddUnload((Module*)app->lose);
	presetLose->AddLoad((Module*)app->intro);

	
	ReloadPreset* gameOver = new ReloadPreset("gameToGameOver", 2, 2);
	gameOver->AddUnload((Module*)app->map);
	gameOver->AddUnload((Module*)app->scene);
	gameOver->AddUnload((Module*)app->entityManager);
	gameOver->AddUnload((Module*)app->physics);

	presetList.Add(gameOver);

	return true;
}

bool Reload::Start()
{
	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	screenRect = app->render->camera;
	screenRect.x = screenRect.y = 0;

	return true;
}

bool Reload::PostUpdate()
{


	if (activePreset != nullptr) {

		// Steps in order: fade out -> reload modules -> fade in -> finished
		switch (currentStep)
		{
		case FADE_OUT: {
			FadeOut();
			break;
		}
		case RELOAD: {
			ReloadModules();
			break;
		}
		case FADE_IN: {
			FadeIn();
			break;
		}
		default:
			break;
		}
		//LOG("fadeRatio: %f", fadeRatio);
		//LOG("Alpha: %i", (uint8)(fadeRatio * 255.0f));
		app->render->DrawRectangle(screenRect, 0, 0, 0, (uint8)(fadeRatio*255.0f), true, false);
	}

	return true;
}

bool Reload::CleanUp()
{
	for (ListItem<ReloadPreset*>* item = presetList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			RELEASE(item->data);
		}
	}
	presetList.Clear();

	return true;
}

bool Reload::StartReload(SString presetName)
{
	bool ret = false;
	if (activePreset == nullptr) {

		for (size_t i = 0; i < presetList.Count(); i++)
		{
			ListItem<ReloadPreset*>* item = presetList.At(i);
			if (item->data->name == presetName) {
				ret = true;
				activePreset = item->data;
				currentStep = ReloadStep::FADE_OUT;
			}
		}
	}

	return true;
}

void Reload::FadeOut()
{
	if (!timerActive) {
		timer.Start();
		timerActive = true;
	}
	else {
		fadeRatio = (timer.ReadMSec()/1000.0f) / activePreset->fadeOut;
		fadeRatio = b2Clamp(fadeRatio, 0.0f, 255.0f);
		if (fadeRatio >= 1.0f) {
			activePreset->fadeOut;
			timerActive = false;
			currentStep = ReloadStep::RELOAD;
		}
	}
}

void Reload::ReloadModules()
{
	

	for (ListItem<Module*>* item = activePreset->unload.end; item != nullptr; item = item->prev)
	{
		if (item->data != nullptr) item->data->Disable();
	}

	// Algunos modulos requieren recargar la configuración al reiniciarse
	for (ListItem<Module*>* item = activePreset->load.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr && item->data->needsAwaking && !item->data->awoken) {
			pugi::xml_node config = app->GetConfig(*(item->data));
			item->data->Awake(config);
		}
	}

	for (ListItem<Module*>* item = activePreset->load.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) item->data->Enable();
	}

	currentStep = ReloadStep::FADE_IN;
}

void Reload::FadeIn()
{
	if (!timerActive) {
		timer.Start();
		timerActive = true;
	}
	else {
		fadeRatio = 1- ((timer.ReadMSec() / 1000.0f) / activePreset->fadeIn);
		fadeRatio = b2Clamp(fadeRatio, 0.0f, 255.0f);
		if (fadeRatio <= b2_epsilon) {
			activePreset = nullptr;
			timerActive = false;
			currentStep = ReloadStep::NONE;
			fadeRatio = 0.0f;
		}
	}
}
