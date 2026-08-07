#include "SceneGame.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneTask.h"
#include "SceneJournal.h"
#include "SceneEndGame.h"
#include "SceneGameMenu.h"
#include "ComponentMachine.h"
#include "ComponentMultiselect.h"
#include "SceneMsgBox.h"
#include "CompanyInfo.h"
#include "ComponentMovable.h"
#include "VolumeCalculator.h"

// Размеры камеры и скорость прокрутки камеры
const int SCROLLSPEED = 10;
const float VIEW_SIZE_X = 1024.0f;
const float VIEW_SIZE_Y = 576.0f;

// Процедуры без привязки к игре
// Градиент цвета по остатку здоровья
sf::Color getColorByHPNorm(float hp_norm) {
    if (hp_norm < 0.5f) return sf::Color::Red;
    if (hp_norm < 0.75f) return sf::Color(255, 255, 0);
    return sf::Color::Green;
}

sf::Vector2f getActionButtonPos(int i) {
    const int ACTION_COL_COUNT = 5;
    return { (float)(1024 - 400 + 8 + (i % ACTION_COL_COUNT) * 76), (float)(768 - 192 + 10 + (i / ACTION_COL_COUNT) * 72) };
}
// Конец

SceneGame::SceneGame(std::string company, int leveln, Difficulty difficulty) {
    this->levelcode = { company, leveln, difficulty };
}

SceneGame::SceneGame(LevelCode levelcode) {
    this->levelcode = levelcode;
}

void SceneGame::doScalePlus() {
    if (tekscale < 6) {
        tekscale++;
        updateScale();
    }
}

void SceneGame::doScaleMinus() {
    if (tekscale > 0) {
        tekscale--;
        updateScale();
    }
}

// Обновление мини-карты
void SceneGame::updateMiniMap() {
    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++) {
            if (game.isFog(i, j)||(!showterrain))
                minimap.setCellColor(i, j, sf::Color::Black);
            else {
                if (color_terrains.count(game.getMap(i, j)) > 0) minimap.setCellColor(i, j, color_terrains[game.getMap(i, j)]);
                // Затемнение грибами, только рельефа
                int stage = game.getMushrooms(i, j).size();
                // Здесь зависимость прозрачности от стадии грибов - считаем, что максимальная плотность это 4
                if (stage > 0) minimap.addCellColor(i, j, sf::Color{ 160,56,255,uint8_t(64 + 32 * stage) });
            }
        }
    
    // Работаем со всеми юнитами, без деления на свои и чужие
    for (int i = 0; i < game.getUnitCount(); i++)
        for (int dx = 0; dx < game.getUnit(i).getSize().x; dx++)
            for (int dy = 0; dy < game.getUnit(i).getSize().y; dy++) {
                if (game.isFog(game.getUnit(i).getXY().x + dx, game.getUnit(i).getXY().y + dy)) continue;
                sf::Color color;
                if (selector.isUnitSelected(game.getUnit(i).getUID())) {
                    color = sf::Color::Green;
                    if (game.getUnit(i).isComponent<ComponentResource>()) color = sf::Color::Blue;
                    if (game.getUnit(i).isComponent<ComponentEnemy>()) color = sf::Color::Red;
                }
                else {
                    color = sf::Color{ 0,192,0 };
                    if (game.getUnit(i).isComponent<ComponentResource>()) color = sf::Color{ 0,0,192 };
                    if (game.getUnit(i).isComponent<ComponentEnemy>()) color = sf::Color{ 192,0,0 };
                }
                minimap.setCellColor(game.getUnit(i).getXY().x + dx, game.getUnit(i).getXY().y + dy, color);
            }

    minimap.setCenter(view.getCenter().x, view.getCenter().y);
}


float SceneGame::getScale05per20() {
    return 0.5f + tekscale * 0.25f;
}

// Рисование лазера и его анимации в точке приложения
void SceneGame::drawLaserFromTo(sf::RenderTarget& rendertarget, sfge::UniqueSprite & spr_laz, const Laser& laz) {
    sf::Vector2f dir = laz.pos2 - laz.pos1;
    spr_laz->setColor(sfge::SfmlTools::getColorAsBright(color_lasers[laz.type], 0.9 + 0.2 * sin(4.0f * M_PI * getEngine()->getAllTime() + laz.timeshift)));
    spr_laz->setPosition(laz.pos1);
    spr_laz->setRotation(dir.angle());
    spr_laz->setScale({ dir.length() / 128.0f,1.0f });
    rendertarget.draw(*spr_laz);

    anim_lasers[laz.type]->setColor(color_lasers[laz.type]);
    anim_lasers[laz.type]->setPosition(laz.pos2);
    rendertarget.draw(*anim_lasers[laz.type]);
}

void SceneGame::fixCameraPosition() {
    if (view.getCenter().x < view.getSize().x / 2.0f) view.setCenter({ view.getSize().x / 2.0f, view.getCenter().y });
    if (view.getCenter().x > game.getWidth() * BLOCKW - view.getSize().x / 2.0f) view.setCenter({ game.getWidth() * BLOCKW - view.getSize().x / 2.0f, view.getCenter().y });
    if (view.getCenter().y < view.getSize().y / 2.0f) view.setCenter({ view.getCenter().x, view.getSize().y / 2.0f });
    if (view.getCenter().y > game.getHeight() * BLOCKH - view.getSize().y / 2.0f) view.setCenter({ view.getCenter().x, game.getHeight() * BLOCKH - view.getSize().y / 2.0f });
}

void SceneGame::updateScale() {
    view.setSize({ VIEW_SIZE_X * getScale05per20(), VIEW_SIZE_Y * getScale05per20() });
    minimap.setWindowSize(view.getSize().x, view.getSize().y);
    fixCameraPosition();
}

void SceneGame::drawProgressRectsAt(sf::RenderTarget& rendertarget, float perc, float basew, float x, float y, sf::Color color) {
    rect_pblocks.setFillColor(color);
    int cntrect = ((int)(basew * perc) / (rect_pblocks.getSize().x + 2)) + 1;
    for (int j = 0; j < cntrect; j++) drawShapeAt(rendertarget, rect_pblocks, { x + j * (rect_pblocks.getSize().x + 2), y });
}

void SceneGame::drawProgressRectsAt(sf::RenderTarget& rendertarget, float perc, float basew, sf::Vector2f pos, sf::Color color) {
    drawProgressRectsAt(rendertarget, perc, basew, pos.x, pos.y, color);
}

sf::Vector2f getPosMultiIcon(int i) {
    const int UNIT_IN_ROW = 10;
    const int UNIT_STEP_X = 64;
    const int UNIT_STEP_Y = 84;
    float px = 512 - 240 / 2 - 64 + 12;
    float py = 768 - 182 + 34;
    return { px + (i % UNIT_IN_ROW) * UNIT_STEP_X, py + (i / UNIT_IN_ROW) * UNIT_STEP_Y };
}

// Загрузчик игры из файлов
void SceneGame::loadGame() {
    game.setDifficulty(levelcode.difficulty);
    CompanyInfo comp(levelcode.company, getEngine()->getLanguages().getCurrent());
    bool paramok = game.loadConfigs();
    bool textok = game.loadTexts(comp.getLevelStringsFileName(levelcode.level));
    bool mapok = game.loadMap(comp.getLevelMapFileName(levelcode.level));
    bool scriptok = game.loadScript(comp.getLevelScriptFileName(levelcode.level));
    if (game.getErrMsg()) {
        getEngine()->SwitchToScene(std::make_shared<SceneMsgBox>(*game.getErrMsg(), [this]() { getEngine()->doClose(); }));
        getEngine()->getLogger()->WriteLog(*game.getErrMsg());
    }

    game.update(0.0); // Первичная инициализация для тумана войны
    if (game.getErrMsg()) {
        getEngine()->SwitchToScene(std::make_shared<SceneMsgBox>(*game.getErrMsg(), [this]() { getEngine()->doClose(); }));
        getEngine()->getLogger()->WriteLog(*game.getErrMsg());
    }

    tekscale = DEFAULT_SCALE;
    updateScale();

    // Обновление камеры здесь дублировано для того, чтобы установить её до вызова основного цикла, пока показано задание
    if (auto newvp = game.getOnceNewViewPoint())
        view.setCenter({ (float)(*newvp).x * BLOCKW, (float)(*newvp).y * BLOCKH });

    selector.unSelectAll();

    minimap.prepareMiniMap(8, 768 - 192 + 8, 192 - 16, 192 - 16, game.getWidth(), game.getHeight(),
        VIEW_SIZE_X, VIEW_SIZE_Y, game.getWidth() * BLOCKW, game.getHeight() * BLOCKH);
    updateMiniMap();

    stbuilder.updateByGame(game);
    fogbuilder.initByGame(game);

}

void SceneGame::muteLaserSounds()
{
    for (auto & v: effect_lasers)
        v.second->setVolume(0.0f);
}

void SceneGame::Render(sf::RenderTarget & rendertarget) {
    rendertarget.setView(view);

    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++)
                if (auto treeblock = stbuilder.getTerrainSubType(i, j)) {
                    // Этот трюк нужен, чтобы сначала вывелись территории и фрагменты леса нижние, а потом - верхние, закрывающие пони
                    if (!((*treeblock == TerrainSubType::TreeTop) || (*treeblock == TerrainSubType::TreeTopLeft) || (*treeblock == TerrainSubType::TreeTopRight))) {
                        drawSpriteAt(rendertarget, *spr_trees[*treeblock], i * BLOCKW, j * BLOCKH);
                    }
                    else {
                        if (spr_terrains.count(game.getMap(i, j)) > 0)
                            drawSpriteAt(rendertarget, *spr_terrains[game.getMap(i, j)], i * BLOCKW, j * BLOCKH );
                    }
                }
                else
                    if (spr_terrains.count(game.getMap(i, j)) > 0)
                        drawSpriteAt(rendertarget, *spr_terrains[game.getMap(i, j)], i * BLOCKW, j * BLOCKH );

    // Декорации
    for (int i = 0; i < game.getDecorCount(); i++)
        if (spr_decors.count(game.getDecor(i).code) > 0)
            drawSpriteAt(rendertarget, *spr_decors[game.getDecor(i).code], game.getDecor(i).pos);

    // Вывод грибов
    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++) {
                auto& mset = game.getMushrooms(i, j);
                for (auto& m : mset)
                    drawSpriteAt(rendertarget, *spr_mushrooms[m.spriteid], i * BLOCKW + m.x, j * BLOCKH + m.y );
            }

    // Крестик после территорий, но до юнитов
    if (showcross) {
        spr_cross->setColor({ 255,255,255,(uint8_t)(*showcross).second });
        drawSpriteAt(rendertarget,*spr_cross, (*showcross).first);
    }

    // Вывод юнитов
    for (int i = 0; i < game.getUnitCount(); i++)
            if (spr_units.count(game.getUnit(i).getCode()) > 0) {
                bool movleft = game.getUnit(i).isUnitRotatedLeft();
                std::string sprcode = game.getUnit(i).getCode();
                if (game.getUnit(i).isComponent<ComponentMovable>()) {
                    std::string dirsuff = "";
                    if (game.getUnit(i).getLastMoving() == Moving::Up) dirsuff = "_t";
                    if ((game.getUnit(i).getLastMoving() == Moving::LeftUp) ||
                        (game.getUnit(i).getLastMoving() == Moving::RightUp)) dirsuff = "_rt";
                    if ((game.getUnit(i).getLastMoving() == Moving::LeftDown) ||
                        (game.getUnit(i).getLastMoving() == Moving::RightDown)) dirsuff = "_rb";
                    if (game.getUnit(i).getLastMoving() == Moving::Down) dirsuff = "_b";
                    sprcode = sprcode + dirsuff;
                }
                spr_units[sprcode]->setPosition(game.getUnit(i).getView());
                spr_units[sprcode]->setScale({ movleft ? -1.0f : 1.0f,1 });
                if (game.isUnitUnderAttack(game.getUnit(i).getUID()))
                    rendertarget.draw(*spr_units[sprcode], &shader_attack);
                else
                    rendertarget.draw(*spr_units[sprcode]);

                if (game.getUnit(i).isComponent<ComponentUnicorn>()) {
                    int marker_x = game.getUnit(i).getView().x + (movleft ? 1 : -1) * (game.getUnit(i).getSizeView().x / 2 - 15);
                    int marker_y = game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2 + 5;
                    for (int j = 0; j < game.getUnit(i).getPostfixes().size(); j++) {
                        marker->setPosition(sf::Vector2f(marker_x + (movleft ? 1 : -1) * marker_dx[j], marker_y + marker_dy[j]));
                        marker->setColor(getColors().getColor(game.getUnit(i).getPostfixes()[j]));
                        rendertarget.draw(*marker);
                    }
                }
            }

    // И здесь вывод только верхних фрагментов леса
    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++)
                if (auto treeblock = stbuilder.getTerrainSubType(i, j))
                    if ((*treeblock == TerrainSubType::TreeTop) || (*treeblock == TerrainSubType::TreeTopLeft) || (*treeblock == TerrainSubType::TreeTopRight))
                        drawSpriteAt(rendertarget, *spr_trees[*treeblock], i * BLOCKW, j * BLOCKH);

    // Полоски здоровья, щита и прогресса выводим после юнитов
    for (int i = 0; i < game.getUnitCount(); i++) {
            if (!(game.getUnit(i).isFullHealth() && userprofile->isHideFullHealthBar()))
                drawProgressRectsAt(rendertarget, game.getUnit(i).getHealthPerMax(), game.getUnit(i).getSizeView().x,
                game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - 8,
                getColorByHPNorm(game.getUnit(i).getHealthPerMax()));
            float v = game.getUnit(i).getShieldPerMax();
            int shift = 14;
            if (v > 0) {
                drawProgressRectsAt(rendertarget, v, game.getUnit(i).getSizeView().x,
                    game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                    game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - shift,
                    sf::Color(107, 230, 255));
                shift += 6;
            }
            if (game.getUnit(i).isWorkingTask(&v))
                drawProgressRectsAt(rendertarget, v, game.getUnit(i).getSizeView().x,
                    game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                    game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - shift,
                    sf::Color(217, 138, 255));
        }

    // Лазеры выводим после всего
    for (int i = 0; i < game.getLaserCount(); i++)
        drawLaserFromTo(rendertarget, spr_laz, game.getLaser(i));

    // Фрагменты тумана в конце
    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++)
            if (game.isFog(i, j))
                drawSpriteAt(rendertarget, fogbuilder.getFogBaseSprite(), i* BLOCKW, j* BLOCKH);
            else
                if (auto sprfog = fogbuilder.getFogSprite(i, j))
                    drawSpriteAt(rendertarget, *sprfog, i * BLOCKW, j * BLOCKH);

    for (int uid : selector.getSelectedUnits()) {
        if (game.getUnitByUID(uid).isComponent<ComponentResource>())
            rect_selector.setOutlineColor(sf::Color::Blue);
        else
            if (game.getUnitByUID(uid).isComponent<ComponentEnemy>())
                rect_selector.setOutlineColor(sf::Color::Red);
            else
                rect_selector.setOutlineColor(sf::Color::Green);
        rect_selector.setPosition(game.getUnitByUID(uid).getView());
        rect_selector.setSize(game.getUnitByUID(uid).getSizeView());
        rect_selector.setOrigin(game.getUnitByUID(uid).getSizeView() / 2.0f);
        rendertarget.draw(rect_selector);
    }

    // Эффект телепортации
    if (!current_teleportation_effect.expired()) rendertarget.draw(*current_teleportation_effect.lock());

    rendertarget.setView(rendertarget.getDefaultView());
    // Далее рисуем управлющие элементы панели
    rendertarget.draw(*spr_border);

    minimap.drawTo(&rendertarget);

    textback.setPosition({ 6, 6 });
    textback.setSize({ 130, 50 });
    rendertarget.draw(textback);

    // Информация по ресурсам и танкам
    text_resource->setString(sfge::SfmlTools::utf2text(getTexts().getStr("Text_Energy") + " " + std::to_string(game.getEnergy())));
    text_resource->setPosition({ 10, 10 });
    text_resource->setFillColor(sf::Color{ 162, 231, 255 });
    rendertarget.draw(*text_resource);
    text_resource->setString(sfge::SfmlTools::utf2text(getTexts().getStr("Text_UnicornCount") + " " + game.getUnicornCountInfo()));
    text_resource->setPosition({ 10, 30 });
    text_resource->setFillColor(sf::Color::White);
    rendertarget.draw(*text_resource);

    if (selector.isSelectedOne()) {
        // Информация по юниту
        // Здесь мы привязываем к позиции text_back без его вывода
        textback.setSize({ 240, 192 });
        textback.setPosition({ 512 - textback.getSize().x / 2 - 64, 768 - textback.getSize().y });

        const GameUnit& selunit = game.getUnitByUID(selector.getSelectedUID());
        drawProgressRectsAt(rendertarget, selunit.getHealthPerMax(), 48, textback.getPosition().x + 12, textback.getPosition().y + 64,
            getColorByHPNorm(selunit.getHealthPerMax()));
        if (selunit.getShieldPerMax() > 0.0f)
            drawProgressRectsAt(rendertarget, selunit.getShieldPerMax(), 48, textback.getPosition().x + 12, textback.getPosition().y + 82,
                sf::Color(107, 230, 255));

        if (spr_icons.count(selunit.getCode()) > 0)
            drawSpriteAt(rendertarget, *spr_icons[selunit.getCode()], { textback.getPosition().x + 12 + 48 / 2, textback.getPosition().y + 34 });

        text_caption->setString(getTexts().getSfmlStr("Unit_" + selunit.getCaption()));
        text_caption->setPosition({ textback.getPosition().x + 78, textback.getPosition().y + 18 });
        rendertarget.draw(*text_caption);

        text_hp->setString(selunit.getHealthInfo());
        text_hp->setPosition({ textback.getPosition().x + 78, textback.getPosition().y + 54 });
        text_hp->setFillColor(getColorByHPNorm(selunit.getHealthPerMax()));
        rendertarget.draw(*text_hp);

        text_hp->setString(selunit.getShieldInfo());
        text_hp->setPosition({ textback.getPosition().x + 78, textback.getPosition().y + 72 });
        text_hp->setFillColor(sf::Color(107, 230, 255));
        rendertarget.draw(*text_hp);

        text_info->setString(getTexts().getSfmlStrReplacedConsts(selunit.getComponentsInfo()));
        text_info->setPosition({ textback.getPosition().x + 12, textback.getPosition().y + 92 });
        rendertarget.draw(*text_info);

        // Действия юнита
        // Здесь мы привязываем к позиции text_back без его вывода
        textback.setSize({ 400, 192 });
        textback.setPosition({ 1024 - textback.getSize().x, 768 - textback.getSize().y });

        std::string current_action_code;
        float progress;
        if (selunit.isWorkingTask(&progress, &current_action_code)) {
            rendertarget.draw(rect_progress_border);
            rect_progress.setSize({ (350 - 16) * progress, 32 });
            rendertarget.draw(rect_progress);
            text_progress->setString(std::to_string((int)(100 * progress)) + "%");
            rendertarget.draw(*text_progress);
            drawSpriteAt(rendertarget, *spr_actions[current_action_code], { textback.getPosition().x + 200 - 32, textback.getPosition().y + 8 });
            if (overundo) rendertarget.draw(*undo, &shader_bright); else rendertarget.draw(*undo);
        }
        else {
            auto actions = selunit.getActions();
            for (int i = 0; i < actions.size(); i++) {
                spr_but_action->setPosition(getActionButtonPos(i));
                std::string msgcode = "";
                rendertarget.draw(*spr_but_action);
                if (spr_actions.count(actions[i].code) > 0) {
                    // Вывод действий либо яркими для позиции под курсором, либо серыми для недоступных
                    spr_actions[actions[i].code]->setPosition({ getActionButtonPos(i).x + 4,getActionButtonPos(i).y + 4 });
                    if (!selunit.canSendAction(actions[i], &msgcode))
                        rendertarget.draw(*spr_actions[actions[i].code], &shader_gray);
                    else
                        if ((overactionidx)&&(*overactionidx==i))
                            rendertarget.draw(*spr_actions[actions[i].code], &shader_bright);
                        else
                            rendertarget.draw(*spr_actions[actions[i].code]);
                }
                                
                if ((overactionidx) && (*overactionidx == i) &&(!counter_errmsg.isActive())) {
                    text_action->setString(getTexts().getSfmlStrReplacedConsts("$Action_"+actions[i].caption+"$\n"+actions[i].getReqStr()));
                    text_action->setFillColor(sf::Color::White);
                    rendertarget.draw(*text_action);
                }
            }
        }
    }
    if (selector.isSelectedMulti()) {
        for (int i = 0; i < selector.getSelectedUnits().size(); i++) {
            const GameUnit& selunit = game.getUnitByUID(selector.getSelectedUnits()[i]);
            drawProgressRectsAt(rendertarget, selunit.getHealthPerMax(), 48,
                getPosMultiIcon(i)+ sf::Vector2f{0, 30 }, getColorByHPNorm(selunit.getHealthPerMax()));
            if (selunit.getShieldPerMax() > 0.0f)
                drawProgressRectsAt(rendertarget, selunit.getShieldPerMax(), 48,
                    getPosMultiIcon(i) + sf::Vector2f{0, 36 }, sf::Color(107, 230, 255));
            if (spr_icons.count(selunit.getCode()) > 0)
                drawSpriteAt(rendertarget, *spr_icons[selunit.getCode()], getPosMultiIcon(i) + sf::Vector2f{ 25, 0 });
        }
    }

    // Вывод сообщения об ошибке, если есть
    if (counter_errmsg.isActive()) {
        text_action->setFillColor(sf::Color::Red);
        rendertarget.draw(*text_action);
    }

    // Вывод таймера, если он есть
    if (auto stimer = game.getTimerStr()) {
        textback.setPosition({ 1024 - 144, 4 });
        textback.setSize({ 140, 36 });
        rendertarget.draw(textback);

        text_timer->setString(sfge::SfmlTools::utf2text(getTexts().getStr("Text_Timer") + " " + *stimer));
        text_timer->setPosition({ 1024 - 144 + 10, 8 });
        rendertarget.draw(*text_timer);
    }

    // Вывод сообщения, если оно есть
    float p = 0;
    for (auto & msg: game.getTekMessages()) {
        textback.setPosition({ 1024/2 - 350, 4 + p });
        textback.setSize({ 700, 100 });
        rendertarget.draw(textback);

        drawTextInBlockWidth(rendertarget, *text_msg, msg.text, 1024 / 2 - 350 + 100, 8 + p, 700 - 100, 5);

        if (spr_dialog_icons.count(msg.icon) > 0)
            drawSpriteAt(rendertarget, *spr_dialog_icons[msg.icon], { 1024 / 2 - 350 + 15, 20 + p });
        p += 104;
    }

    if (rect_holded) {
        rect_selector.setOutlineColor(sf::Color::Green);
        rect_selector.setPosition(sf::Vector2f((*rect_holded).position.x, (*rect_holded).position.y));
        rect_selector.setSize(sf::Vector2f((*rect_holded).size.x, (*rect_holded).size.y));
        rect_selector.setOrigin(sf::Vector2f(0, 0));
        rendertarget.draw(rect_selector);
    }

    if (showfps) {
        text_fps->setString("FPS: " + std::to_string(getEngine()->getFactFPS()));
        rendertarget.draw(*text_fps);
    }
}

void SceneGame::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    // Определение наведения на юнита
    std::optional<int> overunituid = std::nullopt;
    if (mousePos.y < VIEW_SIZE_Y) {
        sf::Vector2f worldpos = getEngine()->getWorldPosByView(view, mousePos);        
        if (auto uid = game.findUnitAt(worldpos.x, worldpos.y))
            if (!game.isFog(game.getUnitByUID(*uid).getXY().x, game.getUnitByUID(*uid).getXY().y))
                overunituid = uid;
    }

    // Проверка положений курсора над кнопками
    overundo = false;
    overactionidx = std::nullopt;
    if (selector.isSelectedOne()) {
        // Разрешаем действия только если юнит не работает над действием в данный момент
        if (!game.getUnitByUID(selector.getSelectedUID()).isWorkingTask()) {
            auto actions = game.getUnitByUID(selector.getSelectedUID()).getActions();
            for (int i = 0; i < actions.size(); i++) {
                spr_but_action->setPosition(getActionButtonPos(i));
                if (spr_but_action->getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) overactionidx = i;
            }
        }
        else
            // Действие отмены
            overundo = undo->getGlobalBounds().contains({ (float)mousePos.x,(float)mousePos.y });
    }
    // Установка курсора, если нужно
    if ((overundo)||(overactionidx)||(overunituid)) getEngine()->setCursor(1);
    
    // Расчеты громкости лазеров
    VolumeCalculator volcalc(getEngine()->getWorldPosByView(view, { 0, 0 }),
        getEngine()->getWorldPosByView(view, { (int)(VIEW_SIZE_X), (int)VIEW_SIZE_Y }), 20.0f);
    for (auto& v : effect_lasers) {
        float laservol = 0.0f;
        for (int i = 0; i < game.getLaserCount(); i++)
            if (game.getLaser(i).type == v.first)
                laservol = std::max(laservol, volcalc.getVolume(game.getLaser(i).pos2));
        v.second->setVolume(laservol);
    }

    for (auto & event : events) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if ((keyPressed->scancode == sf::Keyboard::Scancode::Escape) ||
                (keyPressed->scancode == sf::Keyboard::Scancode::F10)) {
                muteLaserSounds();
                getEngine()->AddOverScene(std::make_shared<SceneGameMenu>(&game, levelcode));
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::F5) {
                muteLaserSounds();
                getEngine()->AddOverScene(std::make_shared<SceneTask>(game));
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::F8) {
                muteLaserSounds();
                getEngine()->AddOverScene(std::make_shared<SceneJournal>(game));
            }

            if (keyPressed->scancode == sf::Keyboard::Scancode::F) showfps = !showfps;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Tab) showterrain = !showterrain;
            if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
                game.skipAllMessages();
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                game.skipTekMessage();
                if (game.getLastEventPos()) {
                    view.setCenter(*game.getLastEventPos());
                    fixCameraPosition();
                }
            }
            if ((keyPressed->scancode == sf::Keyboard::Scancode::NumpadMinus) || (keyPressed->scancode == sf::Keyboard::Scancode::Hyphen))
                doScalePlus();
            if ((keyPressed->scancode == sf::Keyboard::Scancode::NumpadPlus) || (keyPressed->scancode == sf::Keyboard::Scancode::Equal))
                doScaleMinus();
        };
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            // Зона игры
            if (mousePressed->position.y < VIEW_SIZE_Y) {
                sf::Vector2f worldpos = getEngine()->getWorldPosByView(view, mousePressed->position);

                // Выделение юнита
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
                        rect_holded = sf::IntRect(mousePressed->position, sf::Vector2i(1, 1));

                    if (overunituid) {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                                if (selector.isNoSelected())
                                    selector.selectOneUnit(*overunituid);
                                else {
                                    if (game.getUnitByUID(*overunituid).isComponent<ComponentMultiselect>()) {
                                        bool allmulti = true;
                                        for (int selid : selector.getSelectedUnits())
                                            allmulti = allmulti && game.getUnitByUID(selector.getSelectedUID()).isComponent<ComponentMultiselect>();
                                        if (allmulti) selector.invertUnit(*overunituid);
                                    }
                                }
                            }
                            else {
                                selector.selectOneUnit(*overunituid);
                                if (game.getUnitByUID(selector.getSelectedUID()).isComponent<ComponentUnicorn>())
                                    if (userprofile->isVoiceOn())
                                        snd_unicorn_clicks[clickcounter.getNextSoundIdx(selector.getSelectedUID())]->play();
                            }
                    }
                }

                // Команда движения юнита
                if (mousePressed->button == sf::Mouse::Button::Right)
                {
                    // Движение всех юнитов, которыми может управлять игрок
                    for (int uid : selector.getSelectedUnits())
                        if (game.getUnitByUID(uid).isComponent<ComponentMovable>()) {
                            game.setTargetToUnit(uid, worldpos.x / BLOCKW, worldpos.y / BLOCKH);
                            showcross = std::pair(worldpos, 255);
                            if (((started_galop_uid != uid) || (effect_start->getStatus() != sf::SoundSource::Status::Playing))) {
                                effect_start->play();
                                started_galop_uid = uid;
                            }
                        }
                }
            }
            else
                // Зона карты
                if (minimap.isXYonMap(mousePressed->position)) {
                    view.setCenter(minimap.getWorldPosByMapPos(mousePressed->position.x, mousePressed->position.y));
                    fixCameraPosition();
                    mouseholdedonmap = true;
                }
            // Зона действий
                else {
                        std::string msgcode = "";
                        if (overactionidx) {
                            auto actions = game.getUnitByUID(selector.getSelectedUID()).getActions();
                            if (!game.getUnitByUID(selector.getSelectedUID()).canSendAction(actions[*overactionidx], &msgcode)) {
                                text_action->setString(getTexts().getSfmlStr(msgcode));
                                counter_errmsg.upset(1.0f);
                            }
                            else
                                game.sendUnitAction(selector.getSelectedUID(), actions[*overactionidx]);
                        }
                        if (overundo) game.cancelUnitWorkingAction(selector.getSelectedUID());

                        if (selector.isSelectedMulti())
                            for (int i = 0; i < selector.getSelectedUnits().size(); i++)
                                if ((getPosMultiIcon(i).x + 25 - 24 < mousePos.x) &&
                                    (getPosMultiIcon(i).x + 25 + 24 > mousePos.x) &&
                                    (getPosMultiIcon(i).y - 24 < mousePos.y) &&
                                    (getPosMultiIcon(i).y + 24 > mousePos.y))
                                    selector.selectOneUnit(selector.getSelectedUnits()[i]);
                }
        }
        // Применение рамки только по отжатию
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (rect_holded) {
                sf::Vector2f worldpos_1 = getEngine()->getWorldPosByView(view, (*rect_holded).position);
                sf::Vector2f worldpos_2 = getEngine()->getWorldPosByView(view, (*rect_holded).position + (*rect_holded).size);
                
                auto uids = game.findVisibleUnitsInRect(std::min(worldpos_1.x, worldpos_2.x), std::min(worldpos_1.y, worldpos_2.y),
                    std::max(worldpos_1.x, worldpos_2.x), std::max(worldpos_1.y, worldpos_2.y));
                if (uids.size() > 0) {
                    // Выделяем всех единорогов в списке
                    selector.unSelectAll();
                    for (int uid : uids)
                        if (game.getUnitByUID(uid).isComponent<ComponentMultiselect>())
                            selector.invertUnit(uid);
                    // Если никто не выделился, то пробуем тогда выделить одного не-единорога
                    if (selector.isNoSelected())
                        for (int uid : uids)
                            if (!game.getUnitByUID(uid).isComponent<ComponentMultiselect>()) {
                                selector.invertUnit(uid);
                                break;
                            }
                }
                rect_holded = std::nullopt;
            }
        }
        if (const auto* mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (mouseWheelScrolled->delta < 0) doScalePlus();
            if (mouseWheelScrolled->delta > 0) doScaleMinus();
        }
    } // Конец проверки событий

    // Прокрутка карты
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
        view.move({ -SCROLLSPEED * BLOCKW * getScale05per20() * dt, 0 });
        fixCameraPosition();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
        view.move({ +SCROLLSPEED * BLOCKW * getScale05per20() * dt, 0 });
        fixCameraPosition();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
        view.move({ 0, -SCROLLSPEED * BLOCKH * getScale05per20() * dt });
        fixCameraPosition();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
        view.move({ 0, SCROLLSPEED * BLOCKH * getScale05per20() * dt });
        fixCameraPosition();
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseholdedonmap) {
        view.setCenter(minimap.getWorldPosByMapPos(mousePos.x, mousePos.y));
        fixCameraPosition();
    }
    else
        mouseholdedonmap = false;

    laser_apply->update(dt);
    aura->update(dt);

    game.update(dt);
    if (game.getErrMsg()) {
        getEngine()->SwitchToScene(std::make_shared<SceneMsgBox>(*game.getErrMsg(), [this]() { getEngine()->doClose(); }));
        getEngine()->getLogger()->WriteLog(*game.getErrMsg());
    }

    for (int uid : selector.getSelectedUnits())
        if (!game.isUnitExist(uid)) selector.unSelectUnit(uid);

    updateMiniMap();

    if (rect_holded) {
        (*rect_holded).size.x = mousePos.x - (*rect_holded).position.x;
        (*rect_holded).size.y = mousePos.y - (*rect_holded).position.y;
    }

    counter_errmsg.update(dt);
    counter_endgame.update(dt);

    if (effect_start->getStatus() == sf::SoundSource::Status::Playing)
        if (game.isUnitExist(started_galop_uid))
            if (!game.getUnitByUID(started_galop_uid).isTargeted()) effect_start->stop();

    // Добавление эффекта телепортации
    if (auto new_effect = game.getOnceTeleportationEffect()) {
        current_teleportation_effect = teleportation;
        current_teleportation_effect.lock()->setPosition(*new_effect);
        current_teleportation_effect.lock()->playOneTime();
    }

    // Обработка эффекта телепортации
    if (!current_teleportation_effect.expired()) {
        current_teleportation_effect.lock()->update(dt);
        if (!current_teleportation_effect.lock()->isPlayed()) current_teleportation_effect.reset();
    }

    // Обработка разных эффектов
    for (auto effect : game.getOnceAudioEffects()) {
        snd_audioeffects.push_back(std::make_unique<sf::Sound>(*snd_audioeffects_buffer[effect.effect]));
        if (effects_posed.contains(effect.effect)) snd_audioeffects.back()->setVolume(volcalc.getVolume(effect.pos));
        snd_audioeffects.back()->play();
    }

    // Здесь нужен эффект прозрачности по времени
    if (showcross) {
        (*showcross).second -= 255 * dt;
        if ((*showcross).second <= 0) showcross = std::nullopt;
    }

    if (counter_endgame.onceReachNol()) {
        muteLaserSounds();
        getEngine()->AddOverScene(std::make_shared<SceneEndGame>(game, levelcode));
    }

    // Обновление камеры если нужно
    if (auto newvp = game.getOnceNewViewPoint())
        view.setCenter({ (float)(*newvp).x * BLOCKW, (float)(*newvp).y * BLOCKH });

    fogbuilder.updateByGame(game);

    shader_attack.setUniform("stage", (float)(0.25f + 0.25f * sin(4.0f * M_PI * getEngine()->getAllTime())));

    if (game.isGameOver()) {
        if (!counter_endgame.isActive()) {
            if (game.isWin()) {
                userprofile->setLevelCompleted(levelcode.company, levelcode.level);
                userprofile->saveProfile();
            }
            counter_endgame.upset(2.0f);
        }
    }

}

void SceneGame::Init() {
    userprofile = std::static_pointer_cast<UserProfile>(getProfile());

    spr_border = loadSprite("images/border.png");
    spr_border->setPosition({ 0, 768 - 192 });

    spr_cross = loadSprite("images/cross.png");
    spr_cross->setOrigin({ 16,16 });

    spr_but_action = loadSprite("images/button.png") ;
    undo = loadSprite("images/undo.png");
    undo->setPosition({ 1024 - 36 - 10, VIEW_SIZE_Y + 130 });
    marker = loadSprite("images/marker.png");
    marker->setOrigin({ 5, 5 });

    // Используется загрузка каталога в целом, можно вынести как процедуру
    std::string pathload = "images/units/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        auto str = filename.path().string();
        replaceFirstString(str, pathload, "");
        replaceFirstString(str, ".png", "");
        
        spr_units[str] = loadSprite(filename.path().string());
        spr_units[str]->setOrigin({ spr_units[str]->getTexture().getSize().x / 2.0f,
            spr_units[str]->getTexture().getSize().y / 2.0f });

        spr_icons[str] = loadSprite(filename.path().string());
        spr_icons[str]->setOrigin({ spr_units[str]->getTexture().getSize().x / 2.0f,spr_units[str]->getTexture().getSize().y / 2.0f });
        spr_icons[str]->setScale({ 48.0f / spr_units[str]->getTexture().getSize().x, 48.0f / spr_units[str]->getTexture().getSize().y });
    }

    // Догрузка единорогов по направлениям (вынести в процедуру)
    sf::Image source;
    source.loadFromFile("images/unicorn.png");

    const int unitsize = 64;
    spr_units["unicorn"] = loadSprite(source, 0, 0, unitsize, unitsize);
    spr_units["unicorn"]->setOrigin({ unitsize / 2 , unitsize / 2 });

    spr_icons["unicorn"] = loadSprite(source, 0, 0, unitsize, unitsize);
    spr_icons["unicorn"]->setOrigin({ unitsize / 2 , unitsize / 2 });
    spr_icons["unicorn"]->setScale({ 64.0f / unitsize, 64.0f / unitsize });

    spr_units["unicorn_b"] = loadSprite(source, 6 * unitsize, 0, unitsize, unitsize);
    spr_units["unicorn_b"]->setOrigin({ unitsize / 2 , unitsize / 2 });

    spr_units["unicorn_t"] = loadSprite(source, 2 * unitsize, 0, unitsize, unitsize);
    spr_units["unicorn_t"]->setOrigin({ unitsize / 2 , unitsize / 2 });

    spr_units["unicorn_rb"] = loadSprite(source, 7 * unitsize, 0, unitsize, unitsize);
    spr_units["unicorn_rb"]->setOrigin({ unitsize / 2 , unitsize / 2 });

    spr_units["unicorn_rt"] = loadSprite(source, 1 * unitsize, 0, unitsize, unitsize);
    spr_units["unicorn_rt"]->setOrigin({ unitsize / 2 , unitsize / 2 });

    // Используется загрузка каталога в целом, можно вынести как процедуру
    pathload = "images/actions/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        auto str = filename.path().string();
        replaceFirstString(str, pathload, "");
        replaceFirstString(str, ".png", "");

        spr_actions[str] = loadSprite(filename.path().string());
    }

    // Используется загрузка каталога в целом, можно вынести как процедуру
    pathload = "images/icons/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        auto str = filename.path().string();
        replaceFirstString(str, pathload, "");
        replaceFirstString(str, ".png", "");

        spr_dialog_icons[str] = loadSprite(filename.path().string());
    }

    // Используется загрузка каталога в целом, можно вынести как процедуру
    pathload = "images/mushrooms/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        spr_mushrooms.push_back(loadSprite(filename.path().string()));
        spr_mushrooms.back()->setOrigin({ spr_mushrooms.back()->getTexture().getSize().x / 2.0f, spr_mushrooms.back()->getTexture().getSize().y / 2.0f});
    }

    // Используется загрузка каталога в целом, можно вынести как процедуру
    pathload = "images/decors/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        auto str = filename.path().string();
        replaceFirstString(str, pathload, "");
        replaceFirstString(str, ".png", "");

        spr_decors[str] = loadSprite(filename.path().string());
        spr_decors[str]->setOrigin({ spr_decors[str]->getTexture().getSize().x / 2.0f,
            spr_decors[str]->getTexture().getSize().y / 2.0f });
    }

    view.setSize({ VIEW_SIZE_X, VIEW_SIZE_Y });
    view.setViewport(sf::FloatRect({ 0.0,0.0 }, { 1.0,0.75 }));

    text_caption = loadText(20,sf::Color::White);
    text_action = loadText(16);
    text_action->setPosition({ 1024 - 400 + 8, 712 });
    text_hp = loadText(18);
    text_info = loadText(16,sf::Color::White);

    text_timer = loadText(20,sf::Color::White);
    text_resource = loadText(18);
    text_progress = loadText(24, sf::Color::White);
    text_progress->setPosition({ 1024 - 400 / 2 - 16, VIEW_SIZE_Y + 132 });

    text_fps= loadText(16, sf::Color::Green);
    text_fps->setPosition({ 1024 - 70, 768 - 20 });

    text_msg = loadText(18, sf::Color::White);
    
    // Прямоугольники интерфейса
    rect_selector.setOutlineThickness(2);
    rect_selector.setFillColor(sf::Color::Transparent);

    rect_progress_border.setOutlineThickness(2);
    rect_progress_border.setOutlineColor(sf::Color::White);
    rect_progress_border.setPosition({ 1024 - 400 + 8, VIEW_SIZE_Y + 132 });
    rect_progress_border.setSize({ 350 - 16, 32 });
    rect_progress_border.setFillColor(sf::Color::Transparent);

    rect_progress.setOutlineThickness(0);
    rect_progress.setPosition(rect_progress_border.getPosition());
    rect_progress.setFillColor(sf::Color(217, 138, 255));

    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });

    rect_pblocks.setSize({ 4, 4 });
    rect_pblocks.setOutlineThickness(1);
    rect_pblocks.setOutlineColor(sf::Color{ 64,64,64 });

    // Звуки лазера и старта
    effect_lasers[LaserType::Attack] = loadSound("sounds/laser.ogg");
    effect_lasers[LaserType::Harvest] = loadSound("sounds/laser_harvest.ogg");
    effect_lasers[LaserType::Heal] = loadSound("sounds/laser_heal.ogg");
    effect_lasers[LaserType::Detox] = loadSound("sounds/laser.ogg");

    for (auto& v : effect_lasers) {
        v.second->setVolume(0.0f);
        v.second->setLooping(true);
        v.second->play();
    }

    effect_start = loadSound("sounds/start.ogg");
    for (int i = 0; i <= 2; i++)
        snd_unicorn_clicks.push_back(loadSound(std::format("sounds/unicorn_click_{}.ogg",i)));
    snd_unicorn_clicks.push_back(loadSound("sounds/secret.ogg"));

    clickcounter.setCount(snd_unicorn_clicks.size());

    rect_holded = std::nullopt;

    snd_audioeffects_buffer[AudioEffect::Teleport]=loadSoundBuffer("sounds/teleport.ogg");
    // В файле finish_teleport добавлена пауза в начале, чтобы можно было использовать совместно с эффектом телепортации
    snd_audioeffects_buffer[AudioEffect::FinishTeleport] = loadSoundBuffer("sounds/finish_teleport.ogg");
    snd_audioeffects_buffer[AudioEffect::FinishResearch]= loadSoundBuffer("sounds/finish_research.ogg");
    snd_audioeffects_buffer[AudioEffect::FinishUpgrade]= loadSoundBuffer("sounds/finish_upgrade.ogg");
    snd_audioeffects_buffer[AudioEffect::FinishBuilding] = loadSoundBuffer("sounds/finish_building.ogg");
    snd_audioeffects_buffer[AudioEffect::FinishConstruct] = loadSoundBuffer("sounds/finish_construct.ogg");
    snd_audioeffects_buffer[AudioEffect::UnderAttack]= loadSoundBuffer("sounds/under_attack.ogg");
    snd_audioeffects_buffer[AudioEffect::MonsterKilled] = loadSoundBuffer("sounds/monster_killed.ogg");
    snd_audioeffects_buffer[AudioEffect::TowerKilled] = loadSoundBuffer("sounds/tower_killed.ogg");
    snd_audioeffects_buffer[AudioEffect::CrystallKilled] = loadSoundBuffer("sounds/crystal_killed.ogg");

    spr_terrains[Terrain::Ground]=loadSprite("images/terrains/ground.png");
    spr_terrains[Terrain::Water]=loadSprite("images/terrains/water.png");
    spr_terrains[Terrain::Forest]=loadSprite("images/terrains/forest.png");
    spr_terrains[Terrain::Road]=loadSprite("images/terrains/road.png");

    spr_trees[TerrainSubType::TreeBottom] = loadSprite("images/subterrains/tree_bottom.png");
    spr_trees[TerrainSubType::TreeBottomLeft] = loadSprite("images/subterrains/tree_bottom_left.png");
    spr_trees[TerrainSubType::TreeBottomRight] = loadSprite("images/subterrains/tree_bottom_right.png");
    spr_trees[TerrainSubType::TreeTop] = loadSprite("images/subterrains/tree_top.png");
    spr_trees[TerrainSubType::TreeTopLeft] = loadSprite("images/subterrains/tree_top_left.png");
    spr_trees[TerrainSubType::TreeTopRight] = loadSprite("images/subterrains/tree_top_right.png");
    spr_trees[TerrainSubType::TreeLeft] = loadSprite("images/subterrains/tree_left.png");
    spr_trees[TerrainSubType::TreeRight] = loadSprite("images/subterrains/tree_right.png");
    spr_trees[TerrainSubType::WaterTopLeft] = loadSprite("images/subterrains/water_top_left.png");
    spr_trees[TerrainSubType::WaterTop] = loadSprite("images/subterrains/water_top.png");
    spr_trees[TerrainSubType::WaterTopRight] = loadSprite("images/subterrains/water_top_right.png");
    spr_trees[TerrainSubType::WaterLeft] = loadSprite("images/subterrains/water_left.png");
    spr_trees[TerrainSubType::WaterRight] = loadSprite("images/subterrains/water_right.png");
    spr_trees[TerrainSubType::WaterBottomLeft] = loadSprite("images/subterrains/water_bottom_left.png");
    spr_trees[TerrainSubType::WaterBottom] = loadSprite("images/subterrains/water_bottom.png");
    spr_trees[TerrainSubType::WaterBottomRight] = loadSprite("images/subterrains/water_bottom_right.png");
    spr_trees[TerrainSubType::RoadHorz] = loadSprite("images/subterrains/road_horz.png");
    spr_trees[TerrainSubType::RoadVert] = loadSprite("images/subterrains/road_vert.png");
    spr_trees[TerrainSubType::RoadTopLeft] = loadSprite("images/subterrains/road_top_left.png");
    spr_trees[TerrainSubType::RoadTopRight] = loadSprite("images/subterrains/road_top_right.png");
    spr_trees[TerrainSubType::RoadBottomLeft] = loadSprite("images/subterrains/road_bottom_left.png");
    spr_trees[TerrainSubType::RoadBottomRight] = loadSprite("images/subterrains/road_bottom_right.png");

    // Цвета территорий и лазеров
    color_terrains[Terrain::Ground] = sf::Color(105, 149, 19);
    color_terrains[Terrain::Water] = sf::Color(0, 206, 228);
    color_terrains[Terrain::Forest] = sf::Color(25, 103, 39);
    color_terrains[Terrain::Road] = sf::Color(228, 218, 171);

    color_lasers[LaserType::Harvest] = sf::Color(0, 255, 255);
    color_lasers[LaserType::Attack] = sf::Color(255, 0, 0);
    color_lasers[LaserType::Heal] = sf::Color(240, 255, 0);
    color_lasers[LaserType::Detox] = sf::Color(160, 56, 255);

    laser_apply = std::make_shared<sfge::Animation>("images/laser_apply.png", 30, 34, 12, 12);
    laser_apply->setOrigin({ 15,17 });
    laser_apply->play();

    aura = std::make_shared<sfge::Animation>("images/aura_default.png", 86, 80, 12, 12);
    aura->setOrigin({ 43, 40 });
    aura->play();

    anim_lasers[LaserType::Harvest] = aura;
    anim_lasers[LaserType::Attack] = laser_apply;
    anim_lasers[LaserType::Heal] = aura;
    anim_lasers[LaserType::Detox] = laser_apply;

    teleportation = std::make_unique<sfge::Animation>("images/teleportation.png", 96, 96, 9, 9);
    teleportation->setOrigin({ 48, 48 });

    spr_laz = loadSprite("images/laser.png");
    spr_laz->setOrigin({ 0,3 });

    shader_gray.loadFromFile("shaders/gray.frag", sf::Shader::Type::Fragment);
    shader_bright.loadFromFile("shaders/bright.frag", sf::Shader::Type::Fragment);
    shader_attack.loadFromFile("shaders/attack.frag", sf::Shader::Type::Fragment);

    shader_gray.setUniform("texture", sf::Shader::CurrentTexture);
    shader_bright.setUniform("texture", sf::Shader::CurrentTexture);
    shader_attack.setUniform("texture", sf::Shader::CurrentTexture);

    effects_posed = { AudioEffect::Teleport, AudioEffect::MonsterKilled, AudioEffect::TowerKilled };

    if (std::filesystem::exists(getEngine()->getExeDir() + "/developer.json"))
        game.loadDeveloperConfig(getEngine()->getExeDir() + "/developer.json");

    loadGame();
}
