#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;

class Module
{
public:

	Module(bool startEnabled = true) : active(startEnabled)
	{}

	void Init()
	{
		// active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		awoken = true;
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		started = true;
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		started = false;
		if (needsAwaking)
			awoken = false;
		return true;
	}


	virtual bool Enable()
	{
		if (!active)
		{
			Start();
			active = true;
		}
		return active;
	}

	virtual bool Disable()
	{
		if (active)
		{
			CleanUp();
			active = false;
			if (needsAwaking)
				awoken = false;
		}
		return !active;
	}

public:

	SString name;
	bool active;

	bool needsAwaking = false;

	bool awoken = false;
	bool started = false;

};

#endif // __MODULE_H__