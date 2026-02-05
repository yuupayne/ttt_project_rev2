#include <array>
#include <iostream>
#include <string>

#ifdef _WIN32
  #include <windows.h>
#endif

// 盤面サイズ
constexpr int kBoardSize = 3;

// 盤面の状態を保持する型
using Board = std::array<std::array<std::string, kBoardSize>, kBoardSize>;

// 画面をクリアする（追記方式にならないようにする）
void ClearScreen() {
    //std::cout << "\x1B[2J\x1B[H";
    std::cout << "\n";
}

// 盤面を初期化する
void InitializeBoard(Board &board) {
    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            board[row][col] = " ";
        }
    }
}

// 盤面を描画する（5行の固定フォーマット）
void DrawBoard(const Board &board) {
    for (int row = 0; row < kBoardSize; ++row) {
        std::cout << " " << board[row][0] << " | " << board[row][1] << " | " << board[row][2] << 
        "\t" << " " << row*3+1 << " | " << row*3+2 << " | " << row*3+3 << "\n";
        if (row != kBoardSize - 1) {
            std::cout << "---+---+---\t---+---+---\n";
        }
    }
    std::cout << "\n";
}

// 勝利判定を行う
bool CheckWin(const Board &board, const std::string &symbol) {
    // 縦横の判定
    for (int i = 0; i < kBoardSize; ++i) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) {
            return true;
        }
        if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) {
            return true;
        }
    }

    // 斜めの判定
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
        return true;
    }
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
        return true;
    }

    return false;
}

// 引き分け判定を行う
bool CheckDraw(const Board &board) {
    for (int row = 0; row < kBoardSize; ++row) {
        for (int col = 0; col < kBoardSize; ++col) {
            if (board[row][col] == " ") {
                return false;
            }
        }
    }
    return true;
}

// 1〜9の入力を受け取り、盤面座標に変換する
bool ReadMove(int &out_row, int &out_col) {
    std::string input;
    if (!std::getline(std::cin, input)) {
        return false;
    }

    if (input.size() != 1 || input[0] < '1' || input[0] > '9') {
        std::cout << "\n不正な入力です。\n";
        std::cout << "任意のマス目に対応した1〜9の数字を入力してください。\n";
        return false;
    }

    int number = input[0] - '1';
    out_row = number / kBoardSize;
    out_col = number % kBoardSize;
    return true;
}

// Windows のコンソールに対して UTF-8 を指定
// （cmd / PowerShell / Windows Terminal のいずれでも基本はここが効きます）
static void setup_utf8_console_for_cout() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  // 参考：Cランタイム側のコードページは触らないのが無難（環境依存で崩れることがある）
  // std::cout を前提にするなら _setmode(stdout, _O_U8TEXT) は基本的に避ける
#endif
}

int main() {
    setup_utf8_console_for_cout();

    Board board{};
    const std::array<std::string, 2> player_names = {"Player1", "Player2"};
    const std::array<std::string, 2> player_symbols = {"o", "×"};

    while (true) {
        InitializeBoard(board);
        int current_player = 0;

        while (true) {
            ClearScreen();
            DrawBoard(board);
            std::cout << player_names[current_player] << "の番です" 
                      << "（シンボル：" << player_symbols[current_player] << "）\n";
            std::cout << "任意のマス目に対応した1〜9を入力してください: ";

            int row = 0;
            int col = 0;
            if (!ReadMove(row, col)) {
                continue;
            }

            if (board[row][col] != " ") {
                std::cout << "\n既に配置済みのマスです。\n";
                continue;
            }

            board[row][col] = player_symbols[current_player];

            if (CheckWin(board, player_symbols[current_player])) {
                ClearScreen();
                DrawBoard(board);
                std::cout << player_names[current_player] << "（シンボル：" << player_symbols[current_player]
                          << "）が勝ちました！\n";
                break;
            }

            if (CheckDraw(board)) {
                ClearScreen();
                DrawBoard(board);
                std::cout << "引き分け\n";
                break;
            }

            current_player = 1 - current_player;
        }
    }

    return 0;
}
