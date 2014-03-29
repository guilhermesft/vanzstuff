#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import re

print 'Argument List:', str(sys.argv)

horas_completas = sys.argv[1]
if re.match(r'\d+:\d\d', horas_completas):
	horas = re.search(r'\d+:', horas_completas).group()
	minutos = re.search(r':\d\d', horas_completas).group()
	total_minutos = (int(horas[:len(horas)-1]) * 60) + int(minutos[1:])
	print "Resultado: " + str(total_minutos/60.0)
else:
	print 'Formato de hora invalido.\n Assumino que D é um digito:\n Esperado [D]D:DD'
