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
    Editor(const char *font_name, float size, float char_spacing)
        : font(LoadFont(font_name)), font_height(size), spacing(char_spacing),
          cursor_x(0), cursor_y(0) {

        buffer.push_back("");
        auto w1 = MeasureTextEx(font, "A", font_height, 0).x;
        auto w2 = MeasureTextEx(font, " ", font_height, 0).x;
        char_width = fmaxf(w1, w2);
    }

    vector<string> &lines();
    int x();
    int y();
    Editor *insert(char c);
    Editor *remove();
    Editor *push();
    Editor *pop();
    size_t len();
    void draw();

    Editor *handle_backspace();
    Editor *handle_enter();

private:
    vector<string> buffer;
    Font font;
    float font_height;
    float spacing;
    float char_width;
    int cursor_x;
    int cursor_y;
};

size_t Editor::len() { return buffer.size(); }

int Editor::x() { return cursor_x; }
int Editor::y() { return cursor_y; }
vector<string> &Editor::lines() { return buffer; }

Editor *Editor::push() {
    buffer.insert(buffer.begin() + cursor_y++, "");
    return this;
}

Editor *Editor::pop() {
    cursor_y = max(0, cursor_y - 1);
    buffer.erase(buffer.begin() + cursor_y);
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

Editor *Editor::handle_enter() {
    cursor_x = min(cursor_x + 1, static_cast<int>(buffer[cursor_y].size()));
    auto after = buffer[cursor_y].substr(cursor_x);
    buffer[cursor_y].erase(cursor_x);
    push();
    cursor_x = 0;
    buffer[cursor_y].insert(cursor_x, after);
    return this;
}

void Editor::draw() {
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    for (size_t y = 0; y < len(); y++) {
        DrawTextEx(font, buffer[y].c_str(),
                   (Vector2){.x = 0, .y = static_cast<float>(y * font_height)},
                   font_height, spacing, WHITE);
        DrawRectangle(x() + x() * char_width, y + y * font_height, 2,
                      font_height, WHITE);
    }
}

int main(int argc, char **argv) {

    InitWindow(WIDTH, HEIGHT, PROJECT_NAME);
    GuiLoadStyle("raygui/styles/dark/style_dark.rgs");

    auto editor = make_unique<Editor>("DejaVuSansMono.ttf", 24, 1.0f);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(BACKGROUND_COLOR));

        editor->draw();

        int ch;
        while ((ch = GetCharPressed()) > 0) {
            if (isascii(ch)) {
                editor->insert(static_cast<char>(ch));
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
            editor->handle_backspace();
        } else if (IsKeyPressed(KEY_ENTER) || IsKeyPressedRepeat(KEY_ENTER)) {
            editor->handle_enter();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
