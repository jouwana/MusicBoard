import 'package:flutter/material.dart';
import 'card_mapping.dart';

class CardSetupPage extends StatelessWidget {
  const CardSetupPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Card Setup'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Text('Do you want to register the card?'), // Add a question here
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) => const CardMappingPage(callerID: 'addCard'),
                  ),
                );
              },

              child: const Text('Register Card'),
            ),
          ],
        ),
      ),
    );
  }
}