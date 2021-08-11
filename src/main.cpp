#include <iostream>
#include <optional>
#include <filesystem>

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

#define DR_PCX_IMPLEMENTATION
#include "pcx.hpp"

// Plan:
// 1. Add camera + movement and rotation
// 1.5. Add -WError flag in CMake
// 2. Add minimap in imgui
// 3. Add support for detailed map
// 4. Add PFF loading and select different maps from imgui
// 5. Basically 4. - parse .TRN file

struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3& operator+=(const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }
};

struct Camera {
    Vector3 pos;
    Vector3 dir;

    void rotate()
    {

    }

    void translate(const Vector3& t)
    {
        pos += t;
    }
};

struct Window {

    void init() {

    }
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

void render(sf::Image& result_image, const sf::Image& color_img, const sf::Image& height_img, const Camera& camera, int distance, int horizon, uint16_t win_width, uint16_t win_height)
{
    for(int x = 0; x < result_image.getSize().x; x++)
        for(int y = 0; y < result_image.getSize().y; y++)
            result_image.setPixel(x, y, {53, 81, 92});

    // Going from back to front
    for(int z = distance; z > 1; z--)
    {
        const auto camera_dist = static_cast<float>(z);

        // Draw horizontal line
        auto p_left  = Vector2{ .x = -camera_dist + camera.pos.x, .y = -camera_dist + camera.pos.y };
        auto p_right = Vector2{ .x = camera_dist + camera.pos.x, .y = -camera_dist + camera.pos.y  };

        float dx = float(p_right.x - p_left.x) / (float)win_width;
        float dy = float(p_right.y - p_left.y) / (float)win_height;

        for(uint16_t i = 0; i < win_width; i++)
        {
            if(p_left.x < 0 || p_left.x > height_img.getSize().x || p_left.y < 0 || p_left.y > height_img.getSize().y)
                continue;

            auto heightmap_pixel = height_img.getPixel(p_left.x, p_left.y);

            auto heightmap_val = (heightmap_pixel.r + heightmap_pixel.g + heightmap_pixel.b) / 3.0f;

            float height_on_screen = float(camera.pos.z - heightmap_val) / float(z) * 240.0f + (float)horizon;

            if(height_on_screen >= win_height)
                continue;

            if(height_on_screen < 0)
                height_on_screen = 0;

            auto color = color_img.getPixel(p_left.x , p_left.y);

            for(int j = height_on_screen; j < win_height; j++)
                result_image.setPixel(i, j, color);

            p_left.x += dx;
            p_left.y += dy;
        }
    }
}

int main()
{
    constexpr uint16_t width = 800, height = 600;
    std::string title = "Open Delta";
    sf::RenderWindow window(sf::VideoMode(width, height), title.c_str());
//    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    const auto height_tex = load_pcx_image("data/dfd1/DFD1_D.PCX");

    if(!height_tex.has_value())
        return EXIT_FAILURE;

    const auto& pcx_texture = height_tex.value();

    sf::Texture color_texture;
    if (!color_texture.loadFromFile("data/dfd1/DFD1_C.JPG"))
        return EXIT_FAILURE;

    auto color_img = color_texture.copyToImage();
    auto height_img = pcx_texture.copyToImage();

    Camera camera;
    camera.pos = {512, 512, 50};

    sf::Image result_image;
    result_image.create(width, height, sf::Color{255, 0, 0});

    sf::Texture result_texture;
    result_texture.loadFromImage(result_image);

    sf::Sprite result_sprite;
    result_sprite.setTexture(result_texture);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event {};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                camera.translate({-1.0, 0.0f, 0.0f});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                camera.translate({0.0, -1.0f, 0.0f});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                camera.translate({1.0, 0.0f, 0.0f});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                camera.translate({0.0, 1.0f, 0.0f});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                camera.translate({0.0, 0.0f, 1.0f});
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                camera.translate({0.0, 0.0f, -1.0f});
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        render(result_image, color_img, height_img, camera, 512, 240, width, height);

        result_texture.update(result_image);

        window.clear();
        window.draw(result_sprite);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
