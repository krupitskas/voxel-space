#include <iostream>
#include <optional>
#include <filesystem>

#include <SFML/Graphics.hpp>
#define DR_PCX_IMPLEMENTATION
#include "pcx.hpp"

struct Point
{
    int x = 0;
    int y = 0;
};

std::optional<sf::Texture> load_pcx_image(const std::filesystem::path& path)
{
    int width = 0;
    int height = 0;
    int components = 0;

    uint8_t* pcx_image_data = drpcx_load_file(path.string().c_str(),
            DRPCX_FALSE, &width, &height, &components, 4);

    if (pcx_image_data == nullptr) {
        std::cerr << "Can't load pcx" << std::endl;
        return {};
    }

    sf::Image pcx_image;
    pcx_image.create(width, height, pcx_image_data);
    drpcx_free(pcx_image_data);

    sf::Texture pcx_texture;
    pcx_texture.loadFromImage(pcx_image);

    return pcx_texture;
}

//void horizontal_line(Point p1, Point p2, int offset, int scale, int horizon, Point pmap) {
//    const auto n = 700;
//
//    const float dx = static_cast<float>(p2.x - p1.x) / n;
//    const float dy = static_cast<float>(p2.y - p1.y) / n;
//
//    for(int i = 0; i < n; i)
//
//}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Open Delta");

    const auto pcx_texture_res = load_pcx_image("data/dfd1/DFD1_D.PCX");

    if(!pcx_texture_res.has_value())
        return -1;

    const auto pcx_texture = pcx_texture_res.value();

    sf::Texture color_texture;
    if (!color_texture.loadFromFile("data/dfd1/DFD1_C.JPG"))
        return EXIT_FAILURE;

    sf::Image result_image;
    result_image.create(1024, 1024, sf::Color{255, 0, 0});

    // Read from both colors and create a sprite
    int distance = 800;
    float height = 120.0f;
    float phi = 0.0f;

    for(int i = 0; i < 64; i++)
    {
        const auto y = 500 - i * 16;
        const auto x = 670;
        const auto point = Point {.x = x, .y = y};
        const auto point_map = point;

        for(int z = distance; z > 1; z -= 2)
        {
            const auto pl = Point {.x = -z, .y = -z};
            const auto pr = Point {.x = -z, .y = -z};


        }

    }


    // Finished

    sf::Texture result_texture;
    result_texture.loadFromImage(result_image);
    sf::Sprite result_sprite;
    result_sprite.setTexture(result_texture);

    while (window.isOpen())
    {
        sf::Event event {};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(result_sprite);
        window.display();
    }
}
