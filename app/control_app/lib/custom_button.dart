import 'dart:math';

import 'package:flutter/material.dart';

class CustomButton extends StatelessWidget {
  final String text;
  final VoidCallback onPressed;
  final Color color;
  final Color textColor;
  final double width;
  final double height;

  const CustomButton({
    //get a random value to put inside the key value
    required Key key,
    required this.text,
    required this.onPressed,
    this.color = const Color.fromRGBO(0, 100, 255, 0.5),
    this.textColor = Colors.white,
    this.width = 0.0,
    this.height = 50.0,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    double screenWidth = MediaQuery.of(context).size.width;
    double buttonWidth = width;
    if (buttonWidth == 0.0) {
      buttonWidth = screenWidth * 0.75;
    }

    return Container(
      width: buttonWidth,
      height: height,
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
          foregroundColor: textColor, backgroundColor: color,
        ),
        child: Text(text),
      ),
    );
  }
}