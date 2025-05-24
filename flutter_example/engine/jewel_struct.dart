import 'dart:ffi';

final class JewelStruct extends Struct {
  @Float()
  external double posX;

  @Float()
  external double posY;

  @Float()
  external double velX;

  @Float()
  external double velY;

  @Float()
  external double mass;

  @Int32()
  external int shape;

  @Int32()
  external int color;

  @Int32()
  external int isMatched;
}

enum JewelColor {
  red,
  blue,
  green,
  yellow,
  purple,
  orange,
}

enum JewelShape {
  circle,
  square,
  triangle,
  diamond,
  star,
  heart,
}

class JewelData {
  final double x;
  final double y;
  final double velX;
  final double velY;
  final double mass;
  final int shape;
  final int color;
  final bool isMatched;

  JewelData({
    required this.x,
    required this.y,
    required this.velX,
    required this.velY,
    required this.mass,
    required this.shape,
    required this.color,
    required this.isMatched,
  });

  factory JewelData.fromFFI(JewelStruct raw) {
    return JewelData(
      x: raw.posX,
      y: raw.posY,
      velX: raw.velX,
      velY: raw.velY,
      mass: raw.mass,
      shape: raw.shape,
      color: raw.color,
      isMatched: raw.isMatched != 0,
    );
  }
}
