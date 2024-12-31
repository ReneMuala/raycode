#include <raylib.h>

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

// struct for editor config
struct EditorConfig {
    // Sizes
    float windowPadding;
    float verticalLineSpacing;
    float baseVerticalLineSpacing;
    int lineNumberWidth;
    int spaceBetweenNumbersAndText;
    float editorZoom;
    float zoomSpeed;
    float fontSize;
    float gridWidth;
    float gridHeight;
    float gridLineWidth;

    // Colors
    Color editorBackgroundColor;
    Color editorTextColor;
    Color editorLineNumbersColor;
    Color editorVerticalLineColor;
    Color editorCursorColor;
    Color editorGridColor;

    // debug
    bool debugGrid = false;
};

int main(void) {
    // Create window
    int screenWidth = 1280;
    int screenHeight = 720;
    // SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "GHDcode");
    // SetTargetFPS(60);

    // Editor Config
    EditorConfig editor{
        .windowPadding = 10.0f,
        .baseVerticalLineSpacing = 5.0f,
        .lineNumberWidth = 3,
        .spaceBetweenNumbersAndText = 2,
        .editorZoom = 2.0f,
        .zoomSpeed = 0.5f,
        .fontSize = 16.0f,
        .gridLineWidth = 2.0f,
        .editorBackgroundColor = Color{13, 17, 23},
        .editorTextColor = Color{255, 255, 255, 255},
        .editorLineNumbersColor = Color{255, 255, 255, 100},
        .editorVerticalLineColor = Color{255, 255, 255, 16},
        .editorCursorColor = Color{47, 129, 247, 170},
        .editorGridColor = Color{255, 0, 255, 100},
        .debugGrid = true,

    };
    editor.verticalLineSpacing =
        editor.baseVerticalLineSpacing * editor.editorZoom;
    editor.fontSize = int(16 * editor.editorZoom);

    Vector2 cursorPosition = Vector2{0.0f, 0.0f};
    cursorPosition.x = editor.lineNumberWidth + editor.spaceBetweenNumbersAndText;
    cursorPosition.y = 0;

    // Load Fonts
    Font font =
        LoadFontEx("resources/fonts/FiraCode-Regular.ttf", editor.fontSize, nullptr, 0);

    editor.gridWidth = MeasureTextEx(font, "A", editor.fontSize, 0).x;
    editor.gridHeight = MeasureTextEx(font, "A", editor.fontSize, 0).y;

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

    std::vector<std::string> editorData = splitLines(textData);
    int numLines = editorData.size();

    // Main loop (ESC to exit)
    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        BeginDrawing();

        // Background
        ClearBackground(editor.editorBackgroundColor);

        // FPS counter
        DrawFPS(screenWidth - 110, 10);

        // UI
        {
            // Change editor zoom with control+Q and control+E
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
                editor.editorZoom -= editor.zoomSpeed;
                editor.editorZoom = MAX(0.5f, editor.editorZoom);
                std::cout << "zoom: " << editor.editorZoom << std::endl;
                if (editor.editorZoom > 0.5f) {
                    // int fontSize = int(16 * editor.editorZoom);
                    editor.fontSize = int(16 * editor.editorZoom);
                    UnloadFont(font);
                    font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                      editor.fontSize, nullptr, 0);
                    editor.gridWidth =
                        MeasureTextEx(font, "A", editor.fontSize, 0).x;
                    editor.gridHeight =
                        MeasureTextEx(font, "A", editor.fontSize, 0).y;
                    editor.verticalLineSpacing =
                        editor.baseVerticalLineSpacing * editor.editorZoom;
                }
            }
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) {
                editor.editorZoom += editor.zoomSpeed;
                editor.editorZoom = MIN(5.0f, editor.editorZoom);
                std::cout << "zoom: " << editor.editorZoom << std::endl;
                if (editor.editorZoom < 5.0f) {
                    // int fontSize = int(16 * editor.editorZoom);
                    editor.fontSize = int(16 * editor.editorZoom);
                    UnloadFont(font);
                    font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                      editor.fontSize, nullptr, 0);
                    editor.gridWidth =
                        MeasureTextEx(font, "A", editor.fontSize, 0).x;
                    editor.gridHeight =
                        MeasureTextEx(font, "A", editor.fontSize, 0).y;
                    editor.verticalLineSpacing =
                        editor.baseVerticalLineSpacing * editor.editorZoom;
                }
            }
            // Toggle debug grid with CTRL+G
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G)) {
                editor.debugGrid = !editor.debugGrid;
            }

            // Change vertical spacing using Shift+E and Shift+Q
            if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_E)) {
                editor.baseVerticalLineSpacing += 5;
                editor.baseVerticalLineSpacing =
                    MIN(editor.baseVerticalLineSpacing, 100);
                editor.verticalLineSpacing =
                    editor.baseVerticalLineSpacing * editor.editorZoom;
            }
            if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Q)) {
                editor.baseVerticalLineSpacing -= 5;
                editor.baseVerticalLineSpacing =
                    MAX(editor.baseVerticalLineSpacing, 0);
                editor.verticalLineSpacing =
                    editor.baseVerticalLineSpacing * editor.editorZoom;
            }

            // Cursor movement
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_H)) {
                cursorPosition.x -= 1;
                cursorPosition.x = MAX(editor.lineNumberWidth + editor.spaceBetweenNumbersAndText, cursorPosition.x);
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_L)) {
                cursorPosition.x += 1;
                // TODO: not 80! limit the line width
                cursorPosition.x = MIN(80, cursorPosition.x);
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_K)) {
                cursorPosition.y -= 1;
                cursorPosition.y = MAX(0, cursorPosition.y);
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_J)) {
                cursorPosition.y += 1;
                cursorPosition.y = MIN(numLines - 1, cursorPosition.y);
            }
        }

        // Working grid (this is a grid that every character can be placed on)
        if (true) {
            // Draw the grid
            if (editor.debugGrid) {
                int gridRows =
                    static_cast<int>(screenHeight / editor.gridHeight);
                int gridCols = static_cast<int>(screenWidth / editor.gridWidth);

                // Draw vertical lines
                for (int i = 0; i < gridCols + 1; i++) {
                    DrawRectangle(i * editor.gridWidth + editor.windowPadding,
                                  editor.windowPadding, editor.gridLineWidth,
                                  screenHeight - editor.windowPadding * 2,
                                  editor.editorGridColor);
                }

                // Draw Horizontal lines
                for (int i = 0; i < gridRows + 1; i++) {
                    DrawRectangle(editor.windowPadding,
                                  i * editor.verticalLineSpacing +
                                      i * editor.gridHeight +
                                      editor.windowPadding,
                                  screenWidth - editor.windowPadding * 2,
                                  editor.gridLineWidth, editor.editorGridColor);
                    DrawRectangle(editor.windowPadding,
                                  i * editor.verticalLineSpacing +
                                      (i + 1) * editor.gridHeight +
                                      editor.windowPadding,
                                  screenWidth - editor.windowPadding * 2,
                                  editor.gridLineWidth, editor.editorGridColor);
                }
            }
        }

        // Draw the line numbers
        {
            // Line Numbers
            SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
            for (int i = 0; i < numLines; i++) {
                float x = editor.windowPadding;
                float y = i * editor.verticalLineSpacing +
                          i * editor.gridHeight + editor.windowPadding;
                std::string line_number_text =
                    leftPad(std::to_string(i + 1), editor.lineNumberWidth);
                DrawTextEx(font, line_number_text.c_str(), Vector2{x, y},
                           editor.fontSize, 0, editor.editorLineNumbersColor);
            }
        }

        // Draw the text character by character
        {
            // Text
            for (int i = 0; i < numLines; i++) {
                float x = editor.windowPadding +
                          editor.gridWidth * (editor.lineNumberWidth + 2);
                float y = i * editor.verticalLineSpacing +
                          i * editor.gridHeight + editor.windowPadding;
                std::string line_text = editorData[i];
                DrawTextEx(font, line_text.c_str(), Vector2{x, y},
                           editor.fontSize, 0, editor.editorTextColor);
            }
        }

        // Draw the cursor
        {
            DrawRectangle(cursorPosition.x * editor.gridWidth + editor.windowPadding,
                          (cursorPosition.y) * editor.gridHeight + cursorPosition.y * editor.verticalLineSpacing + editor.windowPadding,
                          editor.gridWidth, editor.gridHeight,
                          editor.editorCursorColor);
        }
        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font);
    CloseWindow();

    return 0;
}