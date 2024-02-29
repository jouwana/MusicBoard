import 'dart:convert';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class ScanCardPage extends StatefulWidget {
  const ScanCardPage({super.key});

  @override
  _ScanCardPageState createState() => _ScanCardPageState();
}

class _ScanCardPageState extends State<ScanCardPage> {
  late BluetoothConnection connection; // Define BluetoothConnection variable
  bool isConnected = false;
  bool isScanning = false;

  @override
  void initState() {
    super.initState();
    _connectToESP32();
  }

  Future<void> _connectToESP32() async {
    List<BluetoothDevice> devices = await FlutterBluetoothSerial.instance.getBondedDevices();
    for (BluetoothDevice device in devices) {
      if (device.name == 'ESP32') { // Replace 'ESP32' with your ESP32 device name
        try {
          connection = await BluetoothConnection.toAddress(device.address);
          setState(() {
            isConnected = true; // Update connection status
          });
        } catch (e) {
          print('Failed to connect to ESP32: $e');
        }
        break;
      }
    }
  }

  void _sendCommandToESP32(String command) async {
    if (isConnected) {
      try {
        connection.output.add(utf8.encode(command)); // Send command as UTF-8 bytes
        await connection.output.allSent;
        setState(() {
          isScanning = true; // Toggle isScanning value
        });
        _awaitResponse(); // Wait for response from ESP32
      } catch (e) {
        print('Failed to send command to ESP32: $e');
      }
    } else {
      print('Not connected to ESP32');
    }
  }

  void _awaitResponse() {
    connection.input?.listen((Uint8List data) {
      String dataString = utf8.decode(data);
      String response = dataString.trim();
      if (response == 'new') {
        // If response is 'new', navigate to CardSetupPage
        setState(() {
          isScanning = false; // Toggle isScanning value
        });
        Navigator.pushNamed(context, '/cardSetup');
      }
    });
  }

  @override
  void dispose() {
    if(connection.isConnected) {
      _sendCommandToESP32(
          'exit_scan'); // Send command to ESP32 to exit scan mode
      connection.finish(); // Finish the connection
      super.dispose();
    }
  }

  @override
  Widget build(BuildContext context) {
    return WillPopScope(
        onWillPop: () async {
          _sendCommandToESP32('exit_scan'); // Send command to ESP32 to exit scan mode
          connection.finish(); // Finish the connection
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
                    child: Text(isScanning ? 'Scanning...' : 'Scan Card'),
                ),
              ],
            ),
          ),
        ),
      )
    );
  }
}