#pragma once

#include "SFML/Graphics.hpp"
#include "cmath"

float my_modf(float a, float b);
float absf(float a);
void bSet(uint8_t &a, uint8_t n);
void bClear(uint8_t &a, uint8_t n);
void bToggle(uint8_t &a, uint8_t n);
void bCopyBool(uint8_t &a, uint8_t n, const bool &b);
void bCopySetIf(uint8_t &a, uint8_t n, const bool &b, const bool if_val);
uint8_t bRead(const uint8_t &a, uint8_t n);
sf::Color hslToColor(float h, float s, float l);
float clampf(const float a, float min, float max);
float setInRange(const float val,float val_start, float val_end, float start, float end);
sf::Vector2f VecAddfloat(sf::Vector2f a, float x, float y);
sf::Vector2f VecAddU8(sf::Vector2f a, unsigned short int x, unsigned short int y);
sf::Vector2f SmoothLinearAni(sf::Vector2f nowPos, sf::Vector2f PrePos, float t);
