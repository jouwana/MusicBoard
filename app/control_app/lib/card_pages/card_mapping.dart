import 'package:flutter/material.dart';


class CardMappingPage extends StatelessWidget {
  final String callerID;
  const CardMappingPage({super.key, required this.callerID});

  @override
  Widget build(BuildContext context) {
    double ButtonWidth = MediaQuery.of(context).size.width * 0.65;
    return Scaffold(
      appBar: AppBar(
        title: const Text('Card Mapping'),
      ),
      body: Center(
        child:SizedBox(
          width: ButtonWidth,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Implement UI to input folder number range for the card
              // Include continue button
              const Text('Folder Range'),
              const TextField(
                decoration: InputDecoration(
                  labelText: 'Start Folder Number',
                ),
                keyboardType: TextInputType.number,
              ),
              const SizedBox(height: 20),
              const TextField(
                decoration: InputDecoration(
                  labelText: 'End Folder Number',
                ),
                keyboardType: TextInputType.number,
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  if(callerID == 'addCard') {
                    Navigator.pushNamed(context, '/menu');
                  } else if(callerID == 'cardProfile') {
                    Navigator.pushNamed(context, '/cardProfile');
                  }
                },
                child: const Text('Continue'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}