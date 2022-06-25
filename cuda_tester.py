import subprocess

if __name__ == '__main__':
    subprocess_args = {'stdout': subprocess.PIPE, 'shell': True}
    with subprocess.Popen(['./test.out'], **subprocess_args) as proc:
        output = proc.stdout.read().decode(encoding="utf-8").split(' ')
        seq, result = [int(x) for x in output[:-3]], ' '.join(output[-3:])
    print(len(seq))
    print(seq == sorted(seq), end='')
    print(result)
    