#include "myFunc.hpp"

float my_modf(float a, float b){
    return a-floorf(a/b)*b;
}
float absf(float a){
    if(a>=0)    return a;
    else        return -a;
}
void bSet(uint8_t &a, uint8_t n){
    a = a|(1u<<n);
}
void bClear(uint8_t &a, uint8_t n){
    a = a&~(1u<<n);
}
void bToggle(uint8_t &a, uint8_t n){
    a = a^(1u<<n);
}
void bCopyBool(uint8_t &a, uint8_t n, const bool &b){
    a = a&~(1u<<n)|(b<<n);
}
void bCopySetIf(uint8_t &a, uint8_t n, const bool &b, const bool if_val){
    if(if_val){bCopyBool(a,n,b);}
    else if(!b){bClear(a,n);}
}
uint8_t bRead(const uint8_t &a, uint8_t n){
    return (a>>n)&1u;
}
sf::Color hslToColor(float h, float s, float l){
    float rgb_t[3];
    float c = (1.f-absf(2.f*l-1.f))*s;
    float x = c*(1.f-absf(my_modf(h/60.f,2.f)-1.f));
    float m = l-c*0.5f;
    if(h>=300.f){
        rgb_t[0] = c;
        rgb_t[1] = 0.f;
        rgb_t[2] = x;
    }else if(h>=240.f){
        rgb_t[0] = x;
        rgb_t[1] = 0.f;
        rgb_t[2] = c;
    }else if(h>=180.f){
        rgb_t[0] = 0.f;
        rgb_t[1] = x;
        rgb_t[2] = c;
    }else if(h>=120.f){
        rgb_t[0] = 0.f;
        rgb_t[1] = c;
        rgb_t[2] = x;
    }else if(h>=60.f){
        rgb_t[0] = x;
        rgb_t[1] = c;
        rgb_t[2] = 0.f;
    }else if(h>=0.f){
        rgb_t[0] = c;
        rgb_t[1] = x;
        rgb_t[2] = 0.f;
    }
    return sf::Color(static_cast<uint8_t>((rgb_t[0]+m)*255u),
                     static_cast<uint8_t>((rgb_t[1]+m)*255u),
                     static_cast<uint8_t>((rgb_t[2]+m)*255u));
}
float clampf(const float a, float min, float max){
    if(a<=min) return min;
    if(a>=max) return max;
    return a;
}
float setInRange(const float val,float val_start, float val_end, float start, float end){
    return start + (val-val_start)*(end-start)/(val_end-val_start);
}

sf::Vector2f VecAddfloat(sf::Vector2f a, float x, float y){
    return sf::Vector2f{a.x+x,a.y+y};
}

sf::Vector2f VecAddU8(sf::Vector2f a, unsigned short int x, unsigned short int y){
    return sf::Vector2f{a.x+(float)x,a.y+(float)y};
}
sf::Vector2f SmoothLinearAni(sf::Vector2f nowPos, sf::Vector2f PrePos, float t){
    return PrePos - (PrePos - nowPos)*clampf(t,0.f,1.f);
}