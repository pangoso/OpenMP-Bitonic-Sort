import subprocess
from typing import Optional
import re

import typer
import pandas as pd

from data_generator import generate_data


def tester(start_the_power_of_two: Optional[int] = typer.Argument(4),
           end_the_power_of_two: Optional[int] = typer.Argument(15),
           test_check: Optional[bool] = typer.Option(True)):
    test_data = {
        'length': [],
        'seq_time': [],
        'par_time': [],
        'seq_works': [],
        'par_works': []
    }
    pattern = re.compile('\d+\.\d+')
    for power_of_two in range(start_the_power_of_two, end_the_power_of_two):
        list_length = 2**power_of_two
        print(list_length)
        max_number = 10**len(str(list_length))
        for i in range(5):
            print(i)
            dataset_sorted = sorted(generate_data(power_of_two, max_number))
            subprocess_args = {'stdout': subprocess.PIPE, 'shell': True}
            with subprocess.Popen(['./seq.out'], **subprocess_args) as proc:
                seq = proc.stdout.read().decode(encoding="utf-8").split('\n')
            with subprocess.Popen(['./par.out'], **subprocess_args) as proc:
                par = proc.stdout.read().decode(encoding="utf-8").split('\n')
            par_sorted = par[1].strip()
            seq_sorted = seq[1].strip()
            test_data['length'].append(list_length)
            test_data['seq_time'].append(float(pattern.findall(seq[2])[0]))
            test_data['par_time'].append(float(pattern.findall(par[2])[0]))
            formated_set = str(dataset_sorted)[1:-1].replace(', ', ' ').strip()
            test_data['seq_works'].append(seq_sorted == formated_set)
            test_data['par_works'].append(par_sorted == formated_set)
            # print(f"{par_sorted}\n{formated_set}")
    if test_check:
        pd.DataFrame(test_data).to_csv('test_results.csv', index=False)
    else:
        pd.DataFrame(test_data).loc[:, ['length', 'seq_time', 'par_time']].groupby(
                ['length']).mean().to_csv('test_results.csv')


if __name__ == '__main__':
    typer.run(tester)    typer.run(tester)
