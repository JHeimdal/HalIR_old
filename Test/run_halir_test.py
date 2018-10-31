#!/usr/bin/env python3

from subprocess import run, PIPE
from halirtest import molecs, halirrunfile


if __name__ == '__main__':
    for m in molecs:
        print("Running: " + m['name'])
        with open(m['name'] + '.inp', 'w') as outf:
            outf.write(halirrunfile.format(**m))
        runlog = run(['mytest', '-i', m['name'] + '.inp'],
                     stdout=PIPE, stderr=PIPE)
        with open(m['name'] + '.out', 'wb') as outf:
            outf.write(runlog.stderr)
        with open(m['name'] + '.xy', 'wb') as outf:
            outf.write(runlog.stdout)
