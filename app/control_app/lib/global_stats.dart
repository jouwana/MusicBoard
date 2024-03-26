import 'package:flutter/material.dart';

class GlobalStatisticsPage extends StatefulWidget {
  const GlobalStatisticsPage({Key? key}) : super(key: key);

  @override
  _GlobalStatisticsPageState createState() => _GlobalStatisticsPageState();
}

class _GlobalStatisticsPageState extends State<GlobalStatisticsPage> {
  late int numberOfCards;
  late Duration maxPlayTime;
  late String mostPlayedCard;
  late String mostScannedCard;
  late String mostMappedFolder;

  @override
  void initState() {
    // Initialize values for the statistics
    numberOfCards = 100; // Sample number of cards
    maxPlayTime = Duration(seconds: 3600); // Sample max play time: 1 hour
    mostPlayedCard = 'Ace of Spades'; // Sample most played card
    mostScannedCard = 'King of Hearts'; // Sample most scanned card
    mostMappedFolder = 'Folder A'; // Sample most mapped folder
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    double screenHeight = MediaQuery.of(context).size.height;
    double columnHeight = screenHeight * 0.6; // 50% of screen height

    return Scaffold(
      appBar: AppBar(
        title: const Text('Global Statistics'),
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
            Text('Number of Cards:', style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold)),
            Text(numberOfCards.toString(), style: TextStyle(fontSize: 18)),
            SizedBox(height: 10),
            Text('Max Play Time:', style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold)),
            Text(_formatDuration(maxPlayTime), style: TextStyle(fontSize: 18)),
            SizedBox(height: 10),
            Text('Most Played Card:', style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold)),
            Text(mostPlayedCard, style: TextStyle(fontSize: 18)),
            SizedBox(height: 10),
            Text('Most Scanned Card:', style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold)),
            Text(mostScannedCard, style: TextStyle(fontSize: 18)),
            SizedBox(height: 10),
            Text('Most Mapped Folder:', style: TextStyle(fontSize: 20,fontWeight: FontWeight.bold)),
            Text(mostMappedFolder, style: TextStyle(fontSize: 18)),
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
