#pragma once

/*
  Включаемый файл для регистрации типа Game в скриптах.
  Со временем можно сделать его целиком или полностью автогенерируемым 
  на основе описания класса, который подключаем к скрипту.
*/

// Ссылка на глобальный объект игры, с которым работаем в скрипте
Game * current_game;

Game* Game_new(zetscript::ScriptEngine* _script_engine) {
	ZS_UNUSUED_PARAM(_script_engine);
	return current_game;
}

zetscript::zs_int Game_addPortal(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addPortal(x, y);
}

zetscript::zs_int Game_addAcademy(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addAcademy(x, y);
}

zetscript::zs_int Game_addMachinary(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addMachinary(x, y);
}

zetscript::zs_int Game_addEnemy1(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addEnemy1(x, y);
}

zetscript::zs_int Game_addEnemy2(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addEnemy2(x, y);
}

zetscript::zs_int Game_addEnemy3(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addEnemy3(x, y);
}

zetscript::zs_int Game_addCrystal(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int hp) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addCrystal(x, y, hp);
}

zetscript::zs_int Game_addCrystalRich(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int hp) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addCrystalRich(x, y, hp);
}

zetscript::zs_int Game_addCrystalPoor(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int hp) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addCrystalPoor(x, y, hp);
}

zetscript::zs_int Game_addEmptyUnit(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int hp, zetscript::StringScriptObject* name, zetscript::StringScriptObject* code) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addEmptyUnit(x, y, hp, name->getConstChar(), code->getConstChar());
}

zetscript::zs_int Game_addUnicorn(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addUnicorn(x, y, _this->getConfigUnit()["Unicorn"]["InitialHP"].asInt());
}

zetscript::zs_int Game_addMachine(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addMachine(x, y);
}

zetscript::zs_int Game_addLair(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::StringScriptObject* seq) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	return factory.addLair(x, y, seq->getConstChar());
}

void Game_deleteUnitLater(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int uid) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->deleteUnitLater(uid);
}

void Game_incEnergy(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int value) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->incEnergy(value);
}

void Game_decEnergy(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int value) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->decEnergy(value);
}

zetscript::zs_int  Game_getEnergy(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->getEnergy();
}

zetscript::zs_int  Game_getTimer(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->getTimer();
}

void Game_startTimer(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int value) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->startTimer(value);
}

void Game_startHiddenTimer(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int value) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->startHiddenTimer(value);
}

void Game_addNewTask(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* code, zetscript::StringScriptObject* text) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->addNewTask(code->getConstChar(), text->getConstChar());
}

void Game_setTaskCompleted(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* code) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setTaskCompleted(code->getConstChar());
}

void Game_setTaskCancelled(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* code) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setTaskCancelled(code->getConstChar());
}

bool Game_isTaskCompleted(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* code) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isTaskCompleted(code->getConstChar());
}

zetscript::zs_int Game_getCountByComponent(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* value) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->getCountByComponent(value->getConstChar());
}

bool Game_isMushroomsExist(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isMushroomsExist();
}

void Game_setNewViewPoint(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setNewViewPoint(x,y);
}

void Game_addMessage(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* icon, zetscript::zs_int duration, zetscript::StringScriptObject* text) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->addMessage(icon->getConstChar(),duration,text->getConstChar());
}

void Game_addMessageLater(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* icon, zetscript::zs_int duration, zetscript::zs_int delay, zetscript::StringScriptObject* text) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->addMessageLater(icon->getConstChar(),duration,delay,text->getConstChar());
}

zetscript::StringScriptObject* Game_getText(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* name) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _script_engine->newStringScriptObject(_this->getText(name->getConstChar()).data());
}

void Game_allowAction(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* name) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->allowAction(name->getConstChar());
}

void Game_denyAction(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* name) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->denyAction(name->getConstChar());
}

void Game_setFlag(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* name) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setFlag(name->getConstChar());
}

void Game_clearFlag(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* name) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->clearFlag(name->getConstChar());
}

bool Game_isFlag(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* name) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isFlag(name->getConstChar());
}

bool Game_isHard1(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isHard1() ;
}

bool Game_isHard2(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isHard2() ;
}

bool Game_isUnitNearXY(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int uid, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isUnitNearXY(uid,x,y);
}

bool Game_isUnitNearXYDist(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int uid, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int dist) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isUnitNearXYDist(uid,x,y,dist);
}

zetscript::zs_int Game_getUnitHealth(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int uid) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->getUnitHealth(uid);
}

bool Game_isUnitExist(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int uid) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->isUnitExist(uid);
}

void Game_setTargetToUnit(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int uid, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setTargetToUnit(uid,x,y);
}

void Game_clearFogAt(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int dist) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->clearFogAt(x,y,dist);
}

void Game_delete(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	// empty
}

// Метод вызываем при регистрации объекта в скрипте
void registerTypeGameInScript(zetscript::ScriptEngine & engine) {
	engine.registerType<Game>("Game", Game_new, Game_delete);
	engine.registerMemberFunction<Game>("addPortal", &Game_addPortal);
	engine.registerMemberFunction<Game>("addAcademy", &Game_addAcademy);
	engine.registerMemberFunction<Game>("addMachinary", &Game_addMachinary);
	engine.registerMemberFunction<Game>("addEnemy1", &Game_addEnemy1);
	engine.registerMemberFunction<Game>("addEnemy2", &Game_addEnemy2);
	engine.registerMemberFunction<Game>("addEnemy3", &Game_addEnemy3);
	engine.registerMemberFunction<Game>("addCrystal", &Game_addCrystal);
	engine.registerMemberFunction<Game>("addCrystalRich", &Game_addCrystalRich);
	engine.registerMemberFunction<Game>("addCrystalPoor", &Game_addCrystalPoor);
	engine.registerMemberFunction<Game>("addEmptyUnit", &Game_addEmptyUnit);
	engine.registerMemberFunction<Game>("addUnicorn", &Game_addUnicorn);
	engine.registerMemberFunction<Game>("addMachine", &Game_addMachine);
	engine.registerMemberFunction<Game>("addLair", &Game_addLair);
	engine.registerMemberFunction<Game>("incEnergy", &Game_incEnergy);
	engine.registerMemberFunction<Game>("decEnergy", &Game_decEnergy);
	engine.registerMemberFunction<Game>("getEnergy", &Game_getEnergy);
	engine.registerMemberFunction<Game>("addNewTask", &Game_addNewTask);
	engine.registerMemberFunction<Game>("setTaskCompleted", &Game_setTaskCompleted);
	engine.registerMemberFunction<Game>("setTaskCancelled", &Game_setTaskCancelled);
	engine.registerMemberFunction<Game>("isTaskCompleted", &Game_isTaskCompleted);
	engine.registerMemberFunction<Game>("getCountByComponent", &Game_getCountByComponent);
	engine.registerMemberFunction<Game>("isMushroomsExist", &Game_isMushroomsExist);
	engine.registerMemberFunction<Game>("getTimer", &Game_getTimer);
	engine.registerMemberFunction<Game>("startTimer", &Game_startTimer);
	engine.registerMemberFunction<Game>("startHiddenTimer", &Game_startHiddenTimer);
	engine.registerMemberFunction<Game>("setNewViewPoint", &Game_setNewViewPoint);
        engine.registerMemberFunction<Game>("addMessage", &Game_addMessage);
        engine.registerMemberFunction<Game>("addMessageLater", &Game_addMessageLater);
        engine.registerMemberFunction<Game>("getText", &Game_getText);
        engine.registerMemberFunction<Game>("allowAction", &Game_allowAction);
        engine.registerMemberFunction<Game>("denyAction", &Game_denyAction);
        engine.registerMemberFunction<Game>("setFlag", &Game_setFlag);
        engine.registerMemberFunction<Game>("clearFlag", &Game_clearFlag);
        engine.registerMemberFunction<Game>("isFlag", &Game_isFlag);
        engine.registerMemberFunction<Game>("isHard1", &Game_isHard1);
        engine.registerMemberFunction<Game>("isHard2", &Game_isHard2);
		engine.registerMemberFunction<Game>("isUnitNearXYDist", &Game_isUnitNearXYDist);
		engine.registerMemberFunction<Game>("isUnitNearXY", &Game_isUnitNearXY);
		engine.registerMemberFunction<Game>("getUnitHealth", &Game_getUnitHealth);
		engine.registerMemberFunction<Game>("isUnitExist", &Game_isUnitExist);
		engine.registerMemberFunction<Game>("deleteUnitLater", &Game_deleteUnitLater);
		engine.registerMemberFunction<Game>("setTargetToUnit", &Game_setTargetToUnit);
	engine.registerMemberFunction<Game>("clearFogAt", &Game_clearFogAt);
}
