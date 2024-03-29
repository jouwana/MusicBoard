
import 'package:flutter/material.dart';

class CustomButton extends StatelessWidget {
  final String text;
  final VoidCallback? onPressed;
  final Color color;
  final Color textColor;
  final double width;
  final double height;
  final double fontSize;

  const CustomButton({
    //get a random value to put inside the key value
    required Key key,
    required this.text,
    required this.onPressed,
    this.color = const Color.fromRGBO(0, 100, 255, 0.5),
    this.textColor = Colors.white,
    this.width = 0.0,
    this.height = 50.0,
    this.fontSize = 20,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    double screenWidth = MediaQuery.of(context).size.width;
    double buttonWidth = width;
    if (buttonWidth == 0.0) {
      buttonWidth = screenWidth * 0.75;
    }
    else {
      buttonWidth = screenWidth * width;
    }

    return SizedBox(
      width: buttonWidth,
      height: height,
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
            elevation: 5,
        ),
        child: Text(text,
          style: TextStyle(fontSize: fontSize)),
      ),
    );
  }
}