#include "HelperCppClasses/Countdown.h"

Countdown::Countdown(void)
{
	reset() ;
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

bool Countdown::isActive() const
{
	return left>0.0f ;
}

bool Countdown::onceReachNol() 
{
	if (!oncereach) return false ;
	oncereach=false ;
	return true ;
}

void Countdown::reset() 
{
	left=0.0f ;
	oncereach=false ;
}
