#!/bin/bash

client_config_path=$1
N=$2

if [[ -z "$N" || ! "$N" =~ ^[0-9]+$ ]]; then
  echo "Usage: $0 <number_of_clients>"
  exit 1
fi

for ((i=0; i<N; i++)); do
  arg=$(head /dev/urandom | tr -dc '0-9' | head -c 15)
  echo "Client $i running..."
  ./bin/client "$client_config_path" "$arg" &
done

echo "Waiting for all clients to exit"
wait
