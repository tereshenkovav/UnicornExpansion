#include "GameUnit.h"
#include "Game.h"
#include "WayFinder.h"
#include <algorithm>
#include <math.h>
#include <typeinfo>
#include "CppTools.h"

GameUnit::GameUnit(int unitx, int unity, int unitw, int unith, const std::string& caption, int maxhealth, const std::string& code)
{
	uid = rand();

	this->unitx = unitx ;
	this->unity = unity ;
	this->unitw = unitw ;
	this->unith = unith ;

	this->maxhealth = maxhealth;
	this->health = maxhealth;
	this->code = code;
	
	worktek = -1.0f;
	workall = 1.0f;

	v = 0 ;
	treach = 1.0f;
	this->caption = caption;

	moving = Moving::None ;
	lastmoving = Moving::Up ;
	tmoving = 0.0f ;

	targeted = false ;
}

GameUnit::GameUnit() {
	uid = -1;
	this->code = "No unit";
	this->caption = "No unit";
}

int GameUnit::getMovingDx() const {
	if (moving==Moving::Left) return -1 ;
	if (moving==Moving::Right) return 1 ;
	return 0;
}

int GameUnit::getMovingDy() const {
	if (moving==Moving::Up) return -1 ;
	if (moving==Moving::Down) return 1 ;
	return 0;
}

sf::Vector2f GameUnit::getView() const {
	return sf::Vector2f({ unitx * BLOCKW + unitw * BLOCKW / 2.0f + tmoving * v * getMovingDx(),
		unity * BLOCKH + unith * BLOCKH / 2.0f + tmoving * v * getMovingDy() });
}

sf::Vector2f GameUnit::getSizeView() const {
	return sf::Vector2f({ (float)unitw* BLOCKW, (float)unith* BLOCKH });
}

sf::Vector2i GameUnit::getSize() const {
	return sf::Vector2i({ unitw,  unith });
}

sf::Vector2i GameUnit::getXY() const {
	return sf::Vector2i(unitx, unity);
}

int GameUnit::getUID() const
{
	return uid;
}

std::string GameUnit::getHealthInfo() const
{
	// Исправление для запрета вывода нулевого значения
	int int_health = ((int)health < 1) ? 1 : (int)health;
	return std::to_string(int_health) + "/" + std::to_string(maxhealth);
}

float GameUnit::getHealthPerMax() const
{
	int int_health = ((int)health < 1) ? 1 : (int)health;
	return int_health / (float)maxhealth;
}

sf::Vector2i GameUnit::getTarget() const {
	return sf::Vector2i(targetx,targety) ;
}

bool GameUnit::isXYInUnit(float x, float y) const
{
	return (getView().x - getSizeView().x / 2.0f < x) && (x < getView().x + getSizeView().x / 2.0f) &&
		(getView().y - getSizeView().y / 2.0f < y) && (y < getView().y + getSizeView().y / 2.0f);
}

Moving GameUnit::getLastMoving() const
{
	return lastmoving;
}

bool GameUnit::isUnitNearUnit(const GameUnit& unit) const
{
	for (int i1 = 0; i1 < unitw; i1++)
		for (int j1 = 0; j1 < unith; j1++)
			for (int i2 = 0; i2 < unit.unitw; i2++)
				for (int j2 = 0; j2 < unit.unith; j2++) {
					int x1 = unitx + i1;
					int y1 = unity + j1;
					int x2 = unit.unitx + i2;
					int y2 = unit.unity + j2;
					if (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) < 2.0f) return true;
				}
	return false;
}

void GameUnit::moveTo(Moving moving) {
	this->moving = moving ;
	if (moving != Moving::None) this->lastmoving = moving;
	tmoving = 0.0f ;
}

bool GameUnit::isMoving() const {
	return moving!=Moving::None ;
}

bool GameUnit::isTargeted() const {
	return targeted ;
}

void GameUnit::setTarget(int x, int y) {
	// Не учитываем размеры юнита
	if ((x == unitx) && (y == unity)) return;
	// Запрет на движение при выполнении действия (сейчас отключен)
	//if (worktek >= 0) return;
	targetx = x ;
	targety = y ;
	targeted = true ;
}
	
void GameUnit::resetTarget() {
	targeted = false ;
}

std::string GameUnit::getCaption() const
{
	return caption;
}

int GameUnit::getVelocity() const
{
	return v;
}

void GameUnit::setVelocity(int v)
{
	this->v = v;
	treach = (float)BLOCKW / v;
}

void GameUnit::update(float dt) {
	if (worktek >= 0) {
		worktek += dt;
		if (worktek >= workall) {
			// Здесь жестко задано действие после завершения работы
			worktek = -1.0f;
			workaction.component->applyAction(workaction);
		}
	}
	if (moving!=Moving::None) {
		tmoving+=dt ;
		if (tmoving>=treach) {
			unitx+=getMovingDx() ;
			unity+=getMovingDy() ;
			moving=Moving::None ;
			tmoving=0.0f ;
			if (targeted) {
				// Не учитываем размеры юнита
				if ((unitx==targetx)&&(unity==targety)) resetTarget() ;
				// Остановим движение при поступившем действии (сейчас отключено)
				//if (worktek >= 0) resetTarget();
			}
		}
	}
	for (auto i = 0; i < components.size(); i++)
		components[i]->update(dt);
}
	
void GameUnit::updateBusyMap(BusyMap & busymap) const {
	for (int i=0; i<unitw; i++)
		for (int j=0; j<unith; j++) {
			busymap.setBusy(unitx+i,unity+j) ;
			busymap.setBusy(unitx+i+getMovingDx(),unity+j+getMovingDy()) ;
		}
}

bool GameUnit::canSendAction(const UnitAction& action, std::string* msgcode) const
{
	return action.component->canApplyAction(action,msgcode);
}

void GameUnit::decHealth(float value)
{
	health -= value;
	if (health < 0.0f) health = 0.0f;
}

void GameUnit::incHealth(float value)
{
	health += value;
	if (health > maxhealth) health = maxhealth;
}

bool GameUnit::isKilled() const
{
	return health <= 0.0f;
}

std::string GameUnit::getComponentsInfo() const
{
	std::string res = "";
	for (int i = 0; i < components.size(); i++) {
		std::string info = components[i]->getComponentInfo();
		if (info.length() > 0) {
			if (res.length() > 0) res += "\n";
			res += info;
		}
	}
	return res;
}

bool GameUnit::hasComponentByName(const std::string& code) const
{
	for (int i = 0; i < components.size(); i++)
		if (hasEnding(std::string(typeid(*components[i]).name()),code)) return true;
	return false;
}

std::vector<UnitAction> GameUnit::getActions() const
{
	std::vector<UnitAction> actions;
	for (auto comp: components)
	{
		const auto addactions = comp->getActions();
		actions.insert(actions.end(), addactions.begin(), addactions.end());
	}
	return actions;
}

bool GameUnit::sendAction(const UnitAction & action) {
	if (action.component->doPrefixAction(action)) {
		worktek = 0.0f;
		workall = action.time;
		workaction = action;
		return true;
	}
	else
		return false;
}

void GameUnit::addComponent(UnitComponent* comp)
{
	comp->setUnitID(uid);
	components.push_back(comp);
	std::sort(components.begin(), components.end(), [](const UnitComponent * a, const UnitComponent * b) {
		return a->getOrder() < b->getOrder(); });

}

bool GameUnit::isWorkingTask(float* progress, std::string* actioncode) const
{
	if (worktek < 0) return false;
	*progress = worktek/workall;
	*actioncode = workaction.code;
	return true;
}

bool GameUnit::isWorkingTask() const
{
	return (worktek >= 0);
}

GameUnit::~GameUnit()
{
}

std::string GameUnit::getCode() const
{
	std::string postfix = "";
	for (auto comp : components)
		postfix += comp->getCodePostfix();
	if (postfix.length() == 0) return code; else return code + "_" + postfix;
}
