import 'package:flutter/material.dart';

class GlobalStatisticsPage extends StatelessWidget {
  const GlobalStatisticsPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Global Statistics'),
        centerTitle: true,
      ),
      body: const Center(
        child: Text(
          'Global Statistics Page',
          style: TextStyle(fontSize: 24),
        ),
      ),
    );
  }
}