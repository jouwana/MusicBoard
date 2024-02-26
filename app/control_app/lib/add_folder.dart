import 'package:flutter/material.dart';
import '/custom_button.dart';

class AddFolderPage extends StatelessWidget {
  const AddFolderPage({super.key});

  @override
  Widget build(BuildContext context) {
    double textWidth = MediaQuery.of(context).size.width * 0.65;
    return Scaffold(
      appBar: AppBar(
        title: const Text('Add Folder'),
      ),
      body: Center(
        child: SizedBox(
          width: textWidth,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Implement UI to add folder (e.g., text field for folder number and size)
              // Include a button to proceed]
              const Text('Folder Number'),
              const TextField(
                decoration: InputDecoration(
                  labelText: 'Folder Number',
                ),
                keyboardType: TextInputType.number,
              ),
              const SizedBox(height: 20),
              const Text('Folder Size'),
              const TextField(
                decoration: InputDecoration(
                  labelText: 'Folder Size',
                ),
                keyboardType: TextInputType.number,
              ),
              const SizedBox(height: 20),
              CustomButton(
                key: const Key('addFolderSubmitButton'),
                text:'Create Folder',
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

