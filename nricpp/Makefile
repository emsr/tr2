
all: test_ode test_integration test_matrix test_nricpp

test_ode: \
    test_ode.cpp \
    ode.tcc
	$$HOME/bin/bin/g++ -o test_ode test_ode.cpp

test_integration: \
    test_integration.cpp \
    integration.h \
    integration.tcc \
    gauss_quad.tcc
	$$HOME/bin/bin/g++ -o test_integration test_integration.cpp

test_matrix: \
    test_matrix.cpp \
    matrix.h \
    matrix_util.h \
    matrix_cholesky_decomp.tcc \
    matrix_lu_decomp.tcc \
    matrix_qr_decomp.tcc \
    matrix_sv_decomp.tcc

test_nricpp: \
    test_nricpp.cpp \
    nricpp.h \
    integration.h \
    integration.tcc \
    gauss_quad.tcc \
    cmath_variable_template
