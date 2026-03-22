#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include <raylib.h>
#include <cctype>

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

#define PROJECT_NAME "ted"
#define WIDTH 860
#define HEIGHT 640

using namespace std;

class Editor {
public:
    Editor();

    Editor *push(char c);
    Editor *pop();
    size_t len();
    vector<string> &lines();
    int x();
    int y();

private:
    vector<string> buffer;
    int cursor_x;
    int cursor_y;
};

Editor::Editor() {
    buffer.push_back("");
    cursor_x = 0;
    cursor_y = 0;
}

Editor *Editor::push(char c) {
    buffer[cursor_y].push_back(c);
    cursor_x++;
    return this;
}

Editor *Editor::pop() {
    buffer[cursor_y].pop_back();
    cursor_x = max(0, cursor_x - 1);
    return this;
}

size_t Editor::len() { return buffer.size(); }

int Editor::x() { return cursor_x; }
int Editor::y() { return cursor_y; }
vector<string> &Editor::lines() { return buffer; }

int main(int argc, char **argv) {

    InitWindow(WIDTH, HEIGHT, PROJECT_NAME);
    GuiLoadStyle("raygui/styles/dark/style_dark.rgs");

    auto font = LoadFont("HackNerdFontMono-Regular.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    auto editor = make_unique<Editor>();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(BACKGROUND_COLOR));

        for (size_t line = 0; line < editor->len(); line++) {
            DrawTextEx(font, editor->lines()[line].c_str(),
                       (Vector2){.x = 0, .y = static_cast<float>(editor->y())},
                       24, 0, WHITE);
        }

        int ch;
        while ((ch = GetCharPressed()) > 0) {
            if (isascii(ch)) {
                editor->push(static_cast<char>(ch));
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
