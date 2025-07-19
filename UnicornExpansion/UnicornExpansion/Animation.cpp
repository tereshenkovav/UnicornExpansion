#include "Animation.h"

Animation::Animation(const std::string& filename, int w, int h, int framecount, int fps): sf::Sprite(deftex)
{
    this->count = framecount;

    this->fps = fps ;
    tekt = 0 ;
    onceflag = false ;
        
    frames = new sf::Texture[count] ;
    sf::Image source ;
    source.loadFromFile(filename) ;

    sf::IntRect rect ;
    if ((w<0)||(h<0)) {
        w = source.getSize().x/count ;
        h = source.getSize().y ;
    }
    rect.position = { 0, 0 };
    rect.size = { w, h };
    
    for (int i=0; i<count; i++) {
        frames[i].loadFromImage(source,false,rect) ;
        frames[i].setSmooth(true);
        rect.position.x+=w ;
        if (rect.position.x>=(int)source.getSize().x) {
            rect.position.x=0 ;
            rect.position.y+=h ;
        }
    }

    // Здесь обязательно передать нулевой файл обратно к предку - он там используется
    setTexture(frames[0],true);
    
    isplayed = true ;
    update(0) ;
    isplayed = false ;
}

void Animation::update(double dt)
{
    if (!isplayed) return ;

    float sec_per_frame = 1.0f/fps ;

    tekt+=dt ;
    int tekframe = (int)(tekt/sec_per_frame) ;
    if (tekframe>=count) {
        if (onceflag) {
            tekframe = count-1 ;
            isplayed = false ;
        }
        else {
            tekframe = 0 ;
            tekt-=count*sec_per_frame ;
        }
    }
    setTexture(frames[tekframe],true) ;
}

int Animation::getFrame() const
{
    return (int)(tekt * fps);
}

void Animation::play()
{
    isplayed = true ;
    onceflag = false ;
}

void Animation::stop()
{
    isplayed = false ;
}

bool Animation::isPlayed()
{
    return isplayed ;
}

void Animation::playOneTime()
{
    isplayed = true ;
    onceflag = true ;
    tekt=0 ;
    update(0) ;
}
