#include "Physics.h"
#include "Input.h"
#include "app.h"
#include "Log.h"
#include "math.h"
#include "SDL/include/SDL_keycode.h"
#include "Defs.h"
#include "Log.h"
#include "Render.h"
#include "Cannon.h"
#include "Window.h"
#include "Box2D/Box2D/Box2D.h"

// Tell the compiler to reference the compiled Box2D libraries
#ifdef _DEBUG
#pragma comment( lib, "../Game/Source/External/Box2D/libx86/DebugLib/Box2D.lib" )
#else
#pragma comment( lib, "../Game/Source/External/Box2D/libx86/ReleaseLib/Box2D.lib" )
#endif

Physics::Physics(bool startEnabled) : Module(startEnabled)
{
	// Initialise all the internal class variables, at least to NULL pointer
	world = NULL;
}

// Destructor
Physics::~Physics()
{
	// You should do some memory cleaning here, if required
}

bool Physics::Start()
{
	LOG("Creating Physics 2D environment");

	// Create a new World
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// Set this module as a listener for contacts
	world->SetContactListener(this);


	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	
	

	return true;
}

// 
bool Physics::PreUpdate()
{
	bool ret = true;

	// Step (update) the World
	// WARNING: WE ARE STEPPING BY CONSTANT 1/60 SECONDS!
	world->Step(1.0f / 60.0f, 6, 2);
	

	// Because Box2D does not automatically broadcast collisions/contacts with sensors, 
	// we have to manually search for collisions and "call" the equivalent to the ModulePhysics::BeginContact() ourselves...
	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		// For each contact detected by Box2D, see if the first one colliding is a sensor
		if (c->IsTouching() && c->GetFixtureA()->IsSensor())
		{
			// If so, we call the OnCollision listener function (only of the sensor), passing as inputs our custom PhysBody classes
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureB()->GetBody()->GetUserData();
			
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2, c);
		}
	}

	return ret;
}

PhysBody* Physics::CreateRectangle(int x, int y, int width, int height, bodyType type)
{
	b2BodyDef body;

	if (type == DYNAMIC) body.type = b2_dynamicBody;
	if (type == STATIC) body.type = b2_staticBody;
	if (type == KINEMATIC) body.type = b2_kinematicBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	b->ResetMassData();

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* Physics::CreateCircle(int x, int y, int radious, bodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;

	if (type == DYNAMIC) body.type = b2_dynamicBody;
	if (type == STATIC) body.type = b2_staticBody;
	if (type == KINEMATIC) body.type = b2_kinematicBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(radious);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 1.0f;
	b->ResetMassData();

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = radious;
	pbody->height = radious;

	// Return our PhysBody class
	return pbody;
}

PhysBody* Physics::CreateRectangleSensor(int x, int y, int width, int height, bodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;
	if (type == DYNAMIC) body.type = b2_dynamicBody;
	if (type == STATIC) body.type = b2_staticBody;
	if (type == KINEMATIC) body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	// Return our PhysBody class
	return pbody;
}

PhysBody* Physics::CreateChain(int x, int y, int* points, int size, bodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;
	if (type == DYNAMIC) body.type = b2_dynamicBody;
	if (type == STATIC) body.type = b2_staticBody;
	if (type == KINEMATIC) body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];
	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.CreateLoop(p, size / 2);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Clean-up temp array
	delete p;

	// Create our custom PhysBody class
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	// Return our PhysBody class
	return pbody;
}

// Destroys the body the pointer points to, along with Userdata in the form of PhysBody*
void Physics::DestroyBody(b2Body* body)
{
	PhysBody* pBody = reinterpret_cast<PhysBody*>(body->GetUserData());
	if (pBody != nullptr) {
		pBody->body = nullptr;
		if (pBody->boundEntity != nullptr)
		{
			pBody->boundEntity->setToDestroy = true;
			pBody->boundEntity = nullptr;
		}
		delete pBody;
	}
	world->DestroyBody(body);
}

// 
bool Physics::PostUpdate()
{
	bool ret = true;



	// Activate or deactivate debug mode
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->debug = !app->debug;
	
	//  Iterate all objects in the world and draw the bodies
	if (app->debug)
	{
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					uint width, height;
					app->win->GetWindowSize(width, height);
					b2Vec2 pos = f->GetBody()->GetPosition();
					app->render->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius) * app->win->GetScale(), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 255, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
				}
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					int mouse_x, mouse_y;
					app->input->GetMousePosition(mouse_x, mouse_y);
					b2Vec2 p = { PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y) };
					if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
					{


						mouse_body = b;


						b2Vec2 mousePosition;
						mousePosition.x = p.x;
						mousePosition.y = p.y;


						b2MouseJointDef def;
						def.bodyA = ground;
						def.bodyB = mouse_body;
						def.target = mousePosition;
						def.dampingRatio = 0.5f;
						def.frequencyHz = 2.0f;
						def.maxForce = 200.0f * mouse_body->GetMass();


						mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
					}
				}
			}
		}
	}
	if (mouse_body != nullptr && mouse_joint != nullptr) {
		int mouse_x, mouse_y;
		app->input->GetMousePosition(mouse_x, mouse_y);
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			b2Vec2 mousePosition;
			mousePosition = PIXEL_TO_METERS(b2Vec2(mouse_x, mouse_y));
			mouse_joint->SetTarget(mousePosition);

		}
	}

	if (mouse_body != nullptr && mouse_joint != nullptr) {
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			app->physics->world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;
			mouse_body = nullptr;
		}
	}

	return ret;
}

// Called before quitting
bool Physics::CleanUp()
{
	LOG("Destroying physics world");

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		PhysBody* pb = (PhysBody*)b->GetUserData();
		if (pb != nullptr && pb->boundEntity != nullptr) {
			pb->boundEntity->setToDestroy = true;
			pb->boundEntity->pbody = nullptr;
		}
		delete pb;
	}

	// Delete the whole physics world!
	delete world;

	return true;
}

// Callback function to collisions with Box2D
void Physics::BeginContact(b2Contact* contact)
{
	// Call the OnCollision listener function to bodies A and B, passing as inputs our custom PhysBody classes
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB, contact);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA, contact);
}

//--------------- PhysBody

void PhysBody::GetPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}