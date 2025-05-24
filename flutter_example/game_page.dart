import 'dart:async';
import 'dart:math';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:gigatap/dialogs/no_matches.dart';
import 'package:gigatap/engine/jewel_struct.dart';

import 'engine/core/ffi_engine.dart';
import 'engine/rules/game_rules.dart';

class GamePage extends StatefulWidget {
  const GamePage({super.key});

  @override
  GamePageState createState() => GamePageState();
}

class GamePageState extends State<GamePage> {
  static const int gridSize = 5;
  late List<List<JewelData>> boardData;

  final random = Random();
  int? selectedRow;
  int? selectedCol;
  int score = 0;
  int timeLeft = 60;
  Timer? timer;

  bool isProcessing = false;
  bool isBoardReady = false;
  bool boardLoadFailed = false;
  bool _isCheckingNoMatches = false;

  bool showLightning = false;

  @override
  void initState() {
    super.initState();
    _startGame();
  }

  void _startGame() {
    _initializeBoard();

    // Fallback timeout: if board doesn't load in 5 seconds, show error
    Future.delayed(Duration(seconds: 5), () {
      if (!isBoardReady && mounted) {
        setState(() {
          boardLoadFailed = true;
        });
      }
    });
  }

  void _startTimer() {
    timer?.cancel(); // cancel if already running
    timer = Timer.periodic(Duration(seconds: 1), (t) {
      if (timeLeft <= 0) {
        t.cancel();
        _showGameOver();
      } else {
        setState(() => timeLeft--);
      }
    });
  }

  @override
  void dispose() {
    timer?.cancel();
    super.dispose();
  }

  void _initializeBoard() {
    try {
      GameEngine.initBoard(gridSize, gridSize, 2.0, 6, 6);

      boardData = List.generate(
        gridSize,
        (r) => List.generate(
          gridSize,
          (c) => GameEngine.getJewelData(r, c),
        ),
      );

      setState(() {
        isBoardReady = true;
      });

      _startTimer();

      _checkMatches();
    } catch (e) {
      if (kDebugMode) {
        print('❌ initBoard failed: $e');
      }
      if (!mounted) return;
      setState(() {
        boardLoadFailed = true;
      });
    }
  }

  void _showGameOver() {
    showDialog(
      context: context,
      builder: (_) => AlertDialog(
        title: Text('Game Over'),
        content: Text('Final Score: $score'),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.pop(context);
              setState(() {
                score = 0;
                timeLeft = 60;
                _startGame();
              });
            },
            child: Text('Play Again'),
          ),
        ],
      ),
    );
  }

  void _handleTileTap(int row, int col) {
    if (isProcessing) return;

    if (selectedRow == null || selectedCol == null) {
      setState(() {
        selectedRow = row;
        selectedCol = col;
      });
    } else {
      _swapJewels(selectedRow!, selectedCol!, row, col);
    }
  }

  Future<void> _swapJewels(int r1, int c1, int r2, int c2) async {
    final colorBoard =
        boardData.map((row) => row.map((j) => j.color).toList()).toList();

    if (!GameRules.isValidSwap(r1, c1, r2, c2, colorBoard)) {
      // Animate invalid swap:
      setState(() {
        // Swap data locally for animation
        final temp = boardData[r1][c1];
        boardData[r1][c1] = boardData[r2][c2];
        boardData[r2][c2] = temp;
        selectedRow = null;
        selectedCol = null;
        isProcessing = true; // prevent other inputs during animation
      });

      await Future.delayed(Duration(milliseconds: 300));

      setState(() {
        // Swap back after delay
        final temp = boardData[r1][c1];
        boardData[r1][c1] = boardData[r2][c2];
        boardData[r2][c2] = temp;
        isProcessing = false;
      });

      return;
    }

    // Valid swap!!: proceed as normal
    setState(() => isProcessing = true);

    try {
      int gainedScore = GameEngine.swapAndMatch(r1, c1, r2, c2);
      if (gainedScore > 0) {
        setState(() {
          score += gainedScore;
          boardData = List.generate(
            gridSize,
            (r) =>
                List.generate(gridSize, (c) => GameEngine.getJewelData(r, c)),
          );
        });
      } else {
        // No matches made but valid swap: still need to update board?
        setState(() {
          boardData = List.generate(
            gridSize,
            (r) =>
                List.generate(gridSize, (c) => GameEngine.getJewelData(r, c)),
          );
        });
      }

// Always check matches!!! and follow-up with no-match check!!!
      await Future.delayed(Duration(milliseconds: 300));
      await _checkMatches();
    } catch (e) {
      if (kDebugMode) print('❌ Swap error: $e');
    } finally {
      setState(() {
        selectedRow = null;
        selectedCol = null;
        isProcessing = false;
      });
    }
  }

  Future<void> _checkNoMatches([int retries = 5]) async {
    if (_isCheckingNoMatches || retries <= 0) return;
    _isCheckingNoMatches = true;

    try {
      for (int attempt = 0; attempt < retries; attempt++) {
        final hasMoves = GameEngine.hasValidMove();
        if (hasMoves) break;

        if (timeLeft > 5) {
          print(
              "XXX No valid moves detected. Refilling board (attempt $attempt)...");

          await showDialog(
            context: context,
            builder: (_) => const NoMatchesDialog(),
          );

          GameEngine.refillBoard();

          await Future.delayed(Duration(milliseconds: 150));

          setState(() {
            boardData = List.generate(
              gridSize,
              (r) =>
                  List.generate(gridSize, (c) => GameEngine.getJewelData(r, c)),
            );
          });

          print("(-_-) Board refilled after no valid moves.");

          await Future.delayed(Duration(milliseconds: 150));
        } else {
          break;
        }
      }
    } catch (e) {
      print("(ERROR) Error in _checkNoMatches: $e");
    } finally {
      _isCheckingNoMatches = false;
    }
  }

// check matches
  Future<void> _checkMatches() async {
    setState(() => showLightning = true);
    await Future.delayed(Duration(milliseconds: 100));
    setState(() => showLightning = false);

    bool hasMatches = true;

    while (hasMatches) {
      int cleared = GameEngine.matchOnly();

      if (cleared == 0) {
        hasMatches = false;
      } else {
        setState(() {
          score += cleared;
        });

        bool hasDrops = true;
        while (hasDrops) {
          hasDrops = GameEngine.stepDropLogic();
          setState(() {
            boardData = List.generate(
                gridSize,
                (r) => List.generate(
                    gridSize, (c) => GameEngine.getJewelData(r, c)));
          });
          await Future.delayed(Duration(milliseconds: 100));
        }

        await Future.delayed(Duration(milliseconds: 300));
      }
    }

    // check no moves after all matches cleared
    _checkNoMatches();
  }

  Widget _buildJewel(int colorId, int shapeId, int row, int col) {
    final color = JewelColor.values[colorId % JewelColor.values.length];
    final shape = JewelShape.values[shapeId % JewelShape.values.length];

    final colorMap = {
      JewelColor.red: Colors.red,
      JewelColor.blue: Colors.blue,
      JewelColor.green: Colors.green,
      JewelColor.yellow: Colors.yellow,
      JewelColor.purple: Colors.purple,
      JewelColor.orange: Colors.orange,
    };

    final shapeMap = {
      JewelShape.circle: Icons.circle,
      JewelShape.square: Icons.square,
      JewelShape.triangle: Icons.change_history,
      JewelShape.diamond: Icons.diamond,
      JewelShape.star: Icons.star,
      JewelShape.heart: Icons.favorite,
    };

    return Container(
      decoration: BoxDecoration(
        color: selectedRow == row && selectedCol == col
            ? Colors.amber.withOpacity(0.3)
            : Colors.white,
        border: Border.all(color: Colors.black26),
      ),
      child: Center(
        child: Icon(
          shapeMap[shape]!,
          color: colorMap[color]!,
          size: 32,
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    if (boardLoadFailed) {
      return Scaffold(
        appBar: AppBar(title: Text('Ошибка')),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(Icons.error, color: Colors.red, size: 48),
              SizedBox(height: 16),
              Text('Не удалось загрузить игровое поле.'),
              SizedBox(height: 16),
              ElevatedButton(
                onPressed: () {
                  if (!mounted) return;
                  setState(() {
                    boardLoadFailed = false;
                    isBoardReady = false;
                    _startGame();
                  });
                },
                child: Text('Попробовать снова'),
              ),
            ],
          ),
        ),
      );
    }

    if (!isBoardReady) {
      return Scaffold(
        appBar: AppBar(title: Text('Загрузка...')),
        body: Center(child: CircularProgressIndicator()),
      );
    }

    return Scaffold(
      appBar: AppBar(
        title: Text('Jewel Match'),
      ),
      body: Stack(
        children: [
          Container(
            child: Column(
              children: [Text(' Score: $score | Time: $timeLeft')],
            ),
          ),
          // Game board
          Center(
            child: AspectRatio(
              aspectRatio: 1,
              child: GridView.builder(
                physics: NeverScrollableScrollPhysics(),
                itemCount: gridSize * gridSize,
                gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: gridSize,
                  childAspectRatio: 1,
                ),
                itemBuilder: (context, index) {
                  final row = index ~/ gridSize;
                  final col = index % gridSize;
                  final jewel = boardData[row][col];
                  return GestureDetector(
                    onTap: () => _handleTileTap(row, col),
                    child: _buildJewel(jewel.color, jewel.shape, row, col),
                  );
                },
              ),
            ),
          ),

          // lightning effect overlay, we need this? or not? or what?
          IgnorePointer(
            ignoring: !showLightning,
            child: AnimatedOpacity(
              opacity: showLightning ? 1.0 : 0.0,
              duration: Duration(milliseconds: 100),
              child: Container(
                color: Colors.white.withOpacity(0.6),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
