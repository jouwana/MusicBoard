
import 'package:control_app/custom_button.dart';
import 'package:flutter/material.dart';
import 'card_mapping.dart';

class CardProfilePage extends StatefulWidget {
  const CardProfilePage({Key? key}) : super(key: key);

  @override
  _CardProfilePageState createState() => _CardProfilePageState();
}

class _CardProfilePageState extends State<CardProfilePage> {
  String cardName = 'Card Name Placeholder';

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
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
          centerTitle: true,
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
            Text(
            cardName,
            style: const TextStyle(fontSize: 20),
            ),
              const SizedBox(height: 100),
              CustomButton(
                key: const Key('Edit Card Name'),
                onPressed: () {
                  // Logic to edit card name
                  // Show a text box and update button
                },
                text:'Edit Card Name',
                width: 0.65,
              ),
              const SizedBox(height: 30),
              CustomButton(
                key: const Key('Edit Folder Range'),
                onPressed: () {
                  Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const CardMappingPage(callerID: 'cardProfile'))
                  );
                },
                text:'Edit Folder Range',
                width: 0.65,
              ),
              const SizedBox(height: 30),
              CustomButton(
                key: const Key('Personal Statistics'),
                onPressed: () {
                  Navigator.pushNamed(context, '/cardStatistics');
                },
                text: 'Personal Statistics',
                width: 0.65,
              ),
              const SizedBox(height: 30),
              CustomButton(
                key: const Key('Unregister Card'),
                onPressed: () {
                  // Logic to unregister card
                },
                text:'Unregister Card',
                width: 0.65,
              ),
            ],
          ),
        ),
      ),
    );
  }
}