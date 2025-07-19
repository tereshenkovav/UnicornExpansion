#include "Countdown.h"

namespace sf {

const float NOREACH=-1.0f ;

Countdown::Countdown(void)
{
	left=NOREACH ;
	oncereach=false ;
}

void Countdown::upset(float interval) 
{
	left=interval ;
	oncereach = false ;
}

void Countdown::update(float dt) 
{
	if (!isActive()) return ;
	left-=dt ;
	if (!isActive()) oncereach=true ;	
}

bool Countdown::isActive() 
{
	return left>0 ;
}

bool Countdown::onceReachNol() 
{
	if (oncereach) 
	{
		oncereach=false ;
		return true ;
	}
	return false ;
}

void Countdown::reset() 
{
	left=NOREACH ;
	oncereach=false ;
}

Countdown::~Countdown(void)
{
}

}