#
# Copyright (c) 2013 LinkedIn Corp. All rights reserved. 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of the license at
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under the
# License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied.
#

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

#todo : these should be automated.
OS = linux
ARCH = x64
BUILDTYPE=Debug
PAGESPEED_INCLUDE_BASE=${pagespeed}

AM_CPPFLAGS = \
		-ggdb \
		-O0 \
		-Wall \
		-fno-ident \
	    -fdata-sections \
		-I${PAGESPEED_INCLUDE_BASE} \
		-I${PAGESPEED_INCLUDE_BASE} \
 		-I${PAGESPEED_INCLUDE_BASE}/third_party/chromium/src \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/google-sparsehash/src \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/google-sparsehash/gen/arch/${OS}/${ARCH}/include \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/protobuf/src \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/re2/src \
        -I${PAGESPEED_INCLUDE_BASE}/out/${BUILDTYPE}/obj/gen \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/apr/src/include \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/aprutil/src/include \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/apr/gen/arch/${OS}/${ARCH}/include \
        -I${PAGESPEED_INCLUDE_BASE}/third_party/aprutil/gen/arch/${OS}/${ARCH}/include
			
target=ats_pagespeed.so
pkglibdir = ${libexecdir}/trafficserver
pkglib_LTLIBRARIES = ats_pagespeed.la

ats_pagespeed_la_SOURCES = \
				ats_pagespeed.cpp \
				ats_message_handler.cpp \
				ats_stats_variable.cpp \
				ats_rewrite_driver_factory.cpp \
				ats_server_context.cpp \
				ats_rewrite_options.cpp \
				ats_pagespeed_configuration.cpp \
				ats_native_fetcher.cpp \
				${PAGESPEED_INCLUDE_BASE}/net/instaweb/apache/add_headers_fetcher.cc \
    			${PAGESPEED_INCLUDE_BASE}/net/instaweb/apache/loopback_route_fetcher.cc \
    			${PAGESPEED_INCLUDE_BASE}/net/instaweb/apache/serf_url_async_fetcher.cc \
    			${PAGESPEED_INCLUDE_BASE}/out/${BUILDTYPE}/obj/gen/gperf_out/instaweb/kernel/js/js_keywords.gp.cc

# ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_panel_config_pb.a
# ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_timing_pb.a 
# ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/chromium/src/net/tools/liburl_to_filename_encoder.a
# ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/js/libpagespeed_jsminify.a 

#PAGESPEED_ARCHIVES_PSOL= \
#                    ${pagespeed}/lib/${BUILDTYPE}/${OS}/${ARCH}/libapr.a \
#                    ${pagespeed}/lib/${BUILDTYPE}/${OS}/${ARCH}/libaprutil.a \
#                    ${pagespeed}/lib/${BUILDTYPE}/${OS}/${ARCH}/libserf.a \
#                    ${pagespeed}/lib/${BUILDTYPE}/${OS}/${ARCH}/pagespeed_automatic.a

PAGESPEED_ARCHIVES= \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libautomatic_util.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_apr.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/base/libbase.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/pagespeed/libjsminify.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/gflags/libgflags.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/base/libbase_static.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/modp_b64/libmodp_b64.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/chromium/src/base/third_party/dynamic_annotations/libdynamic_annotations.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/serf/libserf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/apr/libapr.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/aprutil/libaprutil.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/zlib/libzlib.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_automatic.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_blink_critical_line_data_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/protobuf/libprotobuf_lite.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libhttp_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/core/libpagespeed_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/build/temp_gyp/libgoogleurl.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/icu/libicuuc.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/icu/libicudata.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/proto/libpagespeed_output_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/proto/libpagespeed_resource_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/proto/libtimeline_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/domain_registry_provider/src/domain_registry/libdomain_registry_lib.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/domain_registry_provider/src/domain_registry/libassert_lib.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_htmlparse_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_htmlparse_core_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_html.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_inline_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_inline_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_detect_reflow_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_detect_reflow_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_deterministic_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_deterministic_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_js_defer_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_js_defer_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_lazyload_images_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_lazyload_images_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_local_storage_cache_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_local_storage_cache_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_css_beacon_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_css_beacon_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_images_beacon_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_images_beacon_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_defer_iframe_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_defer_iframe_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_base.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_system.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_clientstate_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_propcache_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/base64/libbase64.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_html_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_html_option_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_spriter_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_css.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_spriter.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util_enums_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_logging_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_images_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_image_types_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpng/libpng.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/css_parser/libcss_parser.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/css_parser/libutf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_image.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/image_compression/libpagespeed_image_converter.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/image_compression/libpagespeed_jpeg_optimizer.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/image_compression/libpagespeed_jpeg_reader.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libjpeg/libjpeg.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/image_compression/libpagespeed_png_optimizer.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/third_party/giflib/libdgiflib.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/third_party/giflib/libgiflib_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/third_party/optipng/libopngreduc.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/image_compression/libpagespeed_jpeg_utils.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libwebp/libwebp_enc.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libwebp/libwebp_dec.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libwebp/libwebp_dsp.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libwebp/libwebp_utils.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/opencv/libhighgui.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/opencv/libopencv_imgproc.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/opencv/libopencv_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_javascript.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/jsoncpp/libjsoncpp.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/re2/libre2.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_htmlparse.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util_pthread.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libprocess_context.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libautomatic_util.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libhttp_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_add_instrumentation_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_add_instrumentation_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_apr.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_automatic.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_blink_critical_line_data_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_cache_html_info_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_client_domain_rewriter_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_client_domain_rewriter_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_clientstate_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_console.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_console_body_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_console_css_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_console_js_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_css_beacon_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_css_beacon_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_css_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_images_beacon_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_images_beacon_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_images_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_line_info_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_critical_selectors_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_defer_iframe_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_defer_iframe_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_inline_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_inline_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_delay_images_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_detect_reflow_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_detect_reflow_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_deterministic_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_deterministic_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_flush_early_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_htmlparse.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_htmlparse_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_htmlparse_core_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_http_test.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_image_types_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_javascript.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_js_defer_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_js_defer_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_lazyload_images_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_lazyload_images_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_local_storage_cache_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_local_storage_cache_opt_data2c.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_logging_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_propcache_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_base.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_css.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_html.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_html_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_html_option_gperf.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_image.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_javascript.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_rewriter_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_spriter.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_spriter_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_spriter_test.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_system.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util_core.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util_enums_pb.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/net/instaweb/libinstaweb_util_pthread.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/third_party/libpagespeed/src/pagespeed/image_compression/libpagespeed_scanline_utils.a \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/base/pagespeed/kernel/base/string_util.o \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target//jsminify/pagespeed/kernel/js/js_minify.o \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/util/pagespeed/kernel/util/fast_wildcard_group.o \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/pagespeed_webp_optimizer/third_party/libpagespeed/src/pagespeed/image_compression/webp_optimizer.o \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/util/pagespeed/kernel/util/wildcard.o \
                    ${pagespeed}/out/${BUILDTYPE}/obj.target/util/pagespeed/kernel/util/rolling_hash.o

# Because libtool is a "genius" it decides to move all of the archive files out of the group which of course
# will cause problems with the loader because it will have unresolved symbols. So because we need to group
# the archive files, we'll have to do the comma trick below in the group to confuse libtool so it doesn't
# move the archive files...does anyone know anything better than this? If so email me.

# Remove spaces for libtool hack
NOTHING:=
SPACE:=$(NOTHING) $(NOTHING)
COMMA:=,
PAGESPEED_ARCHIVES_COMMA_SEPERATED=$(subst $(SPACE),$(COMMA),$(PAGESPEED_ARCHIVES))

ats_pagespeed_la_LDFLAGS =   \
				-module \
				-avoid-version \
				-shared \
				-Wl,--export-dynamic \
				-Wl,--as-needed \
				-Wl,--gc-sections \
				-Wl,--start-group,$(PAGESPEED_ARCHIVES_COMMA_SEPERATED),--end-group

ats_pagespeed_la_LIBADD = \
	-latscppapi \
        -lyaml-cpp \
        -lrt \
        -lpcre \
	-lpthread

all:
	ln -sf .libs/$(target)

clean-local:
	rm -f $(target)
						   
