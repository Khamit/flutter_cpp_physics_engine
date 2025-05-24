import 'package:flutter/material.dart';

class NoMatchesDialog extends StatelessWidget {
  const NoMatchesDialog({super.key});

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text('No Matches'),
      content: Text('No possible matches on the board!'),
      actions: [
        TextButton(
          onPressed: () => Navigator.pop(context),
          child: Text('OK'),
        ),
      ],
    );
  }
}
