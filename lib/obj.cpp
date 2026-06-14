#include "obj.hpp"
#include "myFunc.hpp"

Button::Button(unsigned short int w, unsigned short int h, sf::Vector2f coor){
    this->isMouse_ = 0u;
    this->w = w;
    this->h = h;
    this->Coor = Coor;
}
Button::Button(){};
void Button::CheckContainMouse(const sf::Vector2f &mousepos){
    if(mousepos.x>=this->Coor.x&&mousepos.x<=this->Coor.x+this->w
    && mousepos.y>=this->Coor.y&&mousepos.y<=this->Coor.y+this->h){
        bSet(this->isMouse_,6);
    }else bClear(this->isMouse_,6);
}
void Button::CheckMousehold(){
    if(bRead(this->isMouse_,6)){
        bCopyBool(this->isMouse_,0,sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
        bCopyBool(this->isMouse_,1,sf::Mouse::isButtonPressed(sf::Mouse::Button::Right));
        bCopyBool(this->isMouse_,2,sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle));
    }
}
void Button::Update(){
    this->CheckMousehold();
}
void Button::Render(sf::RenderWindow &window,const sf::Color &color){
    std::array rec = {   sf::Vertex{this->Coor},
                            sf::Vertex{VecAddU8(this->Coor, this->w, 0.f)},
                            sf::Vertex{VecAddU8(this->Coor, 0.f, this->h)},
                            sf::Vertex{VecAddU8(this->Coor, this->w, this->h)}
                };
    rec[0].color = color;
    rec[1].color = color;
    rec[2].color = color;
    rec[3].color = color;
    window.draw(rec.data(),4,sf::PrimitiveType::TriangleStrip);
}


Slider::Slider(unsigned short int len, unsigned short int sidelen, unsigned short int button_w, sf::Vector2f coor, float start, float end){
    this->len = len;
    this->sideLen = sidelen;
    this->Coor = coor;
    this->start_val = start;
    this->end_val = end;
    this->button.w = button_w;
    this->button.h = sidelen;
    this->button.Coor = coor;
}
void Slider::CheckContainMouse(const sf::Vector2f &mousepos){
    if(mousepos.x>=this->Coor.x&&mousepos.x<=this->Coor.x+this->len+this->button.w
    && mousepos.y>=this->Coor.y&&mousepos.y<=this->Coor.y+this->sideLen){
        bSet(this->isMouse_,6);
    } else bClear(this->isMouse_,6);
}
void Slider::CheckMousehold(){
    bCopySetIf(this->isMouse_,0,sf::Mouse::isButtonPressed(sf::Mouse::Button::Left),bRead(this->isMouse_,6));
    bCopySetIf(this->isMouse_,1,sf::Mouse::isButtonPressed(sf::Mouse::Button::Right),bRead(this->isMouse_,6));
    bCopySetIf(this->isMouse_,2,sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle),bRead(this->isMouse_,6));
}
void Slider::Update(const sf::Vector2f &mousepos){
    this->CheckContainMouse(mousepos);
    this->CheckMousehold();
    if(bRead(this->isMouse_,0)){
        this->button.Coor.x = clampf(mousepos.x-this->button.w*0.5f,this->Coor.x,this->Coor.x+this->len);
    }
}
float Slider::getVal() const {
    return setInRange(this->button.Coor.x-this->Coor.x,0.f,this->len,this->start_val,this->end_val);
}
void Slider::Render(sf::RenderWindow &window,const sf::Color &color,const sf::Color &button_color){
    std::array rec = {  sf::Vertex{this->Coor},
                        sf::Vertex{VecAddU8(this->Coor, this->len+this->button.w, 0.f)},
                        sf::Vertex{VecAddU8(this->Coor, 0.f, this->sideLen)},
                        sf::Vertex{VecAddU8(this->Coor, this->len+this->button.w, this->sideLen)}
                };
    rec[0].color = color;
    rec[1].color = color;
    rec[2].color = color;
    rec[3].color = color;
    window.draw(rec.data(),4,sf::PrimitiveType::TriangleStrip);
    this->button.Render(window,button_color);
}