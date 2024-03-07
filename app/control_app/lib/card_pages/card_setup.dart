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
      ),
    );
  }
}