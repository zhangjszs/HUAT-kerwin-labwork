#!/bin/bash

mkdir -p ~/userdata || { echo "Failed to create userdata directory"; exit 1; }

cd ~/userdata || { echo "Failed to change directory"; exit 1; }

for i in {1..5}
do
  mkdir -p "user$i" || { echo "Failed to create user$i directory"; exit 1; }
  chmod 754 "user$i" || { echo "Failed to set permissions for user$i"; exit 1; }
done

echo "Directories created and permissions set."