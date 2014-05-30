
all: $(TEST_BINS)

TEST_SRCS = \
  test_arcsine_distribution.cpp \
  test_beta_distribution.cpp \
  test_gamma_distribution.cpp \
  test_hoyt_distribution.cpp \
  test_k_distribution2.cpp \
  test_k_distribution.cpp \
  test_nakagami_distribution.cpp \
  test_nakagami_fading_model.cpp \
  test_pareto_distribution.cpp \
  test_rice_distribution_multi.cpp \
  test_rice_distribution.cpp \
  test_rice_fading_model.cpp \
  test_logistic_distribution.cpp

TEST_BINS = \
  test_arcsine_distribution \
  test_beta_distribution \
  test_gamma_distribution \
  test_hoyt_distribution \
  test_k_distribution2 \
  test_k_distribution \
  test_nakagami_distribution \
  test_nakagami_fading_model \
  test_pareto_distribution \
  test_rice_distribution_multi \
  test_rice_distribution \
  test_rice_fading_model \
  test_logistic_distribution


check_fading_model: check_fading_model.cpp libFadingModel.so
	/home/ed/bin/bin/g++ -std=c++11 -o check_fading_model check_fading_model.cpp -L. -lFadingModel

libFadingModel.so: RiceFadingModel.o NakagamiFadingModel.o
	/home/ed/bin/bin/g++ -std=c++11 -fPIC -shared -o libFadingModel.so RiceFadingModel.o NakagamiFadingModel.o

RiceFadingModel.o: RiceFadingModel.h RiceFadingModel.cpp
	/home/ed/bin/bin/g++ -std=c++11 -fPIC -c -o RiceFadingModel.o RiceFadingModel.cpp

NakagamiFadingModel.o: NakagamiFadingModel.h NakagamiFadingModel.cpp
	/home/ed/bin/bin/g++ -std=c++11 -fPIC -c -o NakagamiFadingModel.o NakagamiFadingModel.cpp

fading_model_docs: RiceFadingModel.h NakagamiFadingModel.h
	rm -rf html_model latex_model
	doxygen Doxyfile.model
	cd latex_model; make pdf && cp refman.pdf ../fading_model.pdf


test_bins: $(TEST_BINS)

test_arcsine_distribution: test_arcsine_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_arcsine_distribution test_arcsine_distribution.cpp

test_beta_distribution: test_beta_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_beta_distribution test_beta_distribution.cpp

test_gamma_distribution: test_gamma_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_gamma_distribution test_gamma_distribution.cpp

test_hoyt_distribution: test_hoyt_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_hoyt_distribution test_hoyt_distribution.cpp

test_k_distribution2: test_k_distribution2.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_k_distribution2 test_k_distribution2.cpp

test_k_distribution: test_k_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_k_distribution test_k_distribution.cpp

test_nakagami_distribution: test_nakagami_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_nakagami_distribution test_nakagami_distribution.cpp

test_nakagami_fading_model: test_nakagami_fading_model.cpp NakagamiFadingModel.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_nakagami_fading_model test_nakagami_fading_model.cpp NakagamiFadingModel.cpp

test_pareto_distribution: test_pareto_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_pareto_distribution test_pareto_distribution.cpp

test_rice_distribution_multi: test_rice_distribution_multi.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_rice_distribution_multi test_rice_distribution_multi.cpp

test_rice_distribution: test_rice_distribution.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_rice_distribution test_rice_distribution.cpp

test_rice_fading_model: test_rice_fading_model.cpp RiceFadingModel.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_rice_fading_model test_rice_fading_model.cpp RiceFadingModel.cpp

test_logistic_distribution: test_logistic_distribution.cpp RiceFadingModel.cpp
	/home/ed/bin/bin/g++ -std=c++11 -o test_logistic_distribution test_logistic_distribution.cpp


test:
	/home/ed/bin/bin/g++ -std=c++11 -o testout/beta_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/beta_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/beta_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/beta_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/beta_typedefs.o testsuite/beta_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/beta_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/beta_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/beta_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/beta_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/beta_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/beta_distribution/operators/inequal.cc

	/home/ed/bin/bin/g++ -std=c++11 -o testout/rice_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/rice_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/rice_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/rice_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/rice_typedefs.o testsuite/rice_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/rice_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/rice_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/rice_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/rice_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/rice_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/rice_distribution/operators/inequal.cc

	/home/ed/bin/bin/g++ -std=c++11 -o testout/nakagami_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/nakagami_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/nakagami_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/nakagami_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/nakagami_typedefs.o testsuite/nakagami_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/nakagami_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/nakagami_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/nakagami_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/nakagami_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/nakagami_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/nakagami_distribution/operators/inequal.cc

	/home/ed/bin/bin/g++ -std=c++11 -o testout/k_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/k_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/k_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/k_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/k_typedefs.o testsuite/k_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/k_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/k_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/k_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/k_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/k_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/k_distribution/operators/inequal.cc

	/home/ed/bin/bin/g++ -std=c++11 -o testout/arcsine_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/arcsine_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/arcsine_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/arcsine_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/arcsine_typedefs.o testsuite/arcsine_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/arcsine_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/arcsine_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/arcsine_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/arcsine_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/arcsine_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/arcsine_distribution/operators/inequal.cc

	/home/ed/bin/bin/g++ -std=c++11 -o testout/hoyt_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/hoyt_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/hoyt_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/hoyt_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/hoyt_typedefs.o testsuite/hoyt_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/hoyt_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/hoyt_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/hoyt_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/hoyt_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/hoyt_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/hoyt_distribution/operators/inequal.cc

	/home/ed/bin/bin/g++ -std=c++11 -o testout/pareto_default -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/pareto_distribution/cons/default.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/pareto_parms -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/pareto_distribution/cons/parms.cc
	/home/ed/bin/bin/g++ -std=c++11 -c -o testout/pareto_typedefs.o testsuite/pareto_distribution/requirements/typedefs.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/pareto_serialize -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/pareto_distribution/operators/serialize.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/pareto_equal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/pareto_distribution/operators/equal.cc
	/home/ed/bin/bin/g++ -std=c++11 -o testout/pareto_inequal -I /home/ed/gcc/libstdc++-v3/testsuite/util testsuite/pareto_distribution/operators/inequal.cc


docs:
	rm -rf html latex
	doxygen
	cd latex; make pdf && cp refman.pdf ..


check:
	LD_LIBRARY_PATH=../bin/lib64:. ./check_fading_model


clean:
	rm -f $(TEST_BINS)
	rm -f testout/*
