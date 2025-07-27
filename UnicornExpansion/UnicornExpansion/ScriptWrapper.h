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

void Game_addPortal(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addPortal(x, y);
}

void Game_addEnemy1(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addEnemy1(x, y);
}

void Game_addEnemy2(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addEnemy2(x, y);
}

void Game_addEnemy3(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addEnemy3(x, y);
}

void Game_addCrystal(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::zs_int hp) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addCrystal(x, y, hp);
}

void Game_addUnicorn(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addUnicorn(x, y, _this->getConfigUnit()["Unicorn"]["InitialHP"].asInt());
}

void Game_addLair(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y, zetscript::StringScriptObject* seq) {
	ZS_UNUSUED_PARAM(_script_engine);
	UnitFactory factory(_this);
	factory.addLair(x, y, seq->getConstChar());
}

void Game_incEnergy(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int value) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->incEnergy(value);
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
	return _this->startTimer(value);
}

void Game_setTaskText(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* value) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setTaskText(value->getConstChar());
}

zetscript::zs_int Game_getCountByComponent(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::StringScriptObject* value) {
	ZS_UNUSUED_PARAM(_script_engine);
	return _this->getCountByComponent(value->getConstChar());
}

void Game_setNewViewPoint(zetscript::ScriptEngine* _script_engine, Game* _this, zetscript::zs_int x, zetscript::zs_int y) {
	ZS_UNUSUED_PARAM(_script_engine);
	_this->setNewViewPoint(x,y);
}

void Game_delete(zetscript::ScriptEngine* _script_engine, Game* _this) {
	ZS_UNUSUED_PARAM(_script_engine);
	// empty
}

// Метод вызываем при регистрации объекта в скрипте
void registerTypeGameInScript(zetscript::ScriptEngine & engine) {
	engine.registerType<Game>("Game", Game_new, Game_delete);
	engine.registerMemberFunction<Game>("addPortal", &Game_addPortal);
	engine.registerMemberFunction<Game>("addEnemy1", &Game_addEnemy1);
	engine.registerMemberFunction<Game>("addEnemy2", &Game_addEnemy2);
	engine.registerMemberFunction<Game>("addEnemy3", &Game_addEnemy3);
	engine.registerMemberFunction<Game>("addCrystal", &Game_addCrystal);
	engine.registerMemberFunction<Game>("addUnicorn", &Game_addUnicorn);
	engine.registerMemberFunction<Game>("addLair", &Game_addLair);
	engine.registerMemberFunction<Game>("incEnergy", &Game_incEnergy);
	engine.registerMemberFunction<Game>("getEnergy", &Game_getEnergy);
	engine.registerMemberFunction<Game>("setTaskText", &Game_setTaskText);
	engine.registerMemberFunction<Game>("getCountByComponent", &Game_getCountByComponent);
	engine.registerMemberFunction<Game>("getTimer", &Game_getTimer);
	engine.registerMemberFunction<Game>("startTimer", &Game_startTimer);
	engine.registerMemberFunction<Game>("setNewViewPoint", &Game_setNewViewPoint);
}
