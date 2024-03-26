import 'package:control_app/custom_button.dart';
import 'package:flutter/material.dart';
import 'card_mapping.dart';
import '../Helpers/bluetooth_manager.dart';

class CardSetupPage extends StatelessWidget {
  const CardSetupPage({super.key});

  @override
  void dispose() {
  }

  @override
  Widget build(BuildContext context) {
    return WillPopScope(
      onWillPop:() async {
        await BluetoothHelper.instance.sendCommandToESP32('exit_scan');
        BluetoothHelper.instance.resetConnection();
        return true;
      },
      child: Scaffold(
      appBar: AppBar(
        title: const Text('Card Setup'),
        centerTitle: true,
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Text('Do you want to register the card?',style: TextStyle(fontSize: 18),), // Add a question here
            const SizedBox(height: 50),
            CustomButton(
              key: const Key('Register Card'),
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) => const CardMappingPage(callerID: 'addCard'),
                  ),
                );
              },
              text:'Register Card',
              width: 0.65,
            ),
          ],
        ),
      ),
      ),
    );
  }
}