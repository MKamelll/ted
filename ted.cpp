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

    Editor *insert(char c);
    Editor *remove();
    Editor *push();
    Editor *pop();
    size_t len();
    vector<string> &lines();
    int x();
    int y();

    Editor *handle_backspace();
    Editor *handle_enter();

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

size_t Editor::len() { return buffer.size(); }

int Editor::x() { return cursor_x; }
int Editor::y() { return cursor_y; }
vector<string> &Editor::lines() { return buffer; }

Editor *Editor::push() {
    buffer.push_back("");
    cursor_y++;
    return this;
}

Editor *Editor::pop() {
    buffer.pop_back();
    cursor_y = max(0, cursor_y - 1);
    return this;
}

Editor *Editor::insert(char c) {
    buffer[cursor_y].insert(cursor_x, 1, c);
    cursor_x++;
    return this;
}

Editor *Editor::remove() {
    cursor_x = max(0, cursor_x - 1);
    buffer[cursor_y].erase(cursor_x);
    return this;
}

Editor *Editor::handle_backspace() {
    if (cursor_y == 0 && cursor_x == 0) {
        return this;
    }

    if (cursor_y > 0 && cursor_x == 0) {
        cursor_x = buffer[cursor_y - 1].size();
        buffer[cursor_y - 1].insert(cursor_x, buffer[cursor_y]);
        pop();
    } else {
        remove();
    }

    return this;
}

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
                editor->insert(static_cast<char>(ch));
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
            editor->handle_backspace();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
