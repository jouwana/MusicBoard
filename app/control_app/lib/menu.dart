import 'package:flutter/material.dart';
import 'custom_button.dart';

class MenuPage extends StatelessWidget {
  const MenuPage({super.key});

  @override
  Widget build(BuildContext context) {

    double screenHeight = MediaQuery.of(context).size.height;
    double columnHeight = screenHeight * 0.7; // 70% of screen height

    return Scaffold(
      appBar: AppBar(
        title: const Text('Menu'),
        centerTitle: true,
        backgroundColor: const Color.fromRGBO(0, 100, 255, 0.5),
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
                ),
                CustomButton(
                  key: const Key('cardProfileButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/cardProfile');
                  },
                  text: 'Card Profile',
                ),
                CustomButton(
                  key: const Key('addFolderButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/addFolder');
                  },
                  text: 'Add Folder',
                ),
                CustomButton(
                  key: const Key('folderEditButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/folderEdit');
                  },
                  text: 'Edit Folder',
                ),
                CustomButton(
                  key: const Key('globalStatisticsButton'),
                  onPressed: () {
                    Navigator.pushNamed(context, '/globalStatistics');
                  },
                  text: 'Global Statistics',
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}