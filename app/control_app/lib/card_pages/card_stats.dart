import 'package:flutter/material.dart';

class CardStatisticsPage extends StatefulWidget {
  const CardStatisticsPage({Key? key}) : super(key: key);

  @override
  _CardStatisticsPageState createState() => _CardStatisticsPageState();
}

class _CardStatisticsPageState extends State<CardStatisticsPage> {
  late String registeredOn;
  late Duration playTime;
  late int timesScanned;

  @override
  void initState() {
    // Initialize values for the statistics
    registeredOn = DateTime.now().toString(); // Convert DateTime to string
    playTime = Duration(seconds: 3600); // Sample play time: 1 hour
    timesScanned = 10; // Sample times scanned
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    double screenHeight = MediaQuery.of(context).size.height;
    double columnHeight = screenHeight * 0.55; // 50% of screen height

    return Scaffold(
      appBar: AppBar(
        title: const Text('Card Statistics'),
        centerTitle: true,
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: Center(
        child: SizedBox(
          height: columnHeight,
          child: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                Text('Registered On:', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
                Text(registeredOn, style: TextStyle(fontSize: 18)),
                SizedBox(height: 10), // Add some space between the text and the next text (Play Time
                Text('Play Time:', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
                Text(_formatDuration(playTime), style: TextStyle(fontSize: 18)),
                SizedBox(height: 10), // Add some space between the text and the next text (Times Scanned)
                Text('Times Scanned:', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
                Text(timesScanned.toString(), style: TextStyle(fontSize: 18)),
              ],
            ),
          ),
        ),
      ),
    );
  }

  String _formatDuration(Duration duration) {
    int hours = duration.inHours;
    int minutes = duration.inMinutes.remainder(60);
    int seconds = duration.inSeconds.remainder(60);
    return '$hours:${minutes.toString().padLeft(2, '0')}:${seconds.toString().padLeft(2, '0')}';
  }
}
