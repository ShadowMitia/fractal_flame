#ifndef HEADER_VARIATIONS
#define HEADER_VARIATIONS

#include <numbers>
#include <cmath>

struct Point
{
    float x{0.0};
    float y{0.0};
};

constexpr Point operator+(Point lhs, Point rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
constexpr Point operator*(float k, Point pt) { return {k * pt.x, k * pt.y}; }
constexpr Point operator*(Point pt, float k) { return k * pt; }
constexpr Point operator/(Point pt, float k) { return {pt.x / k, pt.y / k}; }

struct RGB
{
    float r{0.0};
    float g{0.0};
    float b{0.0};
};


constexpr RGB operator+(RGB lhs, RGB rhs) { return {lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b}; }
constexpr RGB operator/(RGB rgb, float k) { return {rgb.r / k, rgb.g / k, rgb.b / k}; }
constexpr RGB operator*(float k, RGB pt) { return {k * pt.r, k * pt.g, k * pt.b}; }
constexpr RGB operator*(RGB pt, float k) { return k * pt; }

struct FlamePoint
{
    Point point;
    RGB colour;
};

// Variation 0
constexpr Point linear(float x, float y)
{
    return {x, y};
}

// Variation 1
constexpr Point sinusoidal(float x, float y)
{
    return {std::sin(x), std::sin(y)};
}

// Variation 2
constexpr Point spherical(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    return Point{x, y} * ((r > 0) ? (1 / (r * r)) : 0);
}

// Variation 3
constexpr Point swirl(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    return Point{x * std::sin(r * r) - y * std::cos(r * r),
                 x * std::cos(r * r) + y * std::sin(r * r)};
}

// Variation 4
constexpr Point horseshoe(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    // if (r < 0.001f)
    // {
    //     r = 0.001f;
    // }
    return Point{((x - y) * (x + y)) / r, (2.f * x * y) / r};
}

// Variation 5
constexpr Point polar(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return Point{theta / std::numbers::pi_v<float>, r - 1};
}

// Variation 6
constexpr Point handkerchief(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return r * Point{std::sin(theta + r), std::cos(theta - r)};
}

// Variation 7
constexpr Point heart(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return Point{r * std::sin(theta * r), -r * std::cos(theta * r)};
}

// Variation 8
constexpr Point disc(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return (theta / std::numbers::pi_v<float>)*Point{std::sin(std::numbers::pi_v<float> * r), std::cos(std::numbers::pi_v<float> * r)};
}

// Variation 9
constexpr Point spiral(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return (1 / r) * Point{std::cos(theta) + std::sin(r), std::sin(theta) - std::cos(r)};
}

// Variation 10
constexpr Point hyperbolic(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return Point{std::sin(theta) / r, r * std::cos(theta)};
}

// Variation 11
constexpr Point diamond(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return Point{std::sin(theta) * std::cos(r), std::cos(theta) * std::sin(r)};
}

// Variation 12
constexpr Point ex(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    auto p0 = std::sin(theta + r);
    auto p1 = std::cos(theta - r);
    return r * Point{p0 * p0 * p0 + p1 * p1 * p1, p0 * p0 * p0 - p1 * p1 * p1};
}

// TODO: Variation 13

// Variation 14
constexpr Point bent(float x, float y)
{
    if (x >= 0 and y >= 0)
    {
        return Point{x, y};
    }
    else if (x < 0 and y >= 0)
    {
        return Point{2 * x, y};
    }
    else if (x >= 0 and y < 0)
    {
        return Point{x, y / 2};
    }
    else if (x < 0 and y < 0)
    {
        return Point{2 * x, y / 2};
    }
}

// TODO: Variation 15

// Variation 16
constexpr Point fisheye(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    return (2 / (r + 1)) * Point{y, x};
}

// TODO: variation 17

// Variation 18
constexpr Point exponential(float x, float y)
{
    return std::exp(x - 1) * Point{std::cos(std::numbers::pi_v<float> * y), std::sin(std::numbers::pi_v<float> * y)};
}

// Variation 19
constexpr Point power(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    const auto theta = std::atan2(x, y);
    return (std::pow(r, std::sin(theta))) * Point{std::cos(theta), std::sin(theta)};
}

// Variation 20
constexpr Point cosine(float x, float y)
{
    return Point{std::cos(std::numbers::pi_v<float> * x) * std::cosh(y), -std::sin(std::numbers::pi_v<float> * x) * std::sinh(y)};
}

// TODO: variation 21

// TODO: variation 22

// TODO: variation 23

// TODO: variation 24

// TODO: variation 25

// TODO: variation 26

// Variation 27
constexpr Point eyefish(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    return (2 / (r + 1)) * Point{x, y};
}

// Variation 28
constexpr Point bubble(float x, float y)
{
    const auto r = std::sqrt(x * x + y * y);
    return (4 / (r * r + 5)) * Point{x, y};
}

// Variation 29
constexpr Point cylinder(float x, float y)
{
    return Point{std::sin(x), y};
}

// TODO: Variation 30

// TODO: Variation 31

// TODO: Variation 32

// TODO: Variation 33

// TODO: Variation 34

// TODO: Variation 35

// TODO: Variation 36

// TODO: Variation 37

// TODO: Variation 38

// TODO: Variation 39

// TODO: Variation 40

// TODO: Variation 41

// Variation 42
constexpr Point tangent(float x, float y)
{
    return Point{std::sin(x) / std::cos(y), std::tan(y)};
}

// TODO: Variation 43

// TODO: Variation 44

// TODO: Variation 45

// TODO: Variation 46

// TODO: Variation 47

// Variation 48
constexpr Point cross(float x, float y)
{
    return std::sqrt(1 / ((x * x - y * y) * (x * x - y * y))) * Point{x, y};
}

#endif /* HEADER_VARIATIONS */