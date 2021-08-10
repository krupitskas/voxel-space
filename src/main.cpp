#include <iostream>
#include <optional>
#include <filesystem>

#include <SFML/Graphics.hpp>

#define DR_PCX_IMPLEMENTATION
#include "pcx.hpp"

struct Point
{
    float x = 0.0f;
    float y = 0.0f;
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

sf::Image render(const sf::Image& color_img, const sf::Image& height_img)
{
    int scr_width = 800;
    int scr_height = 600;
    sf::Image result_image;
    result_image.create(scr_width, scr_height, sf::Color{255, 0, 0});

    const Point player_pos = Point{.x =  256, .y = 512};
    int distance = 512;
    int height = 50;
    int horizon = 240;
    int z_step = 1;

    // Going from back to front
    for(int z = distance; z > 1; z -= z_step)
    {
        // Draw horizontal line
        Point p_left  = Point{ .x = -z + player_pos.x, .y = -z + player_pos.y };
        Point p_right = Point{ .x = z + player_pos.x, .y = -z + player_pos.y };

        float dx = float(p_right.x - p_left.x) / (float)scr_width;
        float dy = float(p_right.y - p_left.y) / (float)scr_height;

        for(int i = 0; i < scr_width; i++)
        {
            if(p_left.x < 0 || p_left.x > height_img.getSize().x || p_left.y < 0 || p_left.y > height_img.getSize().y)
                continue;

            auto heightmap_pixel = height_img.getPixel(p_left.x, p_left.y);

            auto heightmap_val = (heightmap_pixel.r + heightmap_pixel.g + heightmap_pixel.b) / 3.0f;

            float height_on_screen = float(height - heightmap_val) / float(z) * 240.0f + (float)horizon;

            if(height_on_screen >= scr_height)
                continue;

            if(height_on_screen < 0)
                height_on_screen = 0;

            auto color = color_img.getPixel(p_left.x , p_left.y);

            for(int j = height_on_screen; j < scr_height; j++)
                result_image.setPixel(i, j, color);

            p_left.x += dx;
            p_left.y += dy;
        }
    }

    return result_image;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Open Delta");

    const auto height_tex = load_pcx_image("data/dfd1/DFD1_D.PCX");

    if(!height_tex.has_value())
        return -1;

    const auto& pcx_texture = height_tex.value();

    sf::Texture color_texture;
    if (!color_texture.loadFromFile("data/dfd1/DFD1_C.JPG"))
        return EXIT_FAILURE;

    auto color_img = color_texture.copyToImage();
    auto height_img = pcx_texture.copyToImage();

    sf::Texture result_texture;
    result_texture.loadFromImage(render(color_img, height_img));

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
