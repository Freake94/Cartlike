#include "mb/mb_tools.h"

struct Particle {
	Vec3 pos;
	Vec3 vel;
	Vec4 color;
	f32 life;
};

struct Player {
    Vec3 pos;
    Vec3 vel;
};

struct GameState {
    Player player;
    Camera camera;
    RenderWindow window;
    RenderQueue rq;
	Particle* particle;
	i32 particle_amount;
	i32 particle_max;
};

void render(GameState* gs) {
    {//XYZ
        create_cube(&gs->rq, {.5, .1, .1}, {.5, .1, .1}, {1, 0, 0, 1});
        create_cube(&gs->rq, {.1, .5, .1}, {.1, .5, .1}, {0, 1, 0, 1});
        create_cube(&gs->rq, {.1, .1, .5}, {.1, .1, .5}, {0, 0, 1, 1});
    }
    create_cube(&gs->rq, gs->player.pos, {.5, .5, .5}, {0, 1, 0, 1});
	
	for(int i = 0; i < gs->particle_amount; i++) {
		Particle* p = &gs->particle[i];
		create_cube(&gs->rq, p->pos, {.1, .1, .1}, p->color);
		//create_rectangle_xy(&gs->rq, p->pos, {.1, .1, .1}, p->color, {0, 0, -1});
	}
	
    render(&gs->rq);
}

void key_inputs(GameState* gs) {
    Player* p = &gs->player;
    RenderWindow* window = &gs->window;
    if(key_pressed(window, KEY_ESCAPE)) {
        window_close(window);
    }
    
    if(key_pressed(window, KEY_UP)) {
        p->vel.z = 10;
    }
    if(key_pressed(window, KEY_DOWN)) {
        p->vel.z = -10;
    }
    if(key_pressed(window, KEY_LEFT)) {
        p->vel.x = 10;
    }
    if(key_pressed(window, KEY_RIGHT)) {
        p->vel.x = -10;
    }
    if(key_pressed(window, KEY_LEFT_SHIFT)) {
        p->vel.y = 10;
    }
    if(key_pressed(window, KEY_LEFT_CONTROL)) {
        p->vel.y = -10;
    }
}

void spawn_particles(GameState* gs, Vec3 pos) {
	Particle p;
	p.pos = pos;
	p.vel = Vec3{
		f32(6 - rand() % 12), 
		f32(6 - rand() % 12), 
		f32(6 - rand() % 12) } - gs->player.vel;
	p.life = 1.0f;
	p.color = {1, 0, 0, 1};
	gs->particle[gs->particle_amount++] = p;
}

void update(GameState* gs) {
    Player* p = &gs->player;
    Camera* camera = &gs->camera;
    
    p->pos = sadd(p->pos, p->vel, gs->window.deltaT);
    
    camera->target = p->pos;
    camera->pos = { p->pos.x, p->pos.y + 3, p->pos.z - 10};
    camera->up = {0, 1, 0};
    camera_update(camera, get_window_size(&gs->window));
	if(len(gs->player.vel) > 0) {
		for(int i = 0; i < 1; i++)
			spawn_particles(gs, p->pos);
	}
	for(int i = 0; i < gs->particle_amount; i++) {
		Particle* p = &gs->particle[i];
		p->pos = sadd(p->pos, p->vel, gs->window.deltaT);
		p->life -= gs->window.deltaT;
		p->color.w = p->life;
		if(p->life <= 0) {
			*p = gs->particle[--gs->particle_amount];
			i--;
		}
	}
	p->vel = {0, 0, 0};
}

void init(GameState* gs) {
    gs->window = create_window(800, 600, "Cartlike", 1);
    gs->player = {{0, 0, 0}, {0, 0, 0}};
    gs->rq = create_render_queue();
	gs->particle_max = 1024 * 1024;
    gs->particle = (Particle*)malloc(sizeof(Particle) * gs->particle_max);
	gs->particle_amount = 0;
	camera_create(&gs->camera, {10, 10, -10}, gs->player.pos, {0, 1, 0});
    glEnable(GL_LIGHTING);
    simple_light(1, .1, 0, 10, 0);
    light_source(0, .5, 5, 0, -10);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void window_title_update(RenderWindow* window) {
    static float title_time = 0.0f;
    char buffer[8];
    sprintf(buffer, "%f", window->deltaT * 1000);
    if(title_time > 0.3) {
        set_title(window, buffer);
        title_time = 0.0f;
    }
    title_time += window->deltaT;
}

int main() {
    GameState gs;
    init(&gs);
    while(window_is_open(&gs.window)) {
        key_inputs(&gs);
        update(&gs);
        render(&gs);
        window_title_update(&gs.window);
    }
}