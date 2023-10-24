#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>


#define PIXELS_PER_METER (20.0f) // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL (1.0f / PIXELS_PER_METER) // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) std::floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)


// Class: Ball object (a simple stoopid physics object)
class PhysBall
{
public:
	// Position
	// You could also use an array/vector
	float startX;
	float startY;
	float x;
	float y;

	// Velocity
	float vx;
	float vy;

	// Acceleration
	float ax;
	float ay;

	// Force (total) applied to the ball
	float fx;
	float fy;

	// Mass
	float mass;

	// Aerodynamics stuff
	float surface; // Effective wet surface
	float cl; // Aerodynamic Lift coefficient
	float cd; // Aerodynamic Drag coefficient
	float b; // Hydrodynamic Drag coefficient

	// Coefficients of friction & restitution (for bounces)
	float coef_friction;
	float coef_restitution;

	// Shape
	float radius;

	// Has physics enabled?
	bool physics_enabled = true;

	// Has this ball been created or is placeholder?
	bool exists = false;
};

// Class: Ground
class Ground : public SDL_Rect
{
public:
	float x,y,w,h; // Geometry (x,y,w,h)
	SDL_Rect pixels(); // Convert geometry to pixels to draw w/ SDL
};

// Class: Water
class Water : public Ground
{
public:
	float density; // Density of fluid
	float vx; // Velocity x
	float vy; // Velocity y
};

// Class: Atmosphere
class Atmosphere
{
public:
	float density; // Density of air
	float windx; // Wind x
	float windy; // Wind y
};



class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// Physics objects
	std::vector<PhysBall> balls{};
	Atmosphere atmosphere{};
	//Ground ground{};
	Water water{};

	std::vector<Ground> blocks{};

	// Misc
	float dt = 1.0 / 60.0;

private:

	bool debug;
	void (*currentIntegrator)(PhysBall&, float);

};

// Compute modulus of a vector
float modulus(float vx, float vy);

// Compute Aerodynamic Drag force
void compute_aerodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Atmosphere& atmosphere);

// Compute Hydrodynamic Drag force
void compute_hydrodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Water& water);

// Compute Hydrodynamic Buoyancy force
void compute_hydrodynamic_buoyancy(float& fx, float& fy, const PhysBall& ball, const Water& water);

// Integration scheme: Velocity Verlet
void integrator_velocity_verlet(PhysBall& ball, float dt);

//Integration scheme: Euler implicit
void integrator_velocity_euler_implicit(PhysBall& ball, float dt);

//Integration scheme: Euler simplectic
void integrator_velocity_euler_simplectic(PhysBall& ball, float dt);

// Detect collision with ground
collision_state is_colliding_with_ground_unoptimized(const PhysBall& ball, const Ground& ground);

bool is_colliding_with_ground(const PhysBall& ball, const Ground& ground);
// Detect collision with water
bool is_colliding_with_water(const PhysBall& ball, const Water& water);

// Detect collision between circle and rectange
collision_state check_collision_circle_rectangle_unoptimized(float cx, float cy, float cr, float rx, float ry, float rw, float rh);

bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh);

