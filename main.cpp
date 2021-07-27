#include <iostream>
#include <optional>
#include <filesystem>

#include <SFML/Graphics.hpp>
#define DR_PCX_IMPLEMENTATION
#include "pcx.hpp"

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

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Open Delta");

    const auto pcx_texture_res = load_pcx_image("data/dfd1/DFD1_D.PCX");

    if(!pcx_texture_res.has_value())
        return -1;

    sf::Sprite pcx_sprite(pcx_texture_res.value());

    sf::Texture texture;
    if (!texture.loadFromFile("data/dfd1/DFD1_C.JPG"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);

    while (window.isOpen())
    {
        sf::Event event {};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(pcx_sprite);
        window.display();
    }
}
