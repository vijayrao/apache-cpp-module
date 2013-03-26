CLEANFILES = .libs/libmod_cplusplus.so *~

libmod_cplusplus_la_SOURCES = mod_cplusplus.c \
                              apache_handler.cpp \
                              apache_filters.cpp \
                              apache_protocol.cpp \
                              cpp_request.cpp \
                              cpp_server.cpp 		\
			      apache_output_buffer.cpp  \
                              request_env.cpp           \
                              env_hash.cpp           \
			      env_value.cpp


libmod_cplusplus_la_LIBADD = $(LIB_STDCPP)
libmod_cplusplus_la_LDFLAGS = -release $(VERSION)
lib_LTLIBRARIES = libmod_cplusplus.la

make_so:
	@if test ! -L libmod_cplusplus.so ; then ln -s .libs/libmod_cplusplus.so . ; fi


