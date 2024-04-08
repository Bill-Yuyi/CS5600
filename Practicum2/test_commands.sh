#!/bin/bash

# Run command1 and command2 in the background
{
  ./client WRITE local/123.txt remote/hey3.txt   0;
  ./client GET remote/hey3.txt;
} &

# Wait for a moment to ensure the commands have started
sleep 1

# Now, run command3 in the foreground
./client RM remote/hey3.txt;

# Optionally, wait for the background tasks to complete before exiting the script
wait