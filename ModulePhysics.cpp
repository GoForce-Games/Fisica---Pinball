#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <cmath>

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	currentIntegrator = integrator_velocity_verlet;

	// Create ground
	Ground ground = Ground();
	ground.x = 0.0f; // [m]
	ground.y = 0.0f; // [m]
	ground.w = 60.0f; // [m]
	ground.h = 5.0f; // [m]

	blocks.push_back(ground);

	ground.x = 16.0f;
	ground.y = 8.0f;
	ground.w = 4.0f;
	ground.h = 4.0f;

	blocks.push_back(ground);

	ground.x = 8.0f;
	ground.y = 16.0f;
	ground.w = 4.0f;
	ground.h = 4.0f;

	blocks.push_back(ground);

	/*
	// Create Water
	water = Water();
	water.x = ground.x + ground.w; // Start where ground ends [m]
	water.y = 0.0f; // [m]
	water.w = 30.0f; // [m]
	water.h = 5.0f; // [m]
	water.density = 50.0f; // [kg/m^3]
	water.vx = -1.0f; // [m/s]
	water.vy = 0.0f; // [m/s]
	*/

	// Create atmosphere
	atmosphere = Atmosphere();
	atmosphere.windx = 10.0f; // [m/s]
	atmosphere.windy = 5.0f; // [m/s]
	atmosphere.density = 1.0f; // [kg/m^3]

	// Create a ball
	PhysBall ball = PhysBall();

	// Set static properties of the ball
	ball.mass = 10.0f; // [kg]
	ball.surface = 1.0f; // [m^2]
	ball.radius = 0.5f; // [m]
	ball.cd = 0.4f; // [-]
	ball.cl = 1.2f; // [-]
	ball.b = 10.0f; // [...]
	ball.coef_friction = 0.9f; // [-]
	ball.coef_restitution = 0.8f; // [-]

	// Set initial position and velocity of the ball
	ball.x = ball.startX = 1.5f;
	ball.y = ball.startY = (blocks[0].y + blocks[0].h) + 2.5f;
	ball.vx = 0.0f;
	ball.vy = 0.0f;
	ball.physics_enabled = false;
	ball.exists = true;

	// Add ball to the collection
	balls.emplace_back(ball);

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_J) == KEY_DOWN) {
		currentIntegrator = integrator_velocity_verlet;
	}else if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_K) == KEY_DOWN) {
		currentIntegrator = integrator_velocity_euler_implicit;
	}else if (App->input->GetKey(SDL_Scancode::SDL_SCANCODE_L) == KEY_DOWN) {
		currentIntegrator = integrator_velocity_euler_simplectic;
	}


	// Process all balls in the scenario
	for (auto& ball : balls)
	{
		// Skip ball if physics not enabled
		if (!ball.physics_enabled)
		{
			continue;
		}

		// Step #0: Clear old values
		// ----------------------------------------------------------------------------------------
		
		// Reset total acceleration and total accumulated force of the ball
		ball.fx = ball.fy = 0.0f;
		ball.ax = ball.ay = 0.0f;

		// Step #1: Compute forces
		// ----------------------------------------------------------------------------------------

		// Gravity force
		float fgx = ball.mass * 0.0f;
		float fgy = ball.mass * -10.0f; // Let's assume gravity is constant and downwards
		ball.fx += fgx; ball.fy += fgy; // Add this force to ball's total force

		// Aerodynamic Drag force (only when not in water)
		if (!is_colliding_with_water(ball, water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, ball, atmosphere);
			ball.fx += fdx; ball.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(ball, water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, ball, water);
			ball.fx += fhdx; ball.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, ball, water);
			ball.fx += fhbx; ball.fy += fhby; // Add this force to ball's total force
		}

		// Other forces
		// ...

		// Step #2: 2nd Newton's Law
		// ----------------------------------------------------------------------------------------
		
		// SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		ball.ax = ball.fx / ball.mass;
		ball.ay = ball.fy / ball.mass;

		// Step #3: Integrate --> from accel to new velocity & new position
		// ----------------------------------------------------------------------------------------

		// We will use the 2nd order "Velocity Verlet" method for integration.
		currentIntegrator(ball, dt);

		// Step #4: solve collisions
		// ----------------------------------------------------------------------------------------

		for (auto& ground : blocks) {

			// Solve collision between ball and ground

			/* 
				Roger: Aqui estan els canvis que he fet per a col·lisions
				is_colliding_with_ground només té canvis menors,
				com passar la meitat de l'ample i alçada del terra precalculats
			*/
			if (is_colliding_with_ground(ball, ground)) {

				float rw = ground.w / 2.0f; // Half width of rectangle
				float rh = ground.h / 2.0f; // Half height of rectangle
				float rx = (ground.x + rw); // Center of rectangle
				float ry = (ground.y + rh); // Center of rectangle

				float dist_x = std::abs(ball.x - rx);
				float dist_y = std::abs(ball.y - ry);


				int sideX = 0; // -1 = left, 1 = right
				int sideY = 0; // -1 = bottom, 1 = top

				float a = dist_x - rw;
				float b = dist_y - rh;
				float cornerDistance_sq = a * a + b * b;

				float colProportion;
				if (cornerDistance_sq < ball.radius*ball.radius) {
					// Vertex collision: both velocity components may change
					colProportion = 1.0f;
				}
				else
				{
					// Non-vertex collision: only the corresponding velocity component may change (x or y)
					colProportion = abs((dist_x / rw) / (dist_y / rh));
				}

				if (colProportion >= 1.0f) {
					sideX = ball.x < rx ? -1 : 1;
				}
				if (colProportion <= 1.0f) {
					sideY = ball.y < ry ? -1 : 1;
				}

				// SideX is 0 when colliding with only top-bottom
				if (sideX != 0) {
					ball.vx = abs(ball.vx) * sideX;
				}
				// SideY is 0 when colliding with only left-right
				if (sideY != 0) {
					ball.vy = abs(ball.vy) * sideY;
				}

			}

		}
	}

	// Continue game
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	// Colors
	int color_r, color_g, color_b;

	// Draw ground
	color_r = 0; color_g = 255; color_b = 0;
	for (auto& ground: blocks)
	App->renderer->DrawQuad(ground.pixels(), color_r, color_g, color_b);

	// Draw water
	color_r = 0; color_g = 0; color_b = 255;
	App->renderer->DrawQuad(water.pixels(), color_r, color_g, color_b);

	// Draw all balls in the scenario
	for (auto& ball : balls)
	{
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(ball.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(ball.y);
		int size_r = METERS_TO_PIXELS(ball.radius);

		// Select color
		if (ball.physics_enabled)
		{
			color_r = 255; color_g = 255; color_b = 255;
		}
		else
		{
			color_r = 255; color_g = 0; color_b = 0;
		}

		// Draw ball
		App->renderer->DrawCircle(pos_x, pos_y, size_r, color_r, color_g, color_b);
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	return true;
}

// Compute modulus of a vector
float modulus(float vx, float vy)
{
	return std::sqrt(vx * vx + vy * vy);
}

// Compute Aerodynamic Drag force
void compute_aerodynamic_drag(float &fx, float& fy, const PhysBall &ball, const Atmosphere &atmosphere)
{
	float rel_vel[2] = { ball.vx - atmosphere.windx, ball.vy - atmosphere.windy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = 0.5f * atmosphere.density * speed * speed * ball.surface * ball.cd; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Drag force
void compute_hydrodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	float rel_vel[2] = { ball.vx - water.vx, ball.vy - water.vy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = ball.b * speed; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Buoyancy force
void compute_hydrodynamic_buoyancy(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	// Compute submerged area (assume ball is a rectangle, for simplicity)
	float water_top_level = water.y + water.h; // Water top level y
	float h = 2.0f * ball.radius; // Ball "hitbox" height
	float surf = h * (water_top_level - ball.y); // Submerged surface
	if ((ball.y + ball.radius) < water_top_level) surf = h * h; // If ball completely submerged, use just all ball area
	surf *= 0.4; // FUYM to adjust values (should compute the area of circle segment correctly instead; I'm too lazy for that)

	// Compute Buoyancy force
	double fbuoyancy_modulus = water.density * 10.0 * surf; // Buoyancy force (modulus)
	fx = 0.0; // Buoyancy is parallel to pressure gradient
	fy = fbuoyancy_modulus; // Buoyancy is parallel to pressure gradient
}

// Integration scheme: Velocity Verlet
void integrator_velocity_verlet(PhysBall& ball, float dt)
{
	ball.x += ball.vx * dt + 0.5f * ball.ax * dt * dt;
	ball.y += ball.vy * dt + 0.5f * ball.ay * dt * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}

void integrator_velocity_euler_implicit(PhysBall& ball, float dt)
{
	ball.x += ball.vx * dt;
	ball.y += ball.vy * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}

void integrator_velocity_euler_simplectic(PhysBall& ball, float dt)
{
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
	ball.x += ball.vx * dt;
	ball.y += ball.vy * dt;
}

// Detect collision with ground
collision_state is_colliding_with_ground_unoptimized(const PhysBall& ball, const Ground& ground)
{
	float half_width = ground.w / 2.0f;
	float half_height = ground.h / 2.0f;
	float rect_x = (ground.x + half_width); // Center of rectangle
	float rect_y = (ground.y + half_height); // Center of rectangle
	return check_collision_circle_rectangle_unoptimized(ball.x, ball.y, ball.radius, rect_x, rect_y, half_width, half_height);
}

bool is_colliding_with_ground(const PhysBall& ball, const Ground& ground)
{

	//Función original con ligeros cambios como valores precalculados
	float half_width = ground.w / 2.0f;
	float half_height = ground.h / 2.0f;
	float rect_x = (ground.x + half_width); // Center of rectangle
	float rect_y = (ground.y + half_height); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, half_width, half_height);
}

// Detect collision with water
bool is_colliding_with_water(const PhysBall& ball, const Water& water)
{
	float rect_x = (water.x + water.w / 2.0f); // Center of rectangle
	float rect_y = (water.y + water.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, water.w/2, water.h/2);
}

// Detect collision between circle and rectange
collision_state check_collision_circle_rectangle_unoptimized(float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
	// Algorithm taken from https://stackoverflow.com/a/402010

	collision_state ret = collision_state::NO_COLLISION;
	// Distance from center of circle to center of rectangle
	float dist_x = std::abs(cx - rx);
	float dist_y = std::abs(cy - ry);

	// If circle is further than half-rectangle, not intersecting
	if (dist_x > (rw + cr)) { return ret; }
	if (dist_y > (rh + cr)) { return ret; }

	/*

	// If circle is closer than half-rectangle, is intersecting
	if (dist_x <= (rw / 2.0f)) { ret = collision_state::HORIZONTAL; }
	if (dist_y <= (rh / 2.0f)) { ret = (ret == collision_state::HORIZONTAL) ? collision_state::BOTH : collision_state::VERTICAL; }

	*/


	// If all of above fails, check corners
	float a = dist_x - rw;
	float b = dist_y - rh;
	float cornerDistance_sq = a * a + b * b;
	if (cornerDistance_sq <= (cr * cr)) {
		ret = collision_state::BOTH;
	}
	else {
		// Algoritmo que acabo de pensar, requiere mucha division asi que no va a ser muy bueno en rendimiento
		float colProportion = abs((dist_x / rw) / (dist_y / rh));

		if (colProportion < 1.0f) ret = collision_state::VERTICAL;
		else if (colProportion > 1.0f) ret = collision_state::HORIZONTAL;
	}

	return ret;
}

bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
	// Algorithm taken from https://stackoverflow.com/a/402010

	// Distance from center of circle to center of rectangle
	float dist_x = std::abs(cx - rx);
	float dist_y = std::abs(cy - ry);

	// If circle is further than half-rectangle, not intersecting
	if (dist_x > (rw + cr)) { return false; }
	if (dist_y > (rh + cr)) { return false; }

	// If circle is closer than half-rectangle, is intersecting
	if (dist_x <= (rw)) { return true; }
	if (dist_y <= (rh)) { return true; }

	// If all of above fails, check corners
	float a = dist_x - rw;
	float b = dist_y - rh;
	float cornerDistance_sq = a * a + b * b;
	return (cornerDistance_sq <= (cr * cr));
}

// Convert from meters to pixels (for SDL drawing)
SDL_Rect Ground::pixels()
{
	SDL_Rect pos_px{};
	pos_px.x = METERS_TO_PIXELS(x);
	pos_px.y = SCREEN_HEIGHT - METERS_TO_PIXELS(y);
	pos_px.w = METERS_TO_PIXELS(w);
	pos_px.h = METERS_TO_PIXELS(-h); // Can I do this? LOL
	return pos_px;
}


