import 'package:gigatap/engine/core/ffi_engine.dart';
// im not sure about this code...why i check here...maybe better do all inside cpp files? ... nah that is for demo.
class GameRules {
  /// Check if two tiles are adjacent (only 1 cell apart)
  static bool isAdjacent(int r1, int c1, int r2, int c2) {
    final rowDiff = (r1 - r2).abs();
    final colDiff = (c1 - c2).abs();
    return (rowDiff + colDiff) == 1;
  }

  /// Check if swap is valid (adjacent AND creates a match)
  static bool isValidSwap(
      int r1, int c1, int r2, int c2, List<List<int>> board) {
    if (!isAdjacent(r1, c1, r2, c2)) return false;

    final tempBoard = board.map((row) => [...row]).toList(); // clone

    // Swap temporarily
    final temp = tempBoard[r1][c1];
    tempBoard[r1][c1] = tempBoard[r2][c2];
    tempBoard[r2][c2] = temp;

    // Check for matches at swapped positions
    return _hasMatchAt(tempBoard, r1, c1) || _hasMatchAt(tempBoard, r2, c2);
  }

  static bool _hasMatchAt(List<List<int>> board, int row, int col) {
    final color = board[row][col];
    int count = 1;

    // Horizontal check
    int c = col - 1;
    while (c >= 0 && board[row][c] == color) {
      count++;
      c--;
    }
    c = col + 1;
    while (c < board[row].length && board[row][c] == color) {
      count++;
      c++;
    }
    if (count >= 3) return true;

    // Vertical check
    count = 1;
    int r = row - 1;
    while (r >= 0 && board[r][col] == color) {
      count++;
      r--;
    }
    r = row + 1;
    while (r < board.length && board[r][col] == color) {
      count++;
      r++;
    }
    return count >= 3;
  }

  /// Use native function to check if any valid swap exists
  static bool isAnyValidSwap(List<List<int>> board) {
    // Make sure native board state is synced with the Dart board!!!!!
    return GameEngine.hasValidMove();
  }
}
