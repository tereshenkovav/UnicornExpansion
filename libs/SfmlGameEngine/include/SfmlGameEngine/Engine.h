#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Texts.h"
#include "SfmlGameEngine/Colors.h"
#include <map>

namespace sfge {

class Engine
{
private:
    unsigned int width;
    unsigned int height;
    std::string exedir;
    // Флаги закрытия окна и закрытия текущей (верхней) сцены
    bool signal_closed;
    bool signal_exitscene;
    std::unique_ptr<sf::RenderWindow> window;
    // Ссылки на новые сцены - корневой и верхней следующей
    std::shared_ptr<Scene> nextscene;
    std::shared_ptr<Scene> overscene;
    // Шрифт по умолчанию
    std::shared_ptr<sf::Font> defaultfont;
    // Текстуры для курсоров
    std::vector<std::unique_ptr<sf::Texture>> textures;
    // Курсор по умолчанию
    std::shared_ptr<sf::Sprite> defaultcursor;
    // Текущий курсор, установленный в методе внешнем
    std::weak_ptr<sf::Sprite> currentcursor;
    // Карта курсоров дополнительных
    std::map<int, std::shared_ptr<sf::Sprite>> cursors;
    Texts texts;
    Colors colors;
    bool stopupdatingforlostfocus = false;
public:
    Engine(unsigned int width, unsigned int height) ;
    // Основной метод - запуск движка со сценой
    void Run(std::shared_ptr<Scene> scene);
    // Установка поведения при потере фокуса окна
    void setStopUpdatingForLostFocus(bool value);
    // Работа с установкой exe-каталога для сцен, если нужно чего грузить
    void setExeDir(const std::string& exedir);
    std::string getExeDir() const;
    // Работа с курсором - курсор по умолчанию и добавить допкурсоры с числовым ключом
    void loadDefaultCursor(const std::string& filename);
    void addCursor(int code, const std::string& filename);
    // Сделать активным курсор с номером
    void setCursor(int code);
    // Переключение к новой корневой сцене
    void SwitchToScene(std::shared_ptr<Scene> scene);
    // Добавить новую сцену поверх
    void AddOverScene(std::shared_ptr<Scene> scene);
    // Настройка окна - заголовок и иконка
    void setCaption(const std::string & str);
    void setIcon(const std::string& filename);
    // Общие ресурсы движка для всех сцен - шрифт, строки, цвета
    void loadDefaultFont(const std::string& filename);
    std::shared_ptr<sf::Font> getDefaultFont() const;
    void loadTexts(const std::string& filename);
    const Texts& getTexts() const;
    void loadColors(const std::string& filename);
    const Colors& getColors() const;
    // Преобразование из координат окна в мир по представлению
    sf::Vector2f getWorldPosByView(const sf::View& view, sf::Vector2i pos);
    // Команда на закрытие игры
    void doClose();
    // Команда на выход из текущей сцены (если сцена корневая - то будет выход из игры)
    void doExitScene();
};

};
