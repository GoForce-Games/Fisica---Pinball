#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

//#include "SDL2_ttf/include/SDL_ttf.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//font = TTF_OpenFont("Assets/Fonts/BrokenConsoleBold-Yza7v.ttf", 0);
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	Font = App->fonts->Load("Assets/Fonts/rtype_font.png", lookupTable, 1);

	return ret;
}



// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//SDL_Color f;
	//f.a = 255;
	//f.r = 255;
	//f.g = 255;
	//f.b = 255;
	//textSurface =TTF_RenderUTF8_Shaded(font, "TEST", f, { 0 });
	//textTexture = App->textures->Load(textSurface);
	//App->renderer->Blit(textTexture, 0, 0);
	sprintf_s(dtText, 100, "delta time %f", App->physics->dt);
	sprintf_s(xText, 100, "x pos %f", App->physics->balls[0].x);
	sprintf_s(yText, 100, "y pos %f", App->physics->balls[0].y);
	sprintf_s(vxText, 100, "vx %f", App->physics->balls[0].vx);
	sprintf_s(vyText, 100, "vy %f", App->physics->balls[0].vy);
	sprintf_s(angleText, 100, "angle %f rad", App->player->launch_angle);
	sprintf_s(axText, 100, "ax %f", App->physics->balls[0].ax);
	sprintf_s(ayText, 100, "ay %f", App->physics->balls[0].ay);

		// TODO 3: Blit the text of the score in at the bottom of the screen
	App->fonts->BlitText(500, 250, Font, dtText);
	App->fonts->BlitText(500, 275, Font, xText);
	App->fonts->BlitText(500, 300, Font, yText);
	App->fonts->BlitText(500, 325, Font, vxText);
	App->fonts->BlitText(500, 350, Font, vyText);
	App->fonts->BlitText(500, 375, Font, angleText);
	App->fonts->BlitText(500, 400, Font, axText);
	App->fonts->BlitText(500, 425, Font, ayText);
	
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	if (Font != NULL)
	{
		App->fonts->UnLoad(Font);
		Font = NULL;
	}

	//TTF_CloseFont(font);

	return true;
}