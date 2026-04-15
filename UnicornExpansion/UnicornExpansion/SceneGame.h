#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SfmlGameEngine/Scene.h"
#include <filesystem>
#include <string>
#include <map>
#include "SfmlGameEngine/Animation.h"

#include "Game.h"
#include "GameUnit.h"
#include "HelperCppClasses/Countdown.h"
#include "MiniMap.h"
#include "SfmlGameEngine/SfmlTools.h"
#include "UnitFactory.h"
#include "ComponentUnicorn.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentEnemy.h"
#include "ComponentResource.h"
#include "ComponentHarvester.h"
#include "SubTerrainBuilder.h"
#include "FogBuilder.h"
#include "HelperCppClasses/StringTools.h"
#include "ClickerCounter.h"
#include "UnitSelector.h"

const int DEFAULT_SCALE = 2;

class SceneGame: public sfge::Scene
{
private:
    int leveln;
    
    // Все данные игры, спрайты, тексты, массивы территорий и цветов
    std::map<std::string, std::unique_ptr<sf::Sprite>> spr_units;
    std::map<std::string, std::unique_ptr<sf::Sprite>> spr_icons;
    std::map<std::string, std::unique_ptr<sf::Sprite>> spr_actions;
    std::map<std::string, std::unique_ptr<sf::Sprite>> spr_dialog_icons;
    std::map<Terrain, std::unique_ptr<sf::Sprite>> spr_terrains;
    std::vector<std::unique_ptr<sf::Sprite>> spr_mushrooms;
    std::map<Terrain, sf::Color> color_terrains;
    std::map<LaserType, sf::Color> color_lasers;
    std::map<LaserType, std::shared_ptr<sfge::Animation>> anim_lasers;
    std::map<TerrainSubType, std::unique_ptr<sf::Sprite>> spr_trees;
    std::vector<std::unique_ptr<sf::Sound>> snd_unicorn_clicks;
    std::map<AudioEffect, std::unique_ptr<sf::Sound>> snd_audioeffects;
    sf::View view;
    Countdown counter_errmsg;
    MiniMap minimap;
    Game game;
    SubTerrainBuilder stbuilder;
    FogBuilder fogbuilder;
    // Управление выделенными юнитами
    UnitSelector selector;
    int started_galop_uid = -1;
    Countdown counter_endgame;
    int tekscale = DEFAULT_SCALE;
    std::weak_ptr<sfge::Animation> current_teleportation_effect;
    bool mouseholdedonmap = false;
    bool showfps = false;

    bool overundo = false;
    std::optional<int> overactionidx = std::nullopt;

    // Сдвиговые значения для маркеров способностей
    std::array<int, 8> marker_dx = { 11, 0, 11, 0, 11, 0, 11, 0 };
    std::array<int, 8> marker_dy = { 0, 0, 11, 11, 64 - 22, 64 - 22, 64 - 11, 64 - 11 };

    // Перенесено из main
    std::unique_ptr<sf::Sprite> spr_border;
    std::unique_ptr<sf::Sprite> spr_but_action;
    std::unique_ptr<sf::Sprite> undo;
    std::unique_ptr<sf::Sprite> marker;
    std::unique_ptr<sf::Text> text_caption;
    std::unique_ptr<sf::Text> text_action;
    std::unique_ptr<sf::Text> text_hp;
    std::unique_ptr<sf::Text> text_info;
    std::unique_ptr<sf::Text> text_timer;
    std::unique_ptr<sf::Text> text_resource;
    std::unique_ptr<sf::Text> text_progress;
    std::unique_ptr<sf::Text> text_fps;
    std::unique_ptr<sf::Text> text_msg;

    sf::RectangleShape rect_selector;
    sf::RectangleShape rect_progress_border;
    sf::RectangleShape rect_progress;
    sf::RectangleShape textback;

    std::unique_ptr<sf::Sound> effect_fire;
    std::unique_ptr<sf::Sound> effect_start;

    ClickerCounter clickcounter;

    std::optional<sf::Rect<int>> rect_holded;

    std::shared_ptr<sfge::Animation> laser_apply;
    std::shared_ptr<sfge::Animation> aura;
    std::shared_ptr<sfge::Animation> teleportation;

    std::unique_ptr<sf::Sprite> spr_laz;

    sf::Shader shader_gray;
    sf::Shader shader_bright;
    sf::Shader shader_attack;

    sf::RectangleShape rect_pblocks;

    void updateMiniMap();
    float getScale05per20();
    void drawLaserFromTo(sf::RenderTarget& rendertarget, std::unique_ptr<sf::Sprite>& spr_laz, const Laser& laz);
    void fixCameraPosition();
    void updateScale();
    void drawProgressRectsAt(sf::RenderTarget& rendertarget, float perc, float basew, float x, float y, sf::Color color);
    void loadGame(int leveln);

public:
    SceneGame(int leven);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
    virtual void UnInit() override;
};
