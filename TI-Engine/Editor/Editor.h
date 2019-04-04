#include <TI-EngineCore/GUI/Grid.h>

#pragma once

namespace edr
{
    class Editor
    {
        public:

        Editor(); // Init Engine(Editor): reading settings, etc.

        void Start(); // Starts the Engine(Editor) and enter to engine cycle.

        private:

        void LoadEditorLib(); // Using by Editor lib to reload;
        void AddGrid(shared_ptr<eng::Grid> GridToAdd); // Using by Editor.lib to register grid in EngineGUI scope.
        void RemoveGrid(shared_ptr<eng::Grid> GridToRemove); // Reverse of AddGrid ofc.

        shared_ptr<void*> editorLib;
        list<shared_ptr<eng::Grid>> editorGuiStorage;
        bool draw = false;

    };
}