#include <math.h>

struct Vec2 {
    f32 x;
    f32 y;
};

struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};

struct Vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

// ------------------- 2D ------------------- //
static Vec2 mul(Vec2 a, f32 f) {
    return {a.x * f, a.y * f};
}

static Vec2 add(Vec2 a, Vec2 b) {
    return {a.x + b.x, a.y + b.y};
}

static Vec2 sub(Vec2 a, Vec2 b) {
    return {a.x - b.x, a.y - b.y};
}

static Vec2 sadd(Vec2 a, Vec2 b, f32 f) {
    return {a.x + b.x * f, a.y + b.y * f};
}

static f32 len(Vec2 a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}

static Vec2 norm(Vec2 a) {
    f32 l = len(a);
    return {a.x / l, a.y / l};
}

// ------------------- 3D ------------------- //
static Vec3 mul(Vec3 a, f32 f) {
    return {a.x * f, a.y * f, a.z * f};
}

static Vec3 add(Vec3 a, Vec3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

static Vec3 sub(Vec3 a, Vec3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static Vec3 sadd(Vec3 a, Vec3 b, f32 f) {
    return {a.x + b.x * f, a.y + b.y * f, a.z + b.z * f};
}

static f32 len(Vec3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

static Vec3 norm(Vec3 a) {
    f32 l = len(a);
    return {a.x / l, a.y / l, a.z / l};
}

static Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y};
}

// ------------------- 4D ------------------- //
static Vec4 mul(Vec4 a, f32 f) {
    return {a.x * f, a.y * f, a.z * f, a.w * f};
}

static Vec4 add(Vec4 a, Vec4 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static Vec4 sub(Vec4 a, Vec4 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static Vec4 sadd(Vec4 a, Vec4 b, f32 f) {
    return {a.x + b.x * f, a.y + b.y * f, a.z + b.z * f, a.w + b.w * f};
}

static f32 len(Vec4 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

static Vec4 norm(Vec4 a) {
    f32 l = len(a);
    return {a.x / l, a.y / l, a.z / l, a.w / l};
}

union Mat4 {
    struct {
        Vec4 x, y, z, w;
    };
    f32 m[4][4];
};

static Mat4 mul(Mat4 a, Mat4 b) {
    
}