import 'dart:async';

import 'package:gigatap/engine/core/ffi_engine.dart';

typedef ExplosionCallback = Future<void> Function();

class MatchProcessor {
  final int gridSize;

  MatchProcessor({required this.gridSize});
  Future<void> processMatches({
    required Function(int score) onScoreUpdate,
    required Function() onBoardUpdate,
    required ExplosionCallback onExplosion,
  }) async {
    bool keepMatching = true;

    while (keepMatching) {
      final cleared = GameEngine.checkAndClearMatches();

      if (cleared > 0) {
        onScoreUpdate(cleared);

        await onExplosion();

        await Future.delayed(Duration(milliseconds: 300));

        GameEngine.refillBoard();

        onBoardUpdate();

        await Future.delayed(Duration(milliseconds: 200));
      } else {
        // No matches now, check if any valid moves left!
        if (!GameEngine.hasValidMove()) {
          // no valid moves - trigger game over or whatever we need
          keepMatching = false;
        } else {
          //Valid moves available, but no matches at this step, so end loop...
          keepMatching = false;
        }
      }
    }
  }
}
