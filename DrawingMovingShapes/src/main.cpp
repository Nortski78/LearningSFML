#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <filesystem>
#include <vector>

class MyShape {

    std::string m_type = " ", m_name = " ";
    float m_xpos = 0.0f, m_ypos = 0.0f, m_xvel = 0.0f, m_yvel = 0.0f, m_width = 0.0f, m_height = 0.0f, m_radius = 0.0f;
    int m_red = 255, m_green = 255, m_blue = 255;
    std::shared_ptr<sf::Shape> shape;

public:
    MyShape(const std::string objectType, const std::string name, float xpos, float ypos, float xvel, float yvel, int r, int g, int b, float radius )
        : m_type(objectType)
        , m_name(name)
        , m_xpos(xpos)
        , m_ypos(ypos)
        , m_xvel(xvel)
        , m_yvel(yvel)
        , m_red(r)
        , m_green(g)
        , m_blue(b)
        , m_radius(radius)
    {
        if (m_type == "Circle") {
            shape = std::make_shared<sf::CircleShape>(m_radius);
            shape->setFillColor(sf::Color(m_red, m_green, m_blue));
            shape->setPosition(m_xpos, m_ypos);
        }
    }

    MyShape(const std::string objectType, const std::string name, float xpos, float ypos, float xvel, float yvel, int r, int g, int b, float width, float height)
        : m_type(objectType)
        , m_name(name)
        , m_xpos(xpos)
        , m_ypos(ypos)
        , m_xvel(xvel)
        , m_yvel(yvel)
        , m_red(r)
        , m_green(g)
        , m_blue(b)
        , m_width(width)
        , m_height(height)
    {
        if (m_type == "Rectangle") {
            shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(m_width, m_height));
            shape->setFillColor(sf::Color(m_red, m_green, m_blue));
            shape->setPosition(m_xpos, m_ypos);
        }
    }    
    
    const std::shared_ptr<sf::Shape> getShape() {
        return shape;
    }

    void updatePosition(sf::Vector2u winSize) {
        int winWidth = winSize.x;
        int winHeight = winSize.y;

        if (shape->getPosition().x < 0.0f || shape->getPosition().x > (winWidth - (m_width + m_radius * 2))) {
            m_xvel *= -1.0f;
        }
        if (shape->getPosition().y < 0.0f || shape->getPosition().y > (winHeight - (m_height + m_radius * 2))) {
            m_yvel *= -1.0f;
        }

        shape->setPosition(shape->getPosition().x + m_xvel, shape->getPosition().y + m_yvel);
    }
};

void loadFromFile(const std::string filename, sf::RenderWindow& window, std::vector<std::shared_ptr<MyShape>>& shapes) {
    std::ifstream fin(filename);

    float winHeight, winWidth, width, height, radius, xpos, ypos, xvel, yvel;
    int r, g, b;
    std::string objectType, name;

    while (fin >> objectType) {
        if (objectType == "Window") {
            fin >> winHeight >> winWidth;
            window.create(sf::VideoMode(winHeight, winWidth), "My window");
        }
        else if (objectType == "Circle") {
            fin >> name >> xpos >> ypos >> xvel >> yvel >> r >> g >> b >> radius;
            std::shared_ptr<MyShape> circle = std::make_shared<MyShape>(objectType, name, xpos, ypos, xvel, yvel, r, g, b, radius);
            shapes.push_back(circle);
        }
        else if (objectType == "Rectangle") {
            fin >> name >> xpos >> ypos >> xvel >> yvel >> r >> g >> b >> width >> height;
            std::shared_ptr<MyShape> rect = std::make_shared<MyShape>(objectType, name, xpos, ypos, xvel, yvel, r, g, b, width, height);
            shapes.push_back(rect);
        }
    }

    fin.close();
}

int main()
{
    std::vector<std::shared_ptr<MyShape>> myShapes;
    sf::RenderWindow window;    

    loadFromFile("src/config.txt", window, myShapes);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear, draw, display 
        window.clear(sf::Color::Black);
        for (auto& shape : myShapes) {
            window.draw(*shape->getShape());
            shape->updatePosition(window.getSize());
        }
        window.display();
    }

    return 0;
}