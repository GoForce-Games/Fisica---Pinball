#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module()
{
    
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    

    return true;
}

bool Map::Start() {

    mapTexture = app->tex->Load("Assets/Textures/RbCT6y.png");

    return true;
}

bool Map::Update(float dt)
{
    app->render->DrawTexture(mapTexture, 0, 0, NULL);
    
    return true;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

	
    return true;
}
