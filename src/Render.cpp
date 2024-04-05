#include "App.h"

#include <string>
#include <unordered_map>
#include <random>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>

SDL_Texture *texture = NULL;

constexpr int WINDOW_WIDTH = 825;
int rows = 9, cols = 9;
int bombCount = 10;
float tileSize = 45.f;
Vec2f location = {(WINDOW_WIDTH - tileSize * cols) / 2, 90.f};
int difficulty = 0;
int flags = bombCount;

std::string closedTiles;
std::string tiles;
std::unordered_map<char, std::pair<int, int>> tileTextureIndex = {
    {'c', {0, 0}}, {'o', {1, 0}}, {'m', {0, 2}}, {'x', {2, 2}}, {'f', {0, 1}}, {'0', {3, 2}}, {'1', {0, 3}}, {'2', {1, 3}}, {'3', {2, 3}}, {'4', {3, 3}}, {'5', {0, 4}}, {'6', {1, 4}}, {'7', {2, 4}}, {'8', {3, 4}}};

void generateMines(std::string &tiles)
{
    closedTiles.assign(rows * cols, 'c');
    tiles.assign(rows * cols, 'o');

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, rows * cols - 1);

    std::set<int> values;
    std::vector<int> tile_points(rows * cols, 0);

    while (values.size() != bombCount)
    {
        int mine = dist(rng);
        values.insert(mine);
        tiles[mine] = 'm';
        tile_points[mine] = 300;
    }

    for (int i = 0; i < tiles.size(); i++)
    {
        if (tile_points[i] >= 300)
        {
            if (i % cols != cols - 1)
                tile_points[i + 1] += 1; // sağ
            if (i % cols != 0 && i != 0)
                tile_points[i - 1] += 1; // sol
            if (i + cols < rows * cols)
                tile_points[i + cols] += 1; // aşağı
            if (i - cols >= 0)
                tile_points[i - cols] += 1; // yukarı

            if ((i % cols != 0 && i - cols >= 0 && i != 0))
                tile_points[i - (cols + 1)] += 1; // sol yukarı
            if ((i % cols != 0 && i + cols < rows * cols && i != 0))
                tile_points[i + (cols - 1)] += 1; // sol aşağı
            if ((i % cols != cols - 1 && i - cols >= 0))
                tile_points[i - (cols - 1)] += 1; // sağ yukarı
            if ((i % cols != cols - 1 && i + cols < rows * cols))
                tile_points[i + (cols + 1)] += 1; // sağ aşağı
        }
    }

    for (size_t i = 0; i < tiles.size(); i++)
    {
        if (tile_points[i] < 300)
        {
            tiles[i] = tile_points[i] + '0';
        }
    }

    flags = bombCount;
}

void drawTiles(SDL_Renderer *renderer, std::string &tiles)
{
    SDL_FRect rect;
    SDL_FRect srcrect;
    std::pair<int, int> textureIndex;
    auto index = get2DIndex(0, cols);
    for (int i = 0; i < tiles.size(); i++)
    {
        textureIndex = tileTextureIndex[tiles[i]];
        srcrect = {textureIndex.first * 512.f, textureIndex.second * 512.f, 512.f, 512.f};
        index = get2DIndex(i, cols);
        rect = {location.x + index.second * tileSize, location.y + index.first * tileSize, tileSize, tileSize};
        SDL_RenderTexture(renderer, texture, &srcrect, &rect);
    }
}

void zeroSpread(std::string &tiles, std::string &closedTiles, int width, int height, int clickedIndex)
{
    std::vector<int> directions = {-1, 0, 1};
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    int clickedRow = clickedIndex / width;
    int clickedCol = clickedIndex % width;

    q.push({clickedRow, clickedCol});
    visited[clickedRow][clickedCol] = true;

    while (!q.empty())
    {
        auto current = q.front();
        q.pop();
        int row = current.first;
        int col = current.second;

        for (int dx : directions)
        {
            for (int dy : directions)
            {
                int newRow = row + dx;
                int newCol = col + dy;
                if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width && !visited[newRow][newCol])
                {
                    if (tiles[newRow * width + newCol] == '0')
                    {
                        closedTiles[newRow * width + newCol] = tiles[newRow * width + newCol];
                        q.push({newRow, newCol});
                        visited[newRow][newCol] = true;
                    }
                    else if (tiles[newRow * width + newCol] != 'm')
                    {
                        closedTiles[newRow * width + newCol] = tiles[newRow * width + newCol];
                        visited[newRow][newCol] = true;
                    }
                }
            }
        }
    }
}

void setDifficulty(SDL_Window *window, int &difficulty)
{
    if (difficulty > 2)
        difficulty = 0;

    switch (difficulty)
    {
    case 0:
        rows = 9,
        cols = 9;
        bombCount = 10;
        tileSize = 45.f;
        location = {(WINDOW_WIDTH - tileSize * cols) / 2, 90.f};
        break;
    case 1:
        rows = 16,
        cols = 16;
        bombCount = 40;
        tileSize = 28.f;
        location = {(WINDOW_WIDTH - tileSize * cols) / 2, 60.f};
        break;
    case 2:
        rows = 16,
        cols = 30;
        bombCount = 99;
        tileSize = 26.f;
        location = {(WINDOW_WIDTH - tileSize * cols) / 2, 90.f};
        break;
    default:
        break;
    }
    generateMines(tiles);
}

bool checkChord(std::string &closedTiles, int cols, int clickedIndex)
{
    int i = clickedIndex;
    int number = closedTiles[clickedIndex] - '0';
    int count = 0;

    if (i % cols != cols - 1)
        count += closedTiles[i + 1] == 'f' ? 1 : 0; // sağ
    if (i % cols != 0 && i != 0)
        count += closedTiles[i - 1] == 'f' ? 1 : 0; // sol
    if (i + cols < rows * cols)
        count += closedTiles[i + cols] == 'f' ? 1 : 0; // aşağı
    if (i - cols >= 0)
        count += closedTiles[i - cols] == 'f' ? 1 : 0; // yukarı
    if ((i % cols != 0 && i - cols >= 0 && i != 0))
        count += closedTiles[i - (cols + 1)] == 'f' ? 1 : 0; // sol yukarı
    if ((i % cols != 0 && i + cols < rows * cols && i != 0))
        count += closedTiles[i + (cols - 1)] == 'f' ? 1 : 0; // sol aşağı
    if ((i % cols != cols - 1 && i - cols >= 0))
        count += closedTiles[i - (cols - 1)] == 'f' ? 1 : 0; // sağ yukarı
    if ((i % cols != cols - 1 && i + cols < rows * cols))
        count += closedTiles[i + (cols + 1)] == 'f' ? 1 : 0; // sağ aşağı

    return number == count;
}

size_t chord(std::string &closedTiles, int cols, int clickedIndex)
{
    int i = clickedIndex;
    if (i % cols != cols - 1)
        if (closedTiles[i + 1] != 'f')
            closedTiles[i + 1] = tiles[i + 1]; // sağ
    if (i % cols != 0 && i != 0)
        if (closedTiles[i - 1] != 'f')
            closedTiles[i - 1] = tiles[i - 1]; // sol
    if (i + cols < rows * cols)
        if (closedTiles[i + cols] != 'f')
            closedTiles[i + cols] = tiles[i + cols]; // aşağı
    if (i - cols >= 0)
        if (closedTiles[i - cols] != 'f')
            closedTiles[i - cols] = tiles[i - cols]; // yukarı
    if ((i % cols != 0 && i - cols >= 0 && i != 0))
        if (closedTiles[i - (cols + 1)] != 'f')
            closedTiles[i - (cols + 1)] = tiles[i - (cols + 1)]; // sol yukarı
    if ((i % cols != 0 && i + cols < rows * cols && i != 0))
        if (closedTiles[i + (cols - 1)] != 'f')
            closedTiles[i + (cols - 1)] = tiles[i + (cols - 1)]; // sol aşağı
    if ((i % cols != cols - 1 && i - cols >= 0))
        if (closedTiles[i - (cols - 1)] != 'f')
            closedTiles[i - (cols - 1)] = tiles[i - (cols - 1)]; // sağ yukarı
    if ((i % cols != cols - 1 && i + cols < rows * cols))
        if (closedTiles[i + (cols + 1)] != 'f')
            closedTiles[i + (cols + 1)] = tiles[i + (cols + 1)]; // sağ aşağı

    size_t found = closedTiles.find('0');
    while (found != std::string::npos)
    { // Continue until no occurrence is found
        zeroSpread(tiles, closedTiles, cols, rows, (int)found);
        found = closedTiles.find('0', found + 1); // Find the next occurrence starting from the next position
    }

    return closedTiles.find('m');
}

float secondCounter = 0.f;
int second = 0;
bool paused = true;
bool lost = false;
bool won = false;

bool atlasFail = false;

std::unordered_map<Uint8, bool> buttonStates;

void App::Setup()
{
    static Texture atlas("atlas.png", renderer);
    texture = atlas.texture;

    if (!texture)
    {
        SDL_Log("Failed to Load atlas.png");
        atlasFail = true;
        SetStringTextureColorMode({255, 50, 50});
    }

    generateMines(tiles);
}

void App::Update(SDL_Event &event, float deltaTime)
{
    while (SDL_PollEvent(&event))
    {
        // ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            quit = true;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                quit = true;

            else if (event.key.keysym.sym == SDLK_k)
            {
                SDL_Log("fps = %f", 1 / deltaTime);
            }
            else if (event.key.keysym.sym == SDLK_F2)
            {
                generateMines(tiles);
                lost = false;
                won = false;
                paused = true;
                second = 0;
                secondCounter = 0;
            }
            else if (event.key.keysym.sym == SDLK_v)
            {
                difficulty++;
                setDifficulty(window, difficulty);
            }

            break;
        case SDL_EVENT_MOUSE_MOTION:
            float mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (event.button.button == SDL_BUTTON_MIDDLE || (buttonStates[SDL_BUTTON_LEFT] && buttonStates[SDL_BUTTON_RIGHT]))
            {
                std::replace(closedTiles.begin(), closedTiles.end(), 'o', 'c');
                for (size_t i = 0; i < tiles.size(); i++)
                {
                    auto index = get2DIndex((int)i, cols);
                    SDL_FRect rect = {1 + location.x + index.second * tileSize, 1 + location.y + index.first * tileSize, tileSize - 1, tileSize - 1};
                    if (IsMouseInsideRect(mouseX, mouseY, rect))
                    {
                        closedTiles[i] = closedTiles[i] == 'c' ? 'o' : closedTiles[i];
                        if (i % cols != cols - 1)
                            closedTiles[i + 1] = closedTiles[i + 1] == 'c' ? 'o' : closedTiles[i + 1]; // sağ
                        if (i % cols != 0 && i != 0)
                            closedTiles[i - 1] = closedTiles[i - 1] == 'c' ? 'o' : closedTiles[i - 1]; // sol
                        if (i + cols < rows * cols)
                            closedTiles[i + cols] = closedTiles[i + cols] == 'c' ? 'o' : closedTiles[i + cols]; // aşağı
                        if (i - cols >= 0)
                            closedTiles[i - cols] = closedTiles[i - cols] == 'c' ? 'o' : closedTiles[i - cols]; // yukarı
                        if ((i % cols != 0 && i - cols >= 0 && i != 0))
                            closedTiles[i - (cols + 1)] = closedTiles[i - (cols + 1)] == 'c' ? 'o' : closedTiles[i - (cols + 1)]; // sol yukarı
                        if ((i % cols != 0 && i + cols < rows * cols && i != 0))
                            closedTiles[i + (cols - 1)] = closedTiles[i + (cols - 1)] == 'c' ? 'o' : closedTiles[i + (cols - 1)]; // sol aşağı
                        if ((i % cols != cols - 1 && i - cols >= 0))
                            closedTiles[i - (cols - 1)] = closedTiles[i - (cols - 1)] == 'c' ? 'o' : closedTiles[i - (cols - 1)]; // sağ yukarı
                        if ((i % cols != cols - 1 && i + cols < rows * cols))
                            closedTiles[i + (cols + 1)] = closedTiles[i + (cols + 1)] == 'c' ? 'o' : closedTiles[i + (cols + 1)]; // sağ aşağı
                    }
                }
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_GetMouseState(&mouseX, &mouseY);
            buttonStates[event.button.button] = true;
            if (event.button.button == SDL_BUTTON_MIDDLE || (buttonStates[SDL_BUTTON_LEFT] && buttonStates[SDL_BUTTON_RIGHT]))
            {
                for (size_t i = 0; i < tiles.size(); i++)
                {
                    auto index = get2DIndex((int)i, cols);
                    SDL_FRect rect = {1 + location.x + index.second * tileSize, 1 + location.y + index.first * tileSize, tileSize - 1, tileSize - 1};
                    if (IsMouseInsideRect(mouseX, mouseY, rect))
                    {
                        if (checkChord(closedTiles, cols, (int)i))
                        {
                            size_t found = chord(closedTiles, cols, (int)i);
                            if (found != std::string::npos)
                            {
                                closedTiles = tiles;
                                closedTiles[found] = 'x';

                                lost = true;
                                paused = true;
                            }
                            return;
                        }
                        closedTiles[i] = closedTiles[i] == 'c' ? 'o' : closedTiles[i];
                        if (i % cols != cols - 1)
                            closedTiles[i + 1] = closedTiles[i + 1] == 'c' ? 'o' : closedTiles[i + 1]; // sağ
                        if (i % cols != 0 && i != 0)
                            closedTiles[i - 1] = closedTiles[i - 1] == 'c' ? 'o' : closedTiles[i - 1]; // sol
                        if (i + cols < rows * cols)
                            closedTiles[i + cols] = closedTiles[i + cols] == 'c' ? 'o' : closedTiles[i + cols]; // aşağı
                        if (i - cols >= 0)
                            closedTiles[i - cols] = closedTiles[i - cols] == 'c' ? 'o' : closedTiles[i - cols]; // yukarı
                        if ((i % cols != 0 && i - cols >= 0 && i != 0))
                            closedTiles[i - (cols + 1)] = closedTiles[i - (cols + 1)] == 'c' ? 'o' : closedTiles[i - (cols + 1)]; // sol yukarı
                        if ((i % cols != 0 && i + cols < rows * cols && i != 0))
                            closedTiles[i + (cols - 1)] = closedTiles[i + (cols - 1)] == 'c' ? 'o' : closedTiles[i + (cols - 1)]; // sol aşağı
                        if ((i % cols != cols - 1 && i - cols >= 0))
                            closedTiles[i - (cols - 1)] = closedTiles[i - (cols - 1)] == 'c' ? 'o' : closedTiles[i - (cols - 1)]; // sağ yukarı
                        if ((i % cols != cols - 1 && i + cols < rows * cols))
                            closedTiles[i + (cols + 1)] = closedTiles[i + (cols + 1)] == 'c' ? 'o' : closedTiles[i + (cols + 1)]; // sağ aşağı
                    }
                }
            }
            else if (buttonStates[SDL_BUTTON_LEFT])
            {
                if (IsMouseInsideRect(mouseX, mouseY, {(WINDOW_WIDTH - 28) / 2, 10, 28, 28}))
                {
                    generateMines(tiles);
                    lost = false;
                    paused = true;
                    won = false;
                    second = 0;
                    secondCounter = 0;
                    return;
                }

                for (size_t i = 0; i < tiles.size(); i++)
                {
                    auto index = get2DIndex((int)i, cols);
                    SDL_FRect rect = {1 + location.x + index.second * tileSize, 1 + location.y + index.first * tileSize, tileSize - 1, tileSize - 1};

                    if (IsMouseInsideRect(mouseX, mouseY, rect))
                    {

                        if (closedTiles[i] == 'f')
                            return;

                        if (paused && !lost)
                            paused = false;

                        if (std::count(closedTiles.begin(), closedTiles.end(), 'f') + std::count(closedTiles.begin(), closedTiles.end(), 'c') == rows * cols)
                        {
                            std::replace(closedTiles.begin(), closedTiles.end(), 'f', 'c');
                            while (tiles[i] != '0')
                            {
                                generateMines(tiles);
                            }
                            zeroSpread(tiles, closedTiles, cols, rows, (int)i);
                            closedTiles[i] = tiles[i];

                            return;
                        }
                        if (tiles[i] == 'm')
                        {
                            closedTiles = tiles;
                            closedTiles[i] = 'x';

                            lost = true;
                            paused = true;
                            return;
                        }

                        if (tiles[i] == '0')
                        {
                            zeroSpread(tiles, closedTiles, cols, rows, (int)i);
                            closedTiles[i] = tiles[i];
                            return;
                        }

                        closedTiles[i] = tiles[i];
                    }
                }
            }
            else if (buttonStates[SDL_BUTTON_RIGHT])
            {
                if (IsMouseInsideRect(mouseX, mouseY, {(WINDOW_WIDTH - 28) / 2, 10, 28, 28}))
                {
                    difficulty++;
                    setDifficulty(window, difficulty);
                    lost = false;
                    won = false;
                    paused = true;
                    second = 0;
                    secondCounter = 0;
                }

                if (won)
                    return;

                for (size_t i = 0; i < tiles.size(); i++)
                {
                    auto index = get2DIndex((int)i, cols);
                    SDL_FRect rect = {1 + location.x + index.second * tileSize, 1 + location.y + index.first * tileSize, tileSize - 1, tileSize - 1};
                    if (IsMouseInsideRect(mouseX, mouseY, rect))
                    {
                        if (closedTiles[i] == 'c')
                        {
                            closedTiles[i] = 'f';
                        }
                        else if (closedTiles[i] == 'f')
                        {
                            closedTiles[i] = 'c';
                        }
                    }
                }
            }

            flags = bombCount - (int)std::count(closedTiles.begin(), closedTiles.end(), 'f');
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            SDL_GetMouseState(&mouseX, &mouseY);
            if (event.button.button == SDL_BUTTON_MIDDLE || (buttonStates[SDL_BUTTON_LEFT] && buttonStates[SDL_BUTTON_RIGHT]))
            {
                std::replace(closedTiles.begin(), closedTiles.end(), 'o', 'c');
            }

            buttonStates[event.button.button] = false;
            break;
        }
    }

    if (!paused && !won && !lost)
    {
        secondCounter += deltaTime;
        if (secondCounter >= 1.f)
        {
            secondCounter -= 1.f;
            second++;

            auto n = std::inner_product(std::begin(tiles), std::end(tiles),
                                        std::begin(closedTiles),
                                        size_t(0),
                                        std::plus<>(),
                                        std::not_equal_to<>());

            if (n == bombCount)
            {
                std::replace(closedTiles.begin(), closedTiles.end(), 'c', 'f');
                paused = true;
                won = true;
                flags = 0;
            }
        }
    }
}

void App::Draw()
{
    // ImGuiIO &io = this->ImguiNewFrame();
    this->SetRenderDrawColor({40, 48, 62});
    SDL_RenderClear(renderer);

    DrawString(std::to_string(flags), {75, 10, 30, 30});
    DrawString(std::to_string(second), {750, 10, 30, 30});
    SDL_FRect rect = {(WINDOW_WIDTH - 28) / 2, 10, 28, 28};
    SDL_FRect srcrect = {won ? 0 : (lost ? 1 : 3) * 512.f, (won ? 2 : (lost ? 2 : 0)) * 512.f, 512.f, 512.f};
    SDL_RenderTexture(renderer, texture, &srcrect, &rect);

    drawTiles(renderer, closedTiles);

    if (atlasFail)
        DrawString("Unable to load atlas.png", {50, 50, 30, 30});

    // this->ImguiRender();
    SDL_RenderPresent(renderer);
}