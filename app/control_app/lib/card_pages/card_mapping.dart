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
              TextFormField(
                controller: _folderController,
                decoration: const InputDecoration(
                  labelText: 'Add Your Folders: 1,2,3',
                ),
                keyboardType: TextInputType.text,
                inputFormatters: [
                  FilteringTextInputFormatter.allow(RegExp(r'[0-9,]')), // Allow only digits and commas
                  _UniqueCommaFormatter(),
                ],
                onChanged: (value) {
                  setState(() {
                    isButtonEnabled = value.isNotEmpty;
                  });
                },
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: isButtonEnabled ? _validateAndSendToESP32 : null,
                child: const Text('Continue'),
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _validateAndSendToESP32() async {
    String input = _folderController.text.trim();
    if (_isValidInput(input)) {
      bool success = await BluetoothHelper.instance.sendCommandToESP32(input);
      if (!success) {
        _showErrorDialog('Failed to send folders to ESP32.');
        return;
      }
      _navigateToNextScreen();
    } else {
      _showErrorDialog('Invalid input format, please abide by the example.');
    }
  }

  bool _isValidInput(String input) {
    // Check if input is empty or starts/ends with comma
    if (input.isEmpty || input.startsWith(',') || input.endsWith(',')) {
      return false;
    }

    // Check if two commas appear consecutively
    if (input.contains(',,') || input.endsWith(',')) {
      return false;
    }

    return true;
  }

  void _showErrorDialog(String message) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text('Error'),
        content: Text(message),
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

  void _navigateToNextScreen() {
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

class _UniqueCommaFormatter extends TextInputFormatter {
  @override
  TextEditingValue formatEditUpdate(
      TextEditingValue oldValue, TextEditingValue newValue) {
    // Prevent two commas in a row without a number in between
    if (newValue.text.endsWith(',,') && !newValue.text.endsWith(',,,')) {
      return oldValue;
    }
    return newValue;
  }
}
