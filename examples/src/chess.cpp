#include "chess.hpp"

using namespace chess;

int main () {
    Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Movelist moves;
    movegen::legalmoves(moves, board);

    auto ret = board.pieces(PieceType::QUEEN, Color::WHITE);
    std::cout << ret << '\n';
    for (const auto &move : moves) {
        std::cout << uci::moveToUci(move) << std::endl;
    }


    return 0;
}
