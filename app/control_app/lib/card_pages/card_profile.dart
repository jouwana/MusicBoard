
import 'package:flutter/material.dart';
import 'card_mapping.dart';

class CardProfilePage extends StatelessWidget {
  const CardProfilePage({super.key});

  @override
  Widget build(BuildContext context) {
    return WillPopScope(
      onWillPop: () async {
        // Navigate to the menu page
        Navigator.pushReplacementNamed(context, '/menu');
        // Return false to prevent the default back button behavior
        return Future.value(false);
      },
      child: Scaffold(
        appBar: AppBar(
          title: const Text('Card Profile'),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Text('Card Name'), // Display card name here
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  // Logic to edit card name
                  // Show a text box and update button
                },
                child: const Text('Edit Card Name'),
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const CardMappingPage(callerID: 'cardProfile'))
                  );
                },
                child: const Text('Edit Folder Range'),
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  Navigator.pushNamed(context, '/cardStatistics');
                },
                child: const Text('Personal Statistics'),
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  // Logic to unregister card
                },
                child: const Text('Unregister Card'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}