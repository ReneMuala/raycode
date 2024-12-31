#include <raylib.h>

#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

// struct for editor config
struct EditorConfig {
    // Sizes
    float verticalLineSpacing;
    float baseVerticalLineSpacing;
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
        .baseVerticalLineSpacing = 40.0f,
        .editorZoom = 1.0f,
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
    editor.verticalLineSpacing = editor.baseVerticalLineSpacing * editor.editorZoom;

    Vector2 cursorPosition = Vector2{0.0f, 0.0f};

    // Load Fonts
    Font font =
        LoadFontEx("resources/fonts/FiraCode-Regular.ttf", 16, nullptr, 0);
    
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

        // Change editor zoom with control+Q and control+E
        {
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
                    editor.verticalLineSpacing = editor.baseVerticalLineSpacing * editor.editorZoom;
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
                    editor.verticalLineSpacing = editor.baseVerticalLineSpacing * editor.editorZoom;
                }
            }
        }


        // Working grid (this is a grid that every character can be placed on)
        if(true)
        {
            // Debug text
            DrawTextEx(font, "A", Vector2{0.0f, 0.0f}, editor.fontSize, 0, editor.editorTextColor);
            DrawTextEx(font, "B", Vector2{1.0f*editor.gridWidth, 0.0f*editor.gridHeight}, editor.fontSize, 0, editor.editorTextColor);
            DrawTextEx(font, "C", Vector2{0.0f*editor.gridWidth, 1.0f*editor.gridHeight+editor.verticalLineSpacing}, editor.fontSize, 0, editor.editorTextColor);
            DrawTextEx(font, "D", Vector2{1.0f*editor.gridWidth, 1.0f*editor.gridHeight+editor.verticalLineSpacing}, editor.fontSize, 0, editor.editorTextColor);
            

            // Draw the grid
            if (editor.debugGrid) {
                int gridRows = static_cast<int>(screenHeight / editor.gridHeight);
                int gridCols = static_cast<int>(screenWidth / editor.gridWidth);

                // Draw vertical lines
                for(int i=0; i<gridCols+1; i++) {
                    DrawRectangle(i*editor.gridWidth, 0, editor.gridLineWidth, screenHeight, editor.editorGridColor);
                }

                // Draw Horizontal lines
                for(int i=0; i<gridRows/2+1; i++) {
                    DrawRectangle(0, i*editor.verticalLineSpacing + i*editor.gridHeight, screenWidth, editor.gridLineWidth, editor.editorGridColor);
                    DrawRectangle(0, i*editor.verticalLineSpacing + (i+1)*editor.gridHeight, screenWidth, editor.gridLineWidth, editor.editorGridColor);
                }

            }
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font);
    CloseWindow();

    return 0;
}