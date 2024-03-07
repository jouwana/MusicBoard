import 'package:flutter/material.dart';

class CardStatisticsPage extends StatelessWidget {
  const CardStatisticsPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Card Statistics'),
        centerTitle: true,
      ),
      body: const Center(
        child: Text(
          'Card Statistics Page',
          style: TextStyle(fontSize: 24),
        ),
      ),
    );
  }
}
