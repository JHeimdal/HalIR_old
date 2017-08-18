#!/usr/bin/env python3.6

import sys 
from subprocess import check_output as call
from spc import SPC
from halirtest import molecs,halirrunfile


if __name__=='__main__':
	for m in molecs:
		print(halirrunfile.format(**m))
		
