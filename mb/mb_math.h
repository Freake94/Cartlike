#define sqrtf __builtin_sqrtf
#define sinf __builtin_sinf
#define cosf __builtin_cosf

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
static Vec3 mul(const Vec3& a, f32 f) {
    return {a.x * f, a.y * f, a.z * f};
}

static Vec3 add(const Vec3& a, const Vec3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

static Vec3 sub(const Vec3& a, const Vec3& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static Vec3 sadd(const Vec3& a, const Vec3& b, f32 f) {
    return {a.x + b.x * f, a.y + b.y * f, a.z + b.z * f};
}

static f32 len(const Vec3& a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

static Vec3 norm(const Vec3& a) {
    f32 l = len(a);
    return {a.x / l, a.y / l, a.z / l};
}

static Vec3 cross(const Vec3& a, const Vec3& b) {
    return {a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y};
}

// ------------------- 4D ------------------- //
static Vec4 mul(const Vec4& a, f32 f) {
    return {a.x * f, a.y * f, a.z * f, a.w * f};
}

static Vec4 add(const Vec4& a, const Vec4& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static Vec4 sub(const Vec4& a, const Vec4& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static Vec4 sadd(const Vec4& a, const Vec4& b, f32 f) {
    return {a.x + b.x * f, a.y + b.y * f, a.z + b.z * f, a.w + b.w * f};
}

static f32 len(const Vec4& a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

static Vec4 norm(const Vec4& a) {
    f32 l = len(a);
    return {a.x / l, a.y / l, a.z / l, a.w / l};
}

union Mat4 {
    struct {
        Vec4 x, y, z, w;
    };
    f32 m[4][4];
    f32 p[16];
};

Mat4 mul(const Mat4& a, const Mat4& b) {
    Mat4 c = {};
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++) {
            for(int i = 0; i < 4; i++) {
                c.m[col][row] += a.m[col][i] * b.m[i][row];
            }
        }
    }
    return c;
}