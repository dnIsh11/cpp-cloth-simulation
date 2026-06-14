#pragma once

#include <SFML/Graphics.hpp>

class Button{
    public:
        unsigned short int w;
        unsigned short int h;
        sf::Vector2f Coor;
        uint8_t isMouse_;  // lefthold righthold midhold hasleftclicked hasrightclicked hasmidclick ismousein hasmousein
        Button(unsigned short int w, unsigned short int h, sf::Vector2f coor);
        Button();
        void CheckMousehold();
        void CheckContainMouse(const sf::Vector2f &mousepos);
        void Update();
        void Render(sf::RenderWindow &window,const sf::Color &color);
};
class Slider{
    private:
        Button button;
        unsigned short int len;
        unsigned short int sideLen;
        sf::Vector2f Coor;
        float start_val;
        float end_val;
    public:
        uint8_t isMouse_;   // lefthold righthold midhold hasleftclicked hasrightclicked hasmidclick ismousein hasmousein
        uint8_t isState_;
        Slider(unsigned short int len, unsigned short int sidelen , unsigned short int button_w , sf::Vector2f coor, float start, float end);
        void CheckMousehold();
        void CheckContainMouse(const sf::Vector2f &mousepos);
        float getVal() const;
        void Update(const sf::Vector2f &mousepos);
        void Render(sf::RenderWindow &window,const sf::Color &color,const sf::Color &button_color);
};