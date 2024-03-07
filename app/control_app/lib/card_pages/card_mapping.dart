import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import '../Helpers/bluetooth_manager.dart';

class CardMappingPage extends StatefulWidget {
  final String callerID;
  const CardMappingPage({Key? key, required this.callerID}) : super(key: key);

  @override
  _CardMappingPageState createState() => _CardMappingPageState();
}

class _CardMappingPageState extends State<CardMappingPage> {
  final TextEditingController _folderController = TextEditingController();
  List<String> folders = [];
  bool isButtonEnabled = false;

  @override
  Widget build(BuildContext context) {
    double buttonWidth = MediaQuery.of(context).size.width * 0.65;
    return Scaffold(
      appBar: AppBar(
        title: const Text('Card Mapping'),
        centerTitle: true,
      ),
      body: Center(
        child: SizedBox(
          width: buttonWidth,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Implement UI to input folder number range for the card
              // Include continue button
              const Text(
                'Please enter the folder numbers for this card \n\n',
                textAlign: TextAlign.center,
              ),
              TextField(
                controller: _folderController,
                decoration: const InputDecoration(
                  labelText: 'Add a Folder Number',
                ),
                keyboardType: TextInputType.number,
                onSubmitted: (value) {
                  _validateAndAddFolder();
                },
              ),
              const SizedBox(height: 20),
              Text(
                folders.isEmpty ? '' : 'Folders: ${folders.join(', ')}',
                style: const TextStyle(fontSize: 18),
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: folders.isNotEmpty ? _sendFoldersToESP32 : null,
                child: const Text('Continue'),
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _validateAndAddFolder() {
    String input = _folderController.text.trim();
    if (input.isNotEmpty && int.tryParse(input) != null) {
      folders.add(input);
      _folderController.clear();
      setState(() {
        isButtonEnabled = folders.isNotEmpty; // Enable/disable button based on folders list
      });
    } else {
      showDialog(
        context: context,
        builder: (context) => AlertDialog(
          title: Text('Invalid Input'),
          content: Text('Please enter a valid folder number.'),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pop();
              },
              child: Text('OK'),
            ),
          ],
        ),
      );
    }
  }

  void _sendFoldersToESP32() async {
    // Send folders to ESP32
      bool success = await BluetoothHelper.instance.sendCommandToESP32(folders.join(','));
      if (!success) {
        showDialog(
          context: context,
          builder: (context) => AlertDialog(
            title: Text('Error'),
            content: Text('Failed to send folders to ESP32.'),
            actions: <Widget>[
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: Text('OK'),
              ),
            ],
          ),
        );
        return;

    }

    // Navigate to the desired screen
    if (widget.callerID == 'addCard') {
      Navigator.pushNamed(context, '/menu');
    } else if (widget.callerID == 'cardProfile') {
      Navigator.pushNamed(context, '/cardProfile');
    }
  }

  @override
  void dispose() {
    _folderController.dispose();
    super.dispose();
  }
}
