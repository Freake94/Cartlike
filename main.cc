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
    {//XYZ
        create_cube(&gs->rq, {.5, .1, .1}, {.5, .1, .1}, {1, 0, 0, 1});
        create_cube(&gs->rq, {.1, .5, .1}, {.1, .5, .1}, {0, 1, 0, 1});
        create_cube(&gs->rq, {.1, .1, .5}, {.1, .1, .5}, {0, 0, 1, 1});
    }
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
    gs->player = {{0, 0, 0}, {0, 0, 0}};
    gs->rq = create_render_queue(1024 * 1024);
    camera_create(&gs->camera, {10, 10, -10}, gs->player.pos, {0, 1, 0});
    glEnable(GL_LIGHTING);
    simple_light(1, .1, 0, 10, 0);
    light_source(0, .5, 5, 0, -10);
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

void vector_fun() {
    Vec4 v = {1, 2, 3, 4};
    Mat4 a {
        1.0f,2.0f,3.0f,4.0f, 
        5.0f,6.0f,7.0f,8.0f, 
        9.0f,10.0f,11.0f,12.0f, 
        13.0f,14.0f,15.0f,16.0f
    };
    Mat4 b { 
        22.0f, 0.0f, 11.0f, 33.0f, 
        1.0f, 2.0f, 3.0f, 4.0f, 
        5.0f, 4.0f, 1.0f, 2.0f, 
        11.0f, 91.0f, 32.0f, 42.0f
    };
    
    Mat4 m = a * b;
    Vec4 vv = a * v;
    printf("Matrix:\n");
    for(int j = 0; j < 4; j++) {
        printf("{ ");
        for(int i = 0; i < 4; i++) {
            printf("%0.f\t", m(i, j));
        }
        puts(" }");
    }
    puts("\nVector:");
    for(int i = 0; i < 4; i++) {
        printf("%0.f\n", vv[i]);
    }
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
    vector_fun();
}