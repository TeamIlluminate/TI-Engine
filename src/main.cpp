#include "GameMaster.h"
#include "Render.h"

int main() {

    eng::Render* render = new eng::Render(sf::VideoMode::getFullscreenModes()[0]);
    eng::GameMaster::Get().GameStarted(true);

    //Циклическая заглушка, дабы дать рендеру работать. Пока не знаю, что можно имплементировать здесь,
    //потому что по-хорошему тут должен быть цикл редактора\менеджера компонентов какого-то
    while(eng::GameMaster::Get().IsGameStarted()){}

    return 0;
}