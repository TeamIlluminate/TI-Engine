#include "Editor.h"
#include <dlfcn.h>

using namespace edr;

typedef std::function<void(shared_ptr<eng::Grid>)> voidGridDelegate;

Editor::Editor()
{
}

void Editor::Start()
{

}

void Editor::LoadEditorLib()
{
    // Stop GUI drawing, clear GUI and free lib(if it's initialized)
    draw = false;
    editorGuiStorage.clear();
    dlclose(editorLib.get());

    void (*rld)(voidDelegate); // Reload Handler setter structure
    void (*grd) (voidGridDelegate); // Grid argument structure
    void (*init)(); // Init structure

    editorLib = make_shared<void *>(dlopen("bin/libTIEditor.so", RTLD_LAZY));

    if (!editorLib)
    {
        cerr << dlerror();
        return;
    }

    char *error;

    // First of all -> link to init function

    init = (void (*)())dlsym(editorLib.get(), "InitEditorLib"); // Gets address of Init lib function

    if ((error = dlerror()) != NULL)
    {
        cerr << error;
        return;
    }

    // Link & set reload handler

    rld = (void (*)(voidDelegate))dlsym(editorLib.get(), "SetReloadHandler");

    if ((error = dlerror()) != NULL)
    {
        cerr << error;
        return;
    }

    rld(bind(&Editor::LoadEditorLib, this));

    // Set GUI Grids handlers

    grd = (void (*)(voidGridDelegate))dlsym(editorLib.get(), "SetAddGridHandler");

    if ((error = dlerror()) != NULL)
    {
        cerr << error;
        return;
    }

    grd(bind(&Editor::AddGrid, this, placeholders::_1));


    grd = (void (*)(voidGridDelegate))dlsym(editorLib.get(), "SetRemoveGridHandler");

    if ((error = dlerror()) != NULL)
    {
        cerr << error;
        return;
    }

    grd(bind(&Editor::RemoveGrid, this, placeholders::_1));
}

void Editor::AddGrid(shared_ptr<eng::Grid> grid)
{
    editorGuiStorage.push_back(grid);
}

void Editor::RemoveGrid(shared_ptr<eng::Grid> grid)
{
    editorGuiStorage.remove(grid);
}