#pragma once
#include "Module.h"
#include "List.h"
#include "ReloadPreset.h"
#include "Timer.h"
#include "SDL/include/SDL_rect.h"

enum ReloadStep {
    NONE,
    FADE_OUT,
    RELOAD,
    FADE_IN
};

class Reload : public Module
{
public:
    Reload();

    // Destructor
    virtual ~Reload();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    bool StartReload(SString presetName);

private:

    void FadeOut();

    void ReloadModules();

    void FadeIn();

public:

    ReloadPreset* activePreset = nullptr;
    ReloadStep currentStep = NONE;

    List<ReloadPreset*> presetList;

    List<Module*> moduleList;

    bool timerActive = false;
    Timer timer;

    float fadeRatio = 0.0f;

    SDL_Rect screenRect;

};

