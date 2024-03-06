import 'dart:async';
import 'dart:convert';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class BluetoothHelper {
  static final BluetoothHelper _instance = BluetoothHelper._internal();

  factory BluetoothHelper() {
    return _instance;
  }

  BluetoothHelper._internal();

  BluetoothConnection? _connection;

  static BluetoothHelper get instance => _instance;

  bool get isConnected => _connection?.isConnected ?? false;

  Future<void> _initConnection() async {
    if (_connection != null && isConnected) {
      print("Connection already initialized");
      return;
    }

    try {
      final devices = await FlutterBluetoothSerial.instance.getBondedDevices();
      for (BluetoothDevice device in devices) {
        if (device.name == 'ESP32') { // Replace 'ESP32' with your ESP32 device name
          _connection = await BluetoothConnection.toAddress(device.address);
          break;
        }
      }
    } catch (e) {
      print('Failed to connect to ESP32: $e');
    }
  }


  void setConnection(BluetoothConnection connection) {
    _connection = connection;
  }

  Future<void> resetConnection() async {
    _connection?.close();
    _initConnection();
  }

  Future<bool> sendCommandToESP32(String command) async {
    await _initConnection();

    if (_connection == null) {
      print('Bluetooth connection is not available.');
      return false;
    }

    try {
      _connection!.output.add(utf8.encode(command)); // Send command as UTF-8 bytes
      await _connection!.output.allSent;
      return true; // Command sent successfully
    } catch (e) {
      print('Failed to send command to ESP32: $e');
      return false; // Command sending failed
    }
  }


  Future<String?> awaitResponse({String neededCommand = "", String failCommand = ""}) async {
    const int maxAttempts = 4; // Maximum number of attempts
    int attempt = 0;

    while (attempt < maxAttempts) {
      attempt++;

      // Initialize connection if not already connected
      if (!isConnected && [1,2].contains(attempt)) {
        await _initConnection();
        if(isConnected && neededCommand.isNotEmpty){
          sendCommandToESP32(neededCommand);
        }
      }
      else if(!isConnected){
          if(failCommand.isNotEmpty) {
            sendCommandToESP32(failCommand);
          }
        return 'Failed to connect to ESP32.';
      }

      // Check if connected after initialization attempt
      if (isConnected) {
        try {
          final data = await _connection!.input!.first.timeout(Duration(seconds: 15)); // Add a timeout for the response
          final dataString = utf8.decode(data);
          final response = dataString.trim();
          return response; // Return response if successful
        } catch (e) {
          // Handle specific timeout exception or general exceptions
          if (e is TimeoutException) {
            print('Response timeout, retrying... Attempt $attempt/$maxAttempts');
          } else {
            print('Failed to receive response from ESP32: $e, retrying... Attempt $attempt/$maxAttempts');
          }
        }
      } else {
        print('Failed to connect, retrying... Attempt $attempt/$maxAttempts');
        // Optionally, wait a bit before retrying to give some time for the device to become ready
        await Future.delayed(Duration(seconds: 2));
      }
    }

    if(failCommand.isNotEmpty) {
      await sendCommandToESP32(failCommand);
    }
    resetConnection();
    print('Exceeded maximum retry attempts.');
    return 'Exceeded maximum retry attempts.'; // Return null if all attempts fail
  }

}
