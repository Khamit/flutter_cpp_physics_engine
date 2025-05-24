import 'dart:ffi';
import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:gigatap/engine/jewel_struct.dart';

// for example swap_and_match ---------
// Native typedef
typedef SwapAndMatchNative = Int32 Function(Int32, Int32, Int32, Int32);
// Dart typedef
typedef SwapAndMatchDart = int Function(int, int, int, int);
// ------------
// Native typedefs
typedef CheckAndClearMatchesNative = Int32 Function();
typedef RefillBoardNative = Void Function();

// init board with chengable param
typedef InitBoardNative = Void Function(
    Int32 rows, Int32 cols, Float cellSize, Int32 colorCount, Int32 shapeCount);
typedef InitBoardDart = void Function(
    int rows, int cols, double cellSize, int colorCount, int shapeCount);

typedef GetJewelDataNative = JewelStruct Function(Int32 row, Int32 col);
typedef GetJewelColorNative = Int32 Function(Int32 row, Int32 col);
typedef CreateBodyNative = Int32 Function(
    Float posX, Float posY, Float velX, Float velY, Float mass);

// Dart typedefs
typedef CheckAndClearMatchesDart = int Function();
typedef RefillBoardDart = void Function();

typedef GetJewelDataDart = JewelStruct Function(int row, int col);
typedef GetJewelColorDart = int Function(int row, int col);
typedef CreateBodyDart = int Function(
    double posX, double posY, double velX, double velY, double mass);

// Native match_only
typedef MatchOnlyNative = Int32 Function();
typedef MatchOnlyDart = int Function();

// dropped func native
typedef StepDropLogicNative = Int32 Function();
typedef StepDropLogicDart = int Function();

// has valid func
typedef HasValidMoveNative = Int32 Function();
typedef HasValidMoveDart = int Function();

class GameEngine {
  static final DynamicLibrary _lib = (() {
    try {
      if (Platform.isAndroid) {
        return DynamicLibrary.open('libgame_engine.so');
      } else if (Platform.isIOS) {
        return DynamicLibrary.process(); // iOS static linking
      } else if (Platform.isMacOS) {
        return DynamicLibrary.open('libgame_engine.dylib');
      } else if (Platform.isWindows) {
        return DynamicLibrary.open('game_engine.dll');
      } else {
        return DynamicLibrary.open('libgame_engine.so'); // Linux fallback
      }
    } catch (e) {
      if (kDebugMode) {
        print('❌ Failed to load native library: $e');
      }
      rethrow;
    }
  })();

  // swap_and_match
  static final SwapAndMatchDart _swapAndMatch = _lib
      .lookupFunction<SwapAndMatchNative, SwapAndMatchDart>('swap_and_match');

  static int swapAndMatch(int r1, int c1, int r2, int c2) =>
      _swapAndMatch(r1, c1, r2, c2);

  // Lookup native functions
  static final CheckAndClearMatchesDart _checkMatches =
      _lib.lookupFunction<CheckAndClearMatchesNative, CheckAndClearMatchesDart>(
          'check_and_clear_matches');

  static final RefillBoardDart _refillBoard =
      _lib.lookupFunction<RefillBoardNative, RefillBoardDart>('refill_board');

  static final GetJewelDataDart _getJewelData =
      _lib.lookupFunction<GetJewelDataNative, GetJewelDataDart>(
          'get_jewel_data_ffi');

  static final InitBoardDart _initBoard =
      _lib.lookupFunction<InitBoardNative, InitBoardDart>('init_board');

  static final GetJewelColorDart _getColor =
      _lib.lookupFunction<GetJewelColorNative, GetJewelColorDart>(
          'get_jewel_color_at');

  static final CreateBodyDart _createBody =
      _lib.lookupFunction<CreateBodyNative, CreateBodyDart>('create_body');

// ---------- Native match only ---------
  static final MatchOnlyDart _matchOnly =
      _lib.lookupFunction<MatchOnlyNative, MatchOnlyDart>('match_only');

  static int matchOnly() => _matchOnly();
// ---------- Native match only ---------

// has valid
  static final HasValidMoveDart _hasValidMove = _lib
      .lookupFunction<HasValidMoveNative, HasValidMoveDart>('has_valid_move');

// dropped
  static final StepDropLogicDart _stepDropLogic =
      _lib.lookupFunction<StepDropLogicNative, StepDropLogicDart>(
          'step_drop_logic');

  static bool _initialized = false;
// has valid
  static bool hasValidMove() => _hasValidMove() != 0;

  static void initBoard(
      int rows, int cols, double cellSize, int colorCount, int shapeCount) {
    try {
      if (colorCount <= 0 || shapeCount <= 0) {
        throw Exception("Invalid jewel type counts");
      }

      _initBoard(rows, cols, cellSize, colorCount, shapeCount);
      _initialized = true;
    } catch (e) {
      if (kDebugMode) {
        print('❌ initBoard failed: $e');
      }
      rethrow;
    }
  }

  static bool stepDropLogic() {
    try {
      return _stepDropLogic() != 0;
    } catch (e) {
      if (kDebugMode) print('❌ stepDropLogic failed: $e');
      return false;
    }
  }

  static int getJewelColor(int row, int col) {
    if (!_initialized) {
      throw Exception("⚠️ GameEngine not initialized. Call initBoard() first.");
    }
    try {
      return _getColor(row, col);
    } catch (e) {
      if (kDebugMode) {
        print('❌ getJewelColor failed: $e');
      }
      return -1;
    }
  }

  static int createBody(
      double posX, double posY, double velX, double velY, double mass) {
    try {
      return _createBody(posX, posY, velX, velY, mass);
    } catch (e) {
      if (kDebugMode) {
        print('❌ createBody failed: $e');
      }
      return -1;
    }
  }

  static int checkAndClearMatches() {
    try {
      return _checkMatches();
    } catch (e) {
      if (kDebugMode) {
        print('❌ checkAndClearMatches failed: $e');
      }
      return 0;
    }
  }

  static void refillBoard() {
    try {
      _refillBoard();
      // Inside refillBoard()
      print("🚨 refillBoard() called - refilling jewels...");
    } catch (e) {
      if (kDebugMode) {
        print('❌ refillBoard failed: $e');
      }
    }
  }

  static JewelData getJewelData(int row, int col) {
    final raw = _getJewelData(row, col);
    return JewelData.fromFFI(raw);
  }

  Future<void> processMatchesLoop(Function playExplosionAnimation) async {
    bool hasMatches = true;

    while (hasMatches) {
      final score = checkAndClearMatches();

      if (score == 0) {
        hasMatches = false;
        break;
      }

      await playExplosionAnimation();
      await Future.delayed(Duration(milliseconds: 300));
      refillBoard();
      await Future.delayed(Duration(milliseconds: 200));
    }
  }
}
