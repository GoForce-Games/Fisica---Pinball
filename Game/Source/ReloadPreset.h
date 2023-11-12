#pragma once
#include "SString.h"
#include "List.h"

class Module;

struct ReloadPreset {
	SString name;
	List<Module*> unload;
	List<Module*> load;
	//Fade out time in seconds
	float fadeOut;
	//Fade in time in seconds
	float fadeIn;

	ReloadPreset() : name("nameless"), fadeOut(0.0f), fadeIn(0.0f)
	{}

	ReloadPreset(const SString& _name, const float _fadeOut, float _fadeIn) : name(_name), fadeOut(_fadeOut), fadeIn(_fadeIn)
	{}

	bool AddReload(Module* m) {
		AddUnload(m);
		AddLoad(m);
		return true;
	}
	bool AddUnload(Module* m) {
		bool ret = true;
		if (unload.Find(m) == -1)
			unload.Add(m);
		else ret = false;
		return ret;
	}
	bool AddLoad(Module* m) {
		bool ret = true;
		if (load.Find(m) == -1)
			load.Add(m);
		else ret = false;
		return ret;
	}
};