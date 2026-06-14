#include <SFML/Graphics.hpp>
#include "vector"
#include "iostream"
#include "cmath"
#include "sstream"
#include <bitset>
#include "../lib/myFunc.hpp"
#include "../lib/obj.hpp"

class point {
    public:
    sf::Vector2f pos;
    sf::Vector2f acel = {0.f,0.f};
    sf::Vector2f prepos;
    bool IsPinned = false;
    point(float x, float y);
};
point::point(float x, float y){
    this->pos = {x,y};
    this->prepos = {x,y};
};
struct idconnect {
    unsigned int tg1;
    unsigned int tg2;
    unsigned int len;
    idconnect(unsigned int tg1, unsigned int tg2, unsigned int len);
};
idconnect::idconnect(unsigned int tg1, unsigned int tg2, unsigned int len){
    this->tg1 = tg1;
    this->tg2 = tg2;
    this->len = len;
};
void drawline(sf::RenderWindow &window, const sf::Vector2f &a, const sf::Vector2f &b, const sf::Color color){
    std::array line = {sf::Vertex{a},sf::Vertex{b}};
    line[0].color = color;
    line[1].color = color;
    window.draw(line.data(), 2, sf::PrimitiveType::Lines);
};
void drawstring(sf::RenderWindow &window, const std::vector<point*> &points, const std::vector<idconnect*> &id,float t){
    sf::Vector2f dv;
    float d;
    for(size_t i=0; i<id.size(); i++){
        dv = points[id[i]->tg2]->pos - points[id[i]->tg1]->pos;
        d = std::hypotf(dv.x,dv.y)-id[i]->len;
        drawline(   window,
                    SmoothLinearAni(points[id[i]->tg1]->pos,points[id[i]->tg1]->prepos,t),
                    SmoothLinearAni(points[id[i]->tg2]->pos,points[id[i]->tg2]->prepos,t),
                    (hslToColor(clampf(setInRange(-d,0.f,1.f,180.f,360.f)-60.f,0.f,360.f),1.f,0.5f))
                    // points[id[i]->tg1]->pos,
                    // points[id[i]->tg2]->pos,
                    // sf::Color::White
        );
    }
}

const sf::Vector2f gravity = {0.f,200.f};
const float stringLen = 10.f;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

bool isInScreen(sf::Vector2f &a){
    return (a.x>=0&&a.x<WIDTH&&a.y>=0&&a.y<HEIGHT);
}
void KeepInScreen(point *p){
    if(p->pos.x>=WIDTH-1){p->pos.x=WIDTH-1;}
    if(p->pos.y>=HEIGHT-1){p->pos.y=HEIGHT-1;}
    if(p->pos.x<0){p->pos.x=0;}
    if(p->pos.y<0){p->pos.y=0;}
}
void RopePhysicsApply(std::vector<point*> &points,const std::vector<idconnect*> &id){
    sf::Vector2f dv;
    sf::Vector2f ch;
    float d;
    for(size_t j=0; j<=0; j++){
        for(uint8_t k=0; k<=1;k++){
            for(size_t i=(k==0)?0:id.size()-1; (k==0&&i<id.size())||(k==1&&i>=0); i+=(k==0)?1:-1){
                if(i>=id.size()){break;}
                dv = points[id[i]->tg2]->pos - points[id[i]->tg1]->pos;
                d = (dv.x==0.f&&dv.y==0.f)?0.1f:std::hypotf(dv.x,dv.y);
                ch = ((d-id[i]->len)*0.5f/d)*dv;
                points[id[i]->tg1]->pos += ch;
                points[id[i]->tg2]->pos -= ch;

            }
        }
    }
}
void CreatRopeLine(std::vector<point*> &points, std::vector<idconnect*> &id, float x, float y, float tg1, float tg2, unsigned int len){
    points.push_back(new point(x,y));
    id.push_back(new idconnect(tg1,tg2,len));
}
void CreateCloth(std::vector<point*> &points, std::vector<idconnect*> &id, float stx, float sty, int w, int h){
    points.push_back(new point(stx,sty));
    size_t currentid = 0;
    for(size_t i=0;i<h;i++){
        for(size_t j=0;j<w;j++){
            if(currentid!=0){
                points.push_back(new point(stx+float(stringLen*j),sty+float(stringLen*i)));
                if(i!=0){
                    id.push_back(new idconnect(currentid,(i-1)*w+j,stringLen));
                }
                if(j!=0){
                    id.push_back(new idconnect(currentid,i*w+j-1,stringLen));
                }}
            currentid++;
        }
    }
}
sf::Vector2f vecfloat(const sf::Vector2i &a){
    return sf::Vector2f((float)a.x,(float)a.y);
}

int main()
{   
    //Initialize
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "cloth sim");
    window.setFramerateLimit(60);

    sf::Font font("/home/dung/.local/share/fonts/Sono-SemiBold.ttf");
    sf::Text text1(font);
    text1.setCharacterSize(15);
    std::stringstream text1_;
    
    std::vector<point*> Points;
    std::vector<idconnect*> connections;
    Slider timeSlider(300,20,20,{0.f,580.f},1.f,1000.f);

    sf::Vector2f mousePos;
    namespace mouse = sf::Mouse;
    namespace key = sf::Keyboard;
    uint8_t IsStuffs1 = 0; // 0->7 ismousedown hasmousedowned ismouseinscreen haskeyPpressed Inertial IsCountingDone Constrant
    sf::Clock deltaclock;
    float dt;
    sf::Vector2f windowPos;
    sf::Vector2f Oldwinpos;
    
    float disToClosestPoint = 11.f;
    size_t idOfClosestPoint = 0;
    sf::Vector2f temv;
    float temf;
    unsigned short int clw = 1;
    unsigned short int clh = 50;
    CreateCloth(Points,connections,10.f,10.f,clw,clh);
    Points[0]->IsPinned = true;
    Points[clw-1]->IsPinned = true;
    float couter = 0.f;
    int frame_couter = 0;
    float time_slow_f = 1.f;
    //run
    while (window.isOpen())
    {
        dt = deltaclock.restart().asSeconds();
        Oldwinpos = windowPos;
        windowPos = vecfloat(window.getPosition());    
        mousePos = window.mapPixelToCoords(mouse::getPosition(window));
        bCopyBool(IsStuffs1,0,mouse::isButtonPressed(mouse::Button::Left));
        bCopyBool(IsStuffs1,2,isInScreen(mousePos));
        //event check
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto *getkey = event->getIf<sf::Event::KeyReleased>()){
                switch (getkey->scancode)
                {
                    case key::Scan::F:
                        if (bRead(IsStuffs1,0)){
                            Points[idOfClosestPoint]->IsPinned = !Points[idOfClosestPoint]->IsPinned;
                        }
                        break;
                    case key::Scan::G:
                        bToggle(IsStuffs1,4);
                        break;
                    case key::Scan::H:
                        bToggle(IsStuffs1,6);
                        break;
                }
            }
        }
        //update
        
        //points
        timeSlider.Update(mousePos);
        time_slow_f = timeSlider.getVal();
        couter += dt/time_slow_f;
        frame_couter ++;
        if(couter>=dt*(1-1/time_slow_f)){
            if(bRead(IsStuffs1,6)){
                RopePhysicsApply(Points,connections);
            }
            
            for(size_t i=0; i<Points.size(); i++){
                //input check
                Points[i]->acel = {0.f,0.f};
                if(bRead(IsStuffs1,0)&&!bRead(IsStuffs1,1)){
                    temv = mousePos - Points[i]->prepos;
                    temf = std::hypotf(temv.x,temv.y);
                    if(i==0){disToClosestPoint = temf;idOfClosestPoint = 0;}
                    if(temf<disToClosestPoint){
                        disToClosestPoint = temf;
                        idOfClosestPoint = i;
                    }
                }
                if(Points[i]->IsPinned) Points[i]->pos = Points[i]->prepos;
                else{
                    temv = Points[i]->pos;
                    //inertial
                    if (bRead(IsStuffs1,4)){
                        Points[i]->acel += gravity;
                        Points[i]->pos = 2.f*Points[i]->pos-Points[i]->prepos+Points[i]->acel*dt*dt;
                        KeepInScreen(Points[i]);
                    }
                    Points[i]->prepos = temv;
                }
            }
            if(bRead(IsStuffs1,0)&&bRead(IsStuffs1,2)&&disToClosestPoint<=10.f){
                Points[idOfClosestPoint]->pos = mousePos;
                Points[idOfClosestPoint]->prepos = mousePos;
            }
            bCopyBool(IsStuffs1, 1, bRead(IsStuffs1,0));
            bSet(IsStuffs1,5);
        }
    
        //debug
        text1_.str("");
        text1_<<"fps: "<<1/dt
        <<"\nmouse: "<<mousePos.x<<","<<mousePos.y
        <<"\nwindow:"<<(windowPos - Oldwinpos).x<<","<<(windowPos - Oldwinpos).y
        <<"\ncurrent id: "<<idOfClosestPoint
        <<"\ndis: "<<disToClosestPoint
        <<"\nclick?: "<<std::bitset<8>(IsStuffs1)
        <<"\nThing?:"<<std::bitset<8>(timeSlider.isMouse_)
        <<"\nt:"<<(float)frame_couter/time_slow_f
        <<"\nVal:"<<timeSlider.getVal();
        // for(size_t i=0; i<Points.size(); i++){
            //     text1_ << "id:" << i << " vel:" << Points[i]->acel.x << ";" << Points[i]->acel.y << "\n";
        // }
        text1.setString(text1_.str());
        //render
        window.clear();
        drawstring(window,Points,connections,(float)frame_couter/time_slow_f);
        timeSlider.Render(window,sf::Color(100,100,100),sf::Color(255,255,255));

        if(bRead(IsStuffs1,5)){
            couter = 0.f;
            frame_couter = 0;
            bClear(IsStuffs1,5);
        }
        
        window.draw(text1);
        window.display();
    }


    for(size_t i=0;i<Points.size();i++){
        delete Points[i];
        //std::cout<<"data index: "<<i<<" of points vector deleted"<<"\n";
    }
    for(size_t i=0;i<connections.size();i++){
        delete connections[i];
        //std::cout<<"data index: "<<i<<" of connections vector deleted"<<"\n";
    }

    return 0;
}