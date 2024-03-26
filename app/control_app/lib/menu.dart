import 'package:flutter/material.dart';
import 'custom_button.dart';

class MenuPage extends StatelessWidget {
  const MenuPage({super.key});

  @override
  Widget build(BuildContext context) {

    double screenHeight = MediaQuery.of(context).size.height;
    double columnHeight = screenHeight * 0.5; // 70% of screen height

    return Scaffold(
      appBar: AppBar(
        title: const Text('Menu'),
        centerTitle: true,
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        leading: GestureDetector(
          onTap: () {
            // Logic to logout
            // Navigate to admin login page
            Navigator.pushNamed(context, '/adminLogin');
          },
          child: const Row( // Wrap the text with a container
            children: [
              Padding(padding: EdgeInsets.only(left: 15)),
              Icon(Icons.logout),
            ],
          ),
        ),
        actions: const [
          // Add any additional actions here
        ],
        //remove back button
        automaticallyImplyLeading: false,
      ),
      body: Center(
        child: SizedBox(
          height: columnHeight,
          child: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                CustomButton(
                  key: const Key('addCardButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/addCard');
                  },
                  text: 'Add Card',
                  color: Theme.of(context).colorScheme.secondary,
                ),
                CustomButton(
                  key: const Key('cardProfileButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/cardProfile');
                  },
                  text: 'Card Profile',
                  color: Theme.of(context).colorScheme.secondary,
                ),
                CustomButton(
                  key: const Key('globalStatisticsButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/globalStatistics');
                  },
                  text: 'Global Statistics',
                  color: Theme.of(context).colorScheme.secondary,
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}