import logging
import random
from typing import Optional

import typer


def generate_data(the_power_of_two: int, max_number: int):
    list_length = 2**the_power_of_two
    result = [random.randrange(1, max_number, 1) for i in range(list_length)]
    with open('dane.txt', 'wt') as file:
        file.write(f'{list_length}\n')
        file.write(str(result)[1:-1].replace(', ', ' '))
    return result


def generate(the_power_of_two: Optional[int] = typer.Argument(4), max_number: Optional[int] = typer.Argument(100)):
    logging.basicConfig(filename="newfile.log",
                        format='%(asctime)s %(message)s', filemode='w')
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    logger.info(f'Generating {2**the_power_of_two} numbers to dane.txt')
    generate_data(the_power_of_two, max_number)


if __name__ == '__main__':
    typer.run(generate)