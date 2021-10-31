#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <numbers>
#include <algorithm>
#include <cmath>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "variations.hpp"

std::vector<float> create_gaussian_kernel(std::size_t kernel_size, float deviation_factor = 1.0f)
{
    if (not kernel_size % 2 == 0)
    {
        kernel_size--;
    }
    std::vector<float> kernel(kernel_size, 0.f);

    auto center = kernel_size / 2 + 1;

    auto max = 0.f;

    for (std::size_t i = 0; i < kernel_size; i++)
    {
        for (std::size_t j = 0; j < kernel_size; j++)
        {

            auto index = j * kernel_size + i;
            kernel[index] = std::exp(-static_cast<float>((i - center) * (i - center) + (j - center) * (j - center)) / (static_cast<float>(kernel_size) / deviation_factor));

            if (kernel[index] > max)
            {
                max = kernel[index];
            }
        }
    }

    for (auto &ker : kernel)
    {
        ker /= max;
    }

    return kernel;
}

void normalise(std::vector<RGB> &rgb)
{
    float max_r = 0.f;
    float max_g = 0.f;
    float max_b = 0.f;

    for (std::size_t i = 0; i < rgb.size(); i++)
    {
        max_r = std::max(rgb[i].r, max_r);
        max_g = std::max(rgb[i].g, max_g);
        max_b = std::max(rgb[i].b, max_b);
    }

    for (std::size_t i = 0; i < rgb.size(); i++)
    {
        rgb[i].r /= max_r;
        rgb[i].g /= max_g;
        rgb[i].b /= max_b;
    }
}

// std::size_t bounds_check(std::vector<float> signal, std::size_t index, std::size_t filter_width, std::size_t dir) {
//     if (index < 1) {
//         return filter_width - (1 - index);
//     } else if (index > ) {

//     } else {
//         return filter_width;
//     }
// }

std::vector<Point> chaos_game(std::size_t iterations, std::vector<Point> const &shapes)
{
    std::vector<Point> res(iterations, Point{0, 0});

    std::random_device device;
    std::mt19937_64 gen(device());
    std::uniform_real_distribution<float> dis(0, 1);

    std::uniform_int_distribution<std::size_t> dis2(0, shapes.size() - 1);

    Point point{dis(gen), dis(gen)};
    for (std::size_t i = 0; i < 200; i++)
    {
        auto index = dis2(gen);
        point = (shapes[index] + point) * 0.5;
    }

    for (std::size_t i = 0; i < iterations; i++)
    {
        res[i] = point;
        auto index = dis2(gen);
        point = (shapes[index] + point) * 0.5;
        // point = fn(point.x, point.y);
    }

    return res;
}

constexpr float scale(
    float x,
    float xLoOld, float xHiOld,
    float xLoNew, float xHiNew)
{
    return (x - xLoOld) / (xHiOld - xLoOld) * (xHiNew - xLoNew) + xLoNew;
}

std::vector<FlamePoint> flames(std::vector<Point> const &output, std::vector<Point (*)(float, float)> const &variations, std::vector<RGB> const &variations_color)
{
    std::random_device device;
    std::mt19937_64 gen(device());

    std::uniform_int_distribution<std::size_t> dis2(0, variations.size() - 1);

    std::vector<FlamePoint> flames;
    flames.resize(output.size(), FlamePoint{});

    auto final_colour = RGB{0, 1, 1};
    auto final_f = sinusoidal;

    flames[0].point = output[0];
    flames[0].colour = RGB{0, 0, 0};

    for (std::size_t i = 0; i < flames.size(); i++)
    {

        // Flame algorithm
        if (i > 0)
        {

            // Step 1 : Choose a random variation in the set
            auto variation_index = dis2(gen);

            // TODO: Step 2: Apply affine transform to the point

            // Step 3: Apply it to the current point in the chaos game
            flames[i].point = variations[variation_index](flames[i - 1].point.x, flames[i - 1].point.y);

            // Step 4: Set colour to point, associated with chosen variation
            flames[i].colour = variations_color[variation_index];
        }

        // Step 5: Mix colour with final colour
        auto transformed_colour = (flames[i].colour + final_colour) * 0.5f;

        // TODO Step 6: Final affine transform to point

        flames[i].colour = transformed_colour;

        // Step 7: Final transformation to point
        flames[i].point = final_f(flames[i].point.x, flames[i].point.y);
    }

    return flames;
}

int main()
{

    std::random_device device;
    std::mt19937_64 gen(device());

    stbi_flip_vertically_on_write(true);

    const auto shapes = std::vector<Point>{Point{0.0, 0.0}, Point{0.5, std::sqrt(0.75f)}, Point{1.0, 0.0}};

    const std::vector<Point (*)(float, float)> variations{swirl, polar, heart, horseshoe};

    const std::vector<RGB> variations_color{
        RGB{0, 1, 0},
        RGB{0, 0, 1},
        RGB{1, 0, 1},
        RGB{1, 0, 0}};

    auto range = 1.f;

    std::uniform_real_distribution<float> dis(0.0, 1.0);

    // const auto output = chaos_game(1'000'000, shapes);
    std::vector<Point> output(9200000);

    output[0] = Point{-0.5f * range + dis(gen) * range, -0.5f * range + dis(gen) * range};

    auto flames = ::flames(output, variations, variations_color);

    const std::size_t width = 900;
    const std::size_t height = 900;
    const std::size_t channels = 3;

    std::vector<std::size_t> histogram(width * height, 0);

    std::vector<RGB> data(width * height, RGB{0, 0, 0});

    std::size_t max_histogram = 0;

    for (auto &flame : flames)
    {
        flame.point.x = std::floor((flame.point.x + range * 0.5f) / range * static_cast<float>(width));
        flame.point.y = std::floor((flame.point.y + range * 0.5f) / range * static_cast<float>(height));

        auto index = (std::floor(flame.point.y) * static_cast<float>(width) + std::floor(flame.point.x));

        if (index > 0 && index < data.size())
        {
            data[index] = (data[index] * static_cast<float>(histogram[index]) + flame.colour) / (static_cast<float>(histogram[index] + 1));
            histogram[index] += 1;
            if (histogram[index] > max_histogram)
            {
                max_histogram = histogram[index];
            }
        }
    }

    flames.erase(std::begin(flames), std::next(std::begin(flames), 20));

    std::vector<std::uint8_t> pixels(width * height * channels, 0);

    for (auto &flame : flames)
    {

        auto hist_index = static_cast<std::size_t>(std::floor(flame.point.y) * static_cast<float>(width) + std::floor(flame.point.x));

        flame.point.x *= static_cast<float>(channels);

        auto index = static_cast<std::size_t>(std::floor(flame.point.y) * static_cast<float>(width * channels) + std::floor(flame.point.x));

        if (index + channels < pixels.size())
        {

            // Logscale
            auto alpha = [&]
            {
                if (histogram[hist_index] > 0)
                {
                    return std::log10(9.f * static_cast<float>(histogram[hist_index] / static_cast<float>(max_histogram)) + 1.f);
                }
                else
                {
                    return 1.f;
                }
            }();

            const auto gamma = 2.2f;

            auto pixel = data[hist_index] * alpha;

            // Gamma correction
            pixel.r = std::pow(pixel.r, 1.f / gamma);
            pixel.g = std::pow(pixel.g, 1.f / gamma);
            pixel.b = std::pow(pixel.b, 1.f / gamma);

            pixel = pixel * 255;

            pixels[index + 0] = static_cast<std::uint8_t>(pixel.r);
            pixels[index + 1] = static_cast<std::uint8_t>(pixel.g);
            pixels[index + 2] = static_cast<std::uint8_t>(pixel.b);
        }
    }

    stbi_write_jpg("fractal.jpg", width, height, channels, pixels.data(), 100);

    return EXIT_SUCCESS;
}