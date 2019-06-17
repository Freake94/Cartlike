#pragma once
#include <GL/glu.h>
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include "mb_keys.h"

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

#include "mb_math.h"

// Defer macro/thing.
#define CONCAT_INTERNAL(x,y) x##y
#define CONCAT(x,y) CONCAT_INTERNAL(x,y)

template<typename T>
struct ExitScope {
    T lambda;
    ExitScope(T lambda):lambda(lambda){}
    ~ExitScope(){lambda();}
    ExitScope(const ExitScope&);
    private:
    ExitScope& operator =(const ExitScope&);
};

class ExitScopeHelp {
    public:
    template<typename T>
        ExitScope<T> operator+(T t){ return t;}
};

#define defer const auto& CONCAT(defer__, __LINE__) = ExitScopeHelp() + [&]()

//---------------------WINDOW---------------------//
struct RenderWindow {
    GLFWwindow* window;
    f64 deltaT;
    f64 old_time;
    u8 depth;
};

static f64 get_time() {
    return glfwGetTime();
}

void light_source(int index, f32 bright, f32 x, f32 y, f32 z) {
    f32 pos[4] = {x, y, z, 1};
    f32 zero[4] = { 0, 0, 0, 0 };
    f32 c[4] = { bright, bright, bright, 0 };
    GLuint light = GL_LIGHT0 + index;
    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE, c);
    glLightfv(light, GL_AMBIENT, zero);
    glLightfv(light, GL_SPECULAR, zero);
    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void simple_light(int index, f32 bright, f32 x, f32 y, f32 z) {
    f32 d = (f32) (1.0f/sqrtf(x*x+y*y+z*z));
    f32 dir[4] = { x*d,y*d,z*d,0 }, zero[4] = { 0,0,0,0 };
    f32 c[4] = { bright,bright,bright,0 };
    GLuint light = GL_LIGHT0 + index;
    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE, c);
    glLightfv(light, GL_AMBIENT, zero);
    glLightfv(light, GL_SPECULAR, zero);
    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static Vec2 get_window_size(RenderWindow* w) {
    int x, y;
    glfwGetWindowSize(w->window, &x, &y);
    return {(f32)x, (f32)y};
}

static RenderWindow create_window(f32 width, f32 height, const char* c, u8 depth) {
    glfwInit();
    srand(time(nullptr));
    GLFWwindow* window = glfwCreateWindow(width, height, c, NULL, NULL);
    glfwMakeContextCurrent(window);
    if(depth) {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    return (RenderWindow){window, 0, 0, depth};
}

static void update_deltaT(RenderWindow* w) {
    f64 current_time = get_time();
    w->deltaT = current_time - w->old_time;
    w->old_time = current_time;
}

static void window_update(RenderWindow* w) {
    update_deltaT(w);
    glfwSwapBuffers(w->window);
    glfwPollEvents();
    
    if(w->depth) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else glClear(GL_COLOR_BUFFER_BIT);
    
    Vec2 wSize = get_window_size(w);
    glViewport(0, 0, wSize.x, wSize.y);
}

static u8 window_is_open(RenderWindow* w){
    window_update(w);
    return !glfwWindowShouldClose(w->window);
}

static void window_close(RenderWindow* w) {
    glfwSetWindowShouldClose(w->window, GLFW_TRUE);
}

static u8 key_pressed(RenderWindow* w, u32 key) {
    return glfwGetKey(w->window, key) == GLFW_PRESS;
}

static void set_title(RenderWindow* w, char* title) {
    glfwSetWindowTitle(w->window, title);
}

//---------------------RENDER---------------------//
struct Triangle {
    Vec3 normal;
    Vec4 color;
    Vec3 pos[3];
};

struct RenderQueue {
    Triangle* triangle;
    i32 amount;
};

static RenderQueue create_render_queue(int triangle_max_length) {
    RenderQueue rq;
    rq.triangle = (Triangle*)malloc(sizeof(Triangle) * triangle_max_length);
    rq.amount = 0;
    return rq;
}

static void add_triangle(RenderQueue* rq, Triangle tri) {
    rq->triangle[rq->amount] = tri;
    rq->amount++;
}

static void add_rectangle(RenderQueue* rq, const Vec3 pos[3], Vec4 col, Vec3 normal) {
    Triangle a, b;
    a.pos[0] = pos[0];
    a.pos[1] = pos[1];
    a.pos[2] = pos[3];
    a.normal = normal;
    a.color = col;
    b.pos[0] = pos[3];
    b.pos[1] = pos[1];
    b.pos[2] = pos[2];
    b.normal = normal;
    b.color = col;
    add_triangle(rq, a);
    add_triangle(rq, b);
}

static void create_rectangle(RenderQueue* rq, Vec2 pos, Vec2 rad, Vec4 col) {
    Vec3 array[] = {{pos.x - rad.x, pos.y - rad.y}, {pos.x + rad.x, pos.y - rad.y},
        {pos.x + rad.x, pos.y + rad.y}, {pos.x - rad.x, pos.y + rad.y}};
    add_rectangle(rq, array, col, {0, 0, 1});
}

static void create_rectangle_xy(RenderQueue* rq, Vec3 pos, Vec3 rad, Vec4 col, Vec3 norm) {
    Vec3 array[] = {
        {pos.x - rad.x, pos.y - rad.y, pos.z},
        {pos.x + rad.x, pos.y - rad.y, pos.z},
        {pos.x + rad.x, pos.y + rad.y, pos.z},
        {pos.x - rad.x, pos.y + rad.y, pos.z}};
    add_rectangle(rq, array, col, norm);
}

static void create_rectangle_xz(RenderQueue* rq, Vec3 pos, Vec3 rad, Vec4 col, Vec3 norm) {
    Vec3 array[] = {
        {pos.x - rad.x, pos.y, pos.z - rad.z},
        {pos.x + rad.x, pos.y, pos.z - rad.z},
        {pos.x + rad.x, pos.y, pos.z + rad.z},
        {pos.x - rad.x, pos.y, pos.z + rad.z}};
    add_rectangle(rq, array, col, norm);
}

static void create_rectangle_yz(RenderQueue* rq, Vec3 pos, Vec3 rad, Vec4 col, Vec3 norm) {
    Vec3 array[] = {
        {pos.x, pos.y - rad.y, pos.z - rad.z},
        {pos.x, pos.y + rad.y, pos.z - rad.z},
        {pos.x, pos.y + rad.y, pos.z + rad.z},
        {pos.x, pos.y - rad.y, pos.z + rad.z}};
    add_rectangle(rq, array, col, norm);
}

static void create_cube(RenderQueue* rq, Vec3 pos, Vec3 scale, Vec4 col) {
    create_rectangle_xy(rq, {pos.x, pos.y, pos.z - scale.z}, scale, col, {0, 0, -1});
    create_rectangle_xy(rq, {pos.x, pos.y, pos.z + scale.z}, scale, col, {0, 0, +1});
    
    create_rectangle_xz(rq, {pos.x, pos.y - scale.y, pos.z}, scale, col, {0, -1, 0});
    create_rectangle_xz(rq, {pos.x, pos.y + scale.y, pos.z}, scale, col, {0, +1, 0});
    
    create_rectangle_yz(rq, {pos.x - scale.x, pos.y, pos.z}, scale, col, {-1, 0, 0});
    create_rectangle_yz(rq, {pos.x + scale.x, pos.y, pos.z}, scale, col, {+1, 0, 0});
}

static void create_line(RenderQueue* rq, Vec2 start_pos, Vec2 end_pos, f32 width, Vec4 col) {
    Vec2 normal = end_pos - start_pos;
    normal = {normal.y, -normal.x};
    normal = norm(normal);
    Vec2 dir = normal * width;
    Vec3 array[] = {{start_pos.x - dir.x, start_pos.y - dir.y}, {end_pos.x - dir.x, end_pos.y - dir.y}, 
        {end_pos.x + dir.x, end_pos.y + dir.y}, {start_pos.x + dir.x, start_pos.y + dir.y}};
    add_rectangle(rq, array, col, {0, 0, 1});
}

static void set_render_color(Vec4 col) {
    glColor4f(col.x, col.y, col.z, col.w);
}

static void render(RenderQueue* rq) {
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < rq->amount; i++) {
        Triangle* tri = &rq->triangle[i];
        set_render_color(tri->color);
        glNormal3fv(&tri->normal.x);
        for(int j = 0; j < 3; j++) {
            glVertex3fv(&tri->pos[j].x);
        }
    }
    glEnd();
    
    rq->amount = 0;
}

//---------------------CAMERA---------------------//

struct Camera {
    Vec3 pos;
    Vec3 target;
    Vec3 dir;
    Vec3 up;
    Vec3 right;
};

static Camera camera_create(Camera* camera, Vec3 position, Vec3 target, Vec3 up) {
    camera->pos = position;
    camera->target = target;
    camera->dir = norm(camera->pos - camera->target);
    camera->right = norm(cross(up, camera->dir));
    camera->up = cross(camera->dir, camera->right);
}

static void camera_update(Camera* c, Vec2 wSize) {
    c->dir = norm(c->pos - c->target);
    c->right = norm(cross(c->up, c->dir));
    c->up = cross(c->dir, c->right);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, wSize.x / wSize.y, .1, 100);
    gluLookAt(c->pos.x, c->pos.y, c->pos.z,
              c->target.x, c->target.y, c->target.z,
              c->up.x, c->up.y, c->up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
