#!/usr/bin/env python3

import csv
import json
import time

from pathlib import Path

RAW_DATA_FOLDER = 'data/'
PROCESSED_DATA_FOLDER = 'processed-data/'

def main():
  data = []
  for run_path in Path(RAW_DATA_FOLDER).glob('*.json'):
    with run_path.open('r') as f:
      run = json.load(f)

    config = run['config']
    metadata = run['metadata']
    results = run['results']
    for dc in results['diversity_counts']:
      datum = {**config, **metadata, **dc}
      data.append(datum)

  now = int(time.time())
  with Path(f'{PROCESSED_DATA_FOLDER}/collated-result-{now}.csv').open('w') as f:
    writer = csv.DictWriter(f, fieldnames=[
      'N',
      'graph_name',
      'birth_mutation_rate',
      'independent_mutation_rate',
      'num_simulations',
      'num_steps',
      'start_time_s',
      'end_time_s',
      'diversity',
      'count',
    ])
    writer.writeheader()
    for row in data:
      writer.writerow(row)



if __name__ == '__main__':
  main()

