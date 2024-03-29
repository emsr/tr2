#! /bin/bash

chmod -R +rw,-x *.tcc *.cpp *.h *.tpp *.cc *.pro bitops integer_seq logical_const make_array \
 net numeric numeric_limits observer_ptr ostream_joiner randint_n4316 random search shared_mutex value_ptr variant \
 urectum2 uri UPM-TIREM\ Timeline\ and\ Cost.odt *.c *.in *.out string_literal string_view *.C *.txt patch_* CL_* 3.5*

chmod -R +rw,-x ext_distribution/testsuite *.cc

chmod +rwx any binary_io codecvt dynamic_bitset dynarray \
 ext_distribution filesystem ioformat \
 literals optional ratio SD-6 specfun testinc

chmod +rwx .git
chmod -R +rw .git/*
chmod +rwx .git/hooks .git/info .git/logs .git/objects .git/refs
chmod +wx .git/hooks/* .git/info/* .git/logs/* .git/objects/* .git/refs/*
