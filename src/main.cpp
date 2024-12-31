#include <raylib.h>

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

int main(void) {
    // Create window
    int screenWidth = 1280;
    int screenHeight = 720;
    // SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "GHDcode");
    // SetTargetFPS(60);

    // Editor Variables
    const float line_x_offset = 10.0f;
    const float scrollSpeed = 6.0f;
    float editorZoom = 1.0f;
    float vertical_line_padding = 10.0f;
    float vertical_line_width_multiplier = 0.5f;
    Color editorBackgroundColor = Color{13, 17, 23};
    Color editorTextColor = Color{255, 255, 255, 255};
    Color editorLineNumbersColor = Color{255, 255, 255, 100};
    Color editorVerticalLineColor = Color{255, 255, 255, 16};

    // Load Fonts
    Font font =
        LoadFontEx("resources/fonts/FiraCode-Regular.ttf", 16, nullptr, 0);

    // Data
    std::string textData = R"(Hello World!

This is GHDcode, a featureless and fast text editor
written using raylib.

test
test
test
test
test
test
test

By Gholamreza Dar
    )";

    std::vector<std::string> lines = splitLines(textData);
    int numLines = lines.size();

    // Main loop (ESC to exit)
    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        BeginDrawing();

        // Background
        ClearBackground(editorBackgroundColor);

        // FPS counter
        DrawFPS(screenWidth - 110, 10);

        // Change editor zoom with control+scroll
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_Q)) {
            editorZoom -= scrollSpeed * GetFrameTime();
            editorZoom = editorZoom < 0.5f ? 0.5f : editorZoom;
            editorZoom = editorZoom > 5.0f ? 5.0f : editorZoom;
            if (editorZoom > 0.5f) {
                UnloadFont(font);
                int fontSize = int(16 * editorZoom);
                font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                  fontSize, nullptr, 0);
            }
        }
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_E)) {
            editorZoom += scrollSpeed * GetFrameTime();
            editorZoom = editorZoom < 0.5f ? 0.5f : editorZoom;
            editorZoom = editorZoom > 5.0f ? 5.0f : editorZoom;
            if (editorZoom < 5.0f) {
                UnloadFont(font);
                int fontSize = int(16 * editorZoom);
                font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                  fontSize, nullptr, 0);
            }
        }

        // Line Numbers
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
        float numbers_font_size = font.baseSize * editorZoom;
        numbers_font_size = font.baseSize;
        for (int i = 0; i < numLines; i++) {
            float x = line_x_offset;
            float y = 10.0f + i * numbers_font_size;
            std::string line_number_text = leftPad(std::to_string(i + 1), 3);
            DrawTextEx(font, line_number_text.c_str(), Vector2{x, y},
                       numbers_font_size, 0, editorLineNumbersColor);
        }

        float numbers_max_width =
            MeasureTextEx(font, leftPad(std::to_string(numLines), 3).c_str(),
                          numbers_font_size, 0).x;
        // Vertical Line (right side of the numbers)
        DrawRectangle(line_x_offset + numbers_max_width + vertical_line_padding * editorZoom * 0.5,
                      10.0f,
                      vertical_line_width_multiplier * editorZoom * 2 < 1.0f
                          ? 1.0f
                          : vertical_line_width_multiplier * editorZoom * 2,
                      screenHeight - 20.0f, editorVerticalLineColor);

        // Text
        for (int i = 0; i < numLines; i++) {
            float x = line_x_offset + numbers_max_width + vertical_line_padding * 2 * editorZoom;
            float y = 10.0f + i * numbers_font_size;
            std::string line_text = lines[i];
            DrawTextEx(font, line_text.c_str(), Vector2{x, y},
                       numbers_font_size, 0, editorTextColor);
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font);
    CloseWindow();

    return 0;
}