#pragma warning (disable: 4275)

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneCloseHandler.h"
#include "HelperCppClasses/StringTools.h"
#include "UserProfile.h"
#include "SfmlGameEngine/Logger.h"
#include "SceneGame.h"

#pragma comment (lib, "sfml-graphics.lib")
#pragma comment (lib, "sfml-system.lib")
#pragma comment (lib, "sfml-window.lib")
#pragma comment (lib, "sfml-audio.lib")
#pragma comment (lib, "sfml-main-s.lib")
#pragma comment (lib, "zetscript-2-1-0-static.lib")
#pragma comment (lib, "jsoncpp_static.lib")

int main(int argc, char* argv[])
{
    srand(time(NULL));

    std::filesystem::path exepath(argv[0]);
    std::string exedir = exepath.parent_path().string();

    // Установка текущего каталога
    // При ручном указании каталога
    if (argc > 1)
        std::filesystem::current_path(std::string(argv[1]));
    else
        // Для корректной работы внутри AppImage
        if (hasEnding(std::filesystem::current_path().string(), "/usr"))
            std::filesystem::current_path("data");
    // Вариант по умолчанию - data в каталоге исполняемого файла
        else
            std::filesystem::current_path(exedir + "/data");

    // Обработка домашнего каталога для платформ
#ifdef __GNUC__
#ifdef __MINGW32__
    char buf[_MAX_PATH];
    size_t bufcount;
    getenv_s(&bufcount, buf, _MAX_PATH, "LOCALAPPDATA");
    auto profiledir = std::string(buf) + "\\UnicornExpansion\\";
#else
    const char * buf = getenv("HOME");
    auto profiledir = std::string(buf) + "/.local/share/UnicornExpansion/";
#endif
#else
    char buf[_MAX_PATH];
    size_t bufcount;
    getenv_s(&bufcount, buf, _MAX_PATH, "LOCALAPPDATA");
    auto profiledir = std::string(buf) + "\\UnicornExpansion\\" ;
#endif

    // Создаем домашний каталог
    std::filesystem::create_directories(profiledir);

    // Грузим профиль игры
    auto profile = std::make_shared<UserProfile>();
    profile->loadProfile(profiledir+"profile.json");

    sfge::Engine engine(1024, 768);
    engine.setExeDir(exedir);
    engine.loadTexts("strings.txt");
    engine.loadColors("colors.dat");
    engine.setCaption(engine.getTexts().getStr("Text_GameCaption"));
    engine.setIcon("images/icon.png");
    engine.loadDefaultFont("arial.ttf");
    engine.loadDefaultCursor("images/cursor_def.png");
    engine.addCursor(1, "images/cursor_my.png");
    engine.setStopUpdatingForLostFocus(true);
    engine.setUserProfile(profile);
    engine.setCloseHandlerScene(std::make_shared<SceneCloseHandler>());
    engine.setUserLogger(std::make_shared<sfge::LoggerFile>(profiledir + "game.log"));

    // Специальный отладочный код, при передаче второго аргумента - вызываем system-карту
    // Уйдет после реализации консоли управления
    if (argc > 2)
        engine.Run(std::make_shared<SceneGame>("system", std::stoi(std::string(argv[2])), Difficulty::Norm));
    else
        engine.Run(std::make_shared<SceneMainMenu>());
        
    return 0;
}
