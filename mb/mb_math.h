struct Vec2 {
    struct {
        f32 x, y;
    };
    f32 u[2];
    inline f32&       operator[](int i)       { return u[i]; }
    inline const f32& operator[](int i) const { return u[i]; } 
};

struct Vec3 {
    struct {
        f32 x, y, z;
    };
    f32 u[3];
    inline f32&       operator[](int i)       { return u[i]; }
    inline const f32& operator[](int i) const { return u[i]; } 
};

union Vec4 {
    struct {
        f32 x, y, z, w;
    };
    f32 u[4];
    inline f32&       operator[](int i)       { return u[i]; }
    inline const f32& operator[](int i) const { return u[i]; } 
};

union Mat4 {
    struct {
        Vec4 x, y, z, w;
    };
    f32 u[4][4];
    f32 p[16];
    inline f32&       operator()(int i, int j)       { return u[j][i]; }
    inline const f32& operator()(int i, int j) const { return u[j][i]; }
};

// ------------------- 2D ------------------- //
static Vec2 operator * (Vec2 a, f32 f) {
    return {a.x * f, a.y * f};
}

static Vec2 operator + (Vec2 a, Vec2 b) {
    return {a.x + b.x, a.y + b.y};
}

static Vec2 operator - (Vec2 a, Vec2 b) {
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
static Vec3 operator * (const Vec3& a, f32 f) {
    return {a.x * f, a.y * f, a.z * f};
}

static Vec3 operator + (const Vec3& a, const Vec3& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

static Vec3 operator - (const Vec3& a, const Vec3& b) {
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
static Vec4 operator * (const Vec4& a, f32 f) {
    return {a.x * f, a.y * f, a.z * f, a.w * f};
}

static Vec4 operator + (const Vec4& a, const Vec4& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static Vec4 operator - (const Vec4& a, const Vec4& b) {
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

static Vec4 operator * (const Mat4& m, const Vec4& v) {
    Vec4 vec = {};
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++) {
            vec[col] += m(row, col) * v[row];
        }
    }
    return vec;
}

static Mat4 operator * (const Mat4& a, const Mat4& b) {
    Mat4 c = {};
    for(int col = 0; col < 4; col++) {
        for(int row = 0; row < 4; row++) {
            for(int i = 0; i < 4; i++) {
                c(row, col) += a(i, col) * b(row, i);
            }
        }
    }
    return c;
}
