import 'package:flutter/material.dart';
<<<<<<< Updated upstream


class AddCardPage extends StatelessWidget {
  const AddCardPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Add Card'),
      ),
      body: Center(
        child: SizedBox(
          width: MediaQuery.of(context).size.width * 0.75,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Text(
                'To add a new card, please scan the card using the reader on the board',
                textAlign: TextAlign.center,
                style: TextStyle(fontSize: 18),
              ),
              const SizedBox(height: 40),
              ElevatedButton(
                onPressed: () {
                  Navigator.pushNamed(context, '/cardSetup');
                },
                child: const Text('Scan Card'),
              ),
            ],
=======
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import '../Helpers/bluetooth_manager.dart';

class ScanCardPage extends StatefulWidget {
  const ScanCardPage({super.key});

  @override
  _ScanCardPageState createState() => _ScanCardPageState();
}

class _ScanCardPageState extends State<ScanCardPage> {
  late BluetoothHelper _bluetoothManager; // Define BluetoothManager variable
  bool isConnecting = false;
  bool isScanning = false;

  @override
  void initState() {
    super.initState();
    _bluetoothManager = BluetoothHelper.instance; // Get BluetoothManager instance
  }

  void _sendCommandToESP32(String command) async {
    setState(() {
      isConnecting = true; // Toggle isConnecting value
    });
    bool response = await _bluetoothManager.sendCommandToESP32(command); // Send command to ESP32
    if(response) {
      setState(() {
        isConnecting = false; // Toggle isConnecting value
      });
      if(command == 'scan') {
        _awaitResponse(); // Await response from ESP32
        setState(() {
          isScanning = true; // Toggle isScanning value
        });
      }
      else if(command == 'exit_scan') {
        setState(() {
          isScanning = false; // Toggle isScanning value
        });
      }
    }
    else{
      setState(() {
        isConnecting = false; // Toggle isConnecting value
        isScanning = false; // Toggle isScanning value
      });
      // Show an error message if command sending failed
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('Failed to send command to ESP32, please check connection and try again.'),
          duration: Duration(seconds: 3),
        ),
      );
    }
  }

  void _awaitResponse() async {
     //add a limit of 1 minute to await response
      String? response = await _bluetoothManager.awaitResponse(neededCommand: 'scan', failCommand: 'exit_scan'); // Await response from ESP32
      if (response == 'new') {
        // If response is 'new', navigate to CardSetupPage
        setState(() {
          isScanning = false; // Toggle isScanning value
        });
        Navigator.pushNamed(context, '/cardSetup');
      }
      else if(response == 'connection_error') {
        setState(() {
          isScanning = false; // Toggle isScanning value
          isConnecting = false; // Toggle isConnecting value
        });
        // If response is 'connection_error', show an error message
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(
            content: Text('Failed to connect to ESP32, please check your connection and try again.'),
            duration: Duration(seconds: 3),
          ),
        );
      }
      else{
        setState(() {
          isScanning = false; // Toggle isScanning value
          isConnecting = false; // Toggle isConnecting value
        });
        // If response is null, show an error message
        _sendCommandToESP32('exit_scan'); // Send command to ESP32 to exit scan mode
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text( 'Error: $response, please try again.'),
            duration: const Duration(seconds: 3),
          ),
        );
      }
  }

  @override
  void dispose() async {
    _bluetoothManager.resetConnection(); // Reset Bluetooth connection
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return WillPopScope(
        onWillPop: () async {
          _sendCommandToESP32('exit_scan'); // Send command to ESP32 to exit scan mode
          return true; // Allow back navigation
      },
      child: Scaffold(
        appBar: AppBar(
          title: const Text('Add Card'),
        ),
        body: Center(
          child: SizedBox(
            width: MediaQuery.of(context).size.width * 0.75,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text(
                  'To add a new card, please scan the card using the reader on the board',
                  textAlign: TextAlign.center,
                  style: TextStyle(fontSize: 18),
                ),
                const SizedBox(height: 40),
                ElevatedButton(
                  onPressed: isScanning
                      ? null
                      : () {
                        _sendCommandToESP32('scan'); // Send command to ESP32 to start scanning
                      },
                    child: Text(isScanning ? 'Scanning...'
                        : isConnecting? 'Connecting...': 'Scan Card'),
                ),
              ],
            ),
>>>>>>> Stashed changes
          ),
        ),
      ),
    );
  }
}