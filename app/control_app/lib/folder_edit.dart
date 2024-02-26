import 'package:flutter/material.dart';

import 'custom_button.dart';

class FolderEditPage extends StatelessWidget {
  const FolderEditPage({super.key});

  @override
  Widget build(BuildContext context) {
    double textWidth = MediaQuery.of(context).size.width * 0.65;
    return Scaffold(
      appBar: AppBar(
        title: const Text('Folder Edit'),
      ),
      body: Center(
        child: SizedBox(
          width: textWidth,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Implement UI to edit folder (e.g., dropdown for folder number and text field for new size)
              // Include a button to proceed
              const TextField(
                decoration: InputDecoration(
                  labelText: 'Folder Number',
                ),
                keyboardType: TextInputType.number,
              ),
              const SizedBox(height: 20),
              const TextField(
                decoration: InputDecoration(
                  labelText: 'New Folder Size',
                ),
                keyboardType: TextInputType.number,
              ),
              const SizedBox(height: 20),
              CustomButton(
                key: const Key('editFolderSubmitButton'),
                text:'Submit',
                onPressed:  () {
                  Navigator.pushNamed(context, '/menu');
                },
              ),
            ],
          ),
        ),
      ),
    );
  }
}

