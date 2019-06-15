#include "mb/mb_tools.h"

struct Player {
    Vec3 pos;
    Vec3 vel;
};

struct GameState {
    Player player;
    Camera camera;
    RenderWindow window;
    RenderQueue rq;
};

void render(GameState* gs) {
    
    create_cube(&gs->rq, {0, 0, 0}, {1, 1, 1}, {1, 1, 1, 1});
    create_cube(&gs->rq, gs->player.pos, {1, 1, 1}, {0, 1, 0, 1});
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

void update(GameState* gs) {
    Player* p = &gs->player;
    Camera* camera = &gs->camera;
    
    p->pos = sadd(p->pos, p->vel, gs->window.deltaT);
    p->vel = {0, 0, 0};
    
    camera->target = p->pos;
    camera->pos = { p->pos.x, p->pos.y + 3, p->pos.z - 10};
    camera->up = {0, 1, 0};
    camera_update(camera, get_window_size(&gs->window));
}

void init(GameState* gs) {
    gs->window = create_window(800, 600, "Cartlike", 1);
    gs->player = {{0, 0, 0,}, {0, 0, 0}};
    gs->rq = create_render_queue(1024 * 1024);
    camera_create(&gs->camera, {10, 10, -10}, gs->player.pos, {0, 1, 0});
    glEnable(GL_LIGHTING);
    simple_light(1, .1, 0, 10, 0);
    light_source(0, .5, 5, 0, -10);
}

int main() {
    GameState gs;
    init(&gs);
    float title_time = 0.0f;
    while(window_is_open(&gs.window)) {
        key_inputs(&gs);
        update(&gs);
        render(&gs);
        
        char buffer[8];
        sprintf(buffer, "%f", gs.window.deltaT * 1000);
        if(title_time > 0.3) {
            set_title(&gs.window, buffer);
            title_time = 0.0f;
        }
        title_time += gs.window.deltaT;
    }
}