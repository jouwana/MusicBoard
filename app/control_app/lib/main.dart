import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'card_pages/add_card.dart';
import 'folder_edit.dart';
import 'add_folder.dart';
import 'card_pages/card_stats.dart';
import 'global_stats.dart';
import 'card_pages/card_setup.dart';
import 'card_pages/card_profile.dart';
import 'menu.dart';
import 'Helpers/bluetooth_manager.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized(); // Ensure Flutter is initialized
  runApp(const MyApp());
  initializeBluetooth();
}

void initializeBluetooth() async {
  BluetoothHelper(); // Create an instance of BluetoothHelper
}

class MyApp extends StatelessWidget {
  static const platform = MethodChannel('com.example.app/lifecycle');
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {

    platform.setMethodCallHandler((call) async {
      if (call.method == 'onDestroy') {
        BluetoothHelper.instance.sendCommandToESP32('exit_scan');
        print('Received onStop event from Android.');
      }
    });

    return MaterialApp(
      title: 'Your App Title',
      initialRoute: '/adminLogin',
      routes: {
        '/adminLogin': (context) => const AdminLoginPage(),
        '/menu': (context) => const MenuPage(),
        '/addCard': (context) => const ScanCardPage(),
        '/cardProfile': (context) => const CardProfilePage(),
        '/cardSetup': (context) => const CardSetupPage(),
        '/globalStatistics': (context) => const GlobalStatisticsPage(),
        '/cardStatistics': (context) => const CardStatisticsPage(),
        '/addFolder': (context) => const AddFolderPage(),
        '/folderEdit': (context) => const FolderEditPage(),
      },
    );
  }
}

class AdminLoginPage extends StatefulWidget {
  const AdminLoginPage({super.key});

  @override
  _AdminLoginPageState createState() => _AdminLoginPageState();
}

class _AdminLoginPageState extends State<AdminLoginPage> {
  final TextEditingController _securityCodeController = TextEditingController();
  String _errorMessage = '';
  bool _hasError = false;


  @override
  Widget build(BuildContext context) {
    double screenWidth = MediaQuery.of(context).size.width;
    return Scaffold(
      appBar: AppBar(
        centerTitle: true,
        title: const Text('Admin Login'),
        //remove back button
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        automaticallyImplyLeading: false,
      ),
      body: Center(
        child: SizedBox(
          width: screenWidth*0.5,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              TextField(
                controller: _securityCodeController,
                decoration: InputDecoration(
                  labelText: 'Security Code',
                  errorText: _hasError ? _errorMessage : null,
                  errorBorder: _hasError ? const OutlineInputBorder(borderSide: BorderSide(color: Colors.red)) : InputBorder.none,
                  focusedErrorBorder: const OutlineInputBorder(borderSide: BorderSide(color: Colors.red)),
                ),
                keyboardType: TextInputType.number,
                //hide the text
                obscureText: true,
                onChanged: (_) {
                  setState(() {
                    _hasError = false; // Reset error state when text changes
                  });
                },
                onSubmitted: (value) {
                  _login();
                },
                onTap: () {
                  if (_hasError) {
                    _securityCodeController.clear();
                    setState(() {
                      _hasError = false; // Reset error state
                    });
                  }
                },
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: _login,
                child: const Text('Continue'),
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _login() {
    if (_securityCodeController.text == '6825') {
      // Clear text field
      _securityCodeController.clear();
      // Navigate to menu page if security code is correct
      Navigator.pushReplacementNamed(context, '/menu');
    } else {
      // Show error message if security code is incorrect
      setState(() {
        if(_securityCodeController.text.isEmpty) {
          _errorMessage = 'Security code is required';
        } else {
          _errorMessage = 'Incorrect security code';
        }
        _hasError = true; // Set error state to true
      });
    }
  }

  @override
  void dispose() {
    _securityCodeController.dispose();
    super.dispose();
  }
}






