#include <TI-EngineCore/GUI/Grid.h>
#include <TI-EngineCore/GUI/Button.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef std::function<void(shared_ptr<eng::Grid>)> voidGridDelegate;

    voidDelegate reloadHandler;
    voidGridDelegate addGridHandler;
    voidGridDelegate removeGridHandler;

    list<string> someList;

    void InitEditorLib()
    {
        auto testGrid = make_shared<eng::Grid>(sf::Vector2f(100, 200), "sqwer", sf::Vector2f(500, 600));
        testGrid->SetID("0");

        auto testButton = make_shared<eng::Button>("TestButton", sf::Vector2f(100, 50));
        testGrid->AddControl(testButton);
        addGridHandler(testGrid);

        testButton->SetClickHandler(reloadHandler);

        auto testGrid1 = make_shared<eng::Grid>(sf::Vector2f(100, 200), "sqwert", sf::Vector2f(500, 600));
        testGrid->SetID("1");
        addGridHandler(testGrid1);
    }

    void SetReloadHandler(voidDelegate rldf)
    {
        reloadHandler = rldf;
    }

    void SetAddGridHandler(voidGridDelegate grdf)
    {
        addGridHandler = grdf;
    }

    void SetRemoveGridHandler(voidGridDelegate grdf)
    {
        removeGridHandler = grdf;
    }

#ifdef __cplusplus
}
#endif
