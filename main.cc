#include "mb/mb_tools.h"

RenderQueue rq = create_render_queue(1024 * 1024);

void render(RenderWindow* window) {
    create_cube(&rq, {0, 0, 0}, {1, 1, 1}, {0, 1, 0, 1});
    render(&rq);
}

int main() {
    printf("Hello, World!\n");
    RenderWindow window = create_window(800, 600, "Cartlike", 1);
    Camera camera;
    Vec3 camera_pos = {10, 10, -10};
    camera_create(&camera, camera_pos, {0, 0, 0}, {0, 1, 0});
    glEnable(GL_LIGHTING);
    simple_light(1, 1, 0, 10, 0);
    light_source(0, .5, 5, 0, -10);
    while(window_is_open(&window)) {
        camera_update(&camera, get_window_size(&window));
        render(&window);
        if(key_pressed(&window, KEY_ESCAPE)) {
            window_close(&window);
        }
        char buffer[8];
        sprintf(buffer, "%f", window.deltaT * 1000);
        set_title(&window, buffer);
    }
}