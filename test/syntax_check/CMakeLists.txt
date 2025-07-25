cmake_minimum_required(VERSION 3.10.0)
project(etl_syntax_check)

add_definitions(-DETL_DEBUG)

option(NO_STL "No STL" OFF)

if (NO_STL)
	message(STATUS "Compiling for No STL")
	add_definitions(-DETL_NO_STL)
else()
	message(STATUS "Compiling for STL")
endif()

if (ETL_USE_TYPE_TRAITS_BUILTINS)
	message(STATUS "Compiling for built-in type traits")
	add_definitions(-DETL_USE_TYPE_TRAITS_BUILTINS)
endif()

if (ETL_USER_DEFINED_TYPE_TRAITS)
	message(STATUS "Compiling for user defined type traits")
	add_definitions(-DETL_USER_DEFINED_TYPE_TRAITS)
endif()

if (ETL_FORCE_TEST_CPP03_IMPLEMENTATION)
	message(STATUS "Force C++03 implementations")
	add_definitions(-DETL_FORCE_TEST_CPP03_IMPLEMENTATION)
endif()

add_library(tests OBJECT)
target_compile_definitions(tests PRIVATE __STDC_LIMIT_MACROS __STDC_CONSTANT_MACROS __STDC_FORMAT_MACROS)
target_include_directories(tests PRIVATE "")
target_include_directories(tests SYSTEM PRIVATE ../../include)
set_target_properties(tests PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON
        )
target_compile_options(tests
	    PRIVATE
		-fsyntax-only
		)

if (ETL_CXX_STANDARD MATCHES "98")
	message(STATUS "Compiling for C++98")
    set_property(TARGET tests PROPERTY CXX_STANDARD 98)
elseif (ETL_CXX_STANDARD MATCHES "03")
	message(STATUS "Compiling for C++03 (C++98)")
	set_property(TARGET tests PROPERTY CXX_STANDARD 98)
elseif (ETL_CXX_STANDARD MATCHES "11")
	message(STATUS "Compiling for C++11")
	set_property(TARGET tests PROPERTY CXX_STANDARD 11)
elseif (ETL_CXX_STANDARD MATCHES "14")
	message(STATUS "Compiling for C++14")
	set_property(TARGET tests PROPERTY CXX_STANDARD 14)
elseif (ETL_CXX_STANDARD MATCHES "17")
	message(STATUS "Compiling for C++17")
	set_property(TARGET tests PROPERTY CXX_STANDARD 17)
elseif (ETL_CXX_STANDARD MATCHES "20")
	message(STATUS "Compiling for C++20")
	set_property(TARGET tests PROPERTY CXX_STANDARD 20)
else()
	message(STATUS "Compiling for C++23")
	set_property(TARGET tests PROPERTY CXX_STANDARD 23)
endif()

target_sources(tests PRIVATE
        etl_profile.h
		absolute.h.t.cpp
		algorithm.h.t.cpp
		alignment.h.t.cpp
		array.h.t.cpp
		array_view.h.t.cpp
		array_wrapper.h.t.cpp
		atomic.h.t.cpp
		base64.h.t.cpp
		base64_decoder.h.t.cpp
		base64_encoder.h.t.cpp
		basic_format_spec.h.t.cpp
		basic_string.h.t.cpp
		basic_string_stream.h.t.cpp
		binary.h.t.cpp
		bip_buffer_spsc_atomic.h.t.cpp
		bit.h.t.cpp
		bitset.h.t.cpp
		bit_stream.h.t.cpp
		bloom_filter.h.t.cpp
		bresenham_line.h.t.cpp
		buffer_descriptors.h.t.cpp
		byte.h.t.cpp
		byte_stream.h.t.cpp
		callback.h.t.cpp
		callback_service.h.t.cpp
		callback_timer.h.t.cpp
		callback_timer_atomic.h.t.cpp
		callback_timer_deferred_locked.h.t.cpp
		callback_timer_interrupt.h.t.cpp
		callback_timer_locked.h.t.cpp
		char_traits.h.t.cpp
		checksum.h.t.cpp
		chrono.h.t.cpp
		circular_buffer.h.t.cpp
		circular_iterator.h.t.cpp
		closure.h.t.cpp
		combinations.h.t.cpp
		compare.h.t.cpp
		constant.h.t.cpp
		container.h.t.cpp
		correlation.h.t.cpp
		covariance.h.t.cpp
		crc1.h.t.cpp
		crc16.h.t.cpp
		crc16_a.h.t.cpp
		crc16_arc.h.t.cpp
		crc16_aug_ccitt.h.t.cpp
		crc16_buypass.h.t.cpp
		crc16_ccitt.h.t.cpp
		crc16_cdma2000.h.t.cpp
		crc16_dds110.h.t.cpp
		crc16_dectr.h.t.cpp
		crc16_dectx.h.t.cpp
		crc16_dnp.h.t.cpp
		crc16_en13757.h.t.cpp
		crc16_genibus.h.t.cpp
		crc16_kermit.h.t.cpp
		crc16_m17.h.t.cpp
		crc16_maxim.h.t.cpp
		crc16_mcrf4xx.h.t.cpp
		crc16_modbus.h.t.cpp
		crc16_profibus.h.t.cpp
		crc16_riello.h.t.cpp
		crc16_t10dif.h.t.cpp
		crc16_teledisk.h.t.cpp
		crc16_tms37157.h.t.cpp
		crc16_usb.h.t.cpp
		crc16_x25.h.t.cpp
		crc16_xmodem.h.t.cpp
		crc32.h.t.cpp
		crc32_bzip2.h.t.cpp
		crc32_c.h.t.cpp
		crc32_d.h.t.cpp
		crc32_jamcrc.h.t.cpp
		crc32_mpeg2.h.t.cpp
		crc32_posix.h.t.cpp
		crc32_q.h.t.cpp
		crc32_xfer.h.t.cpp
		crc64_ecma.h.t.cpp
		crc8_ccitt.h.t.cpp
		crc8_cdma2000.h.t.cpp
		crc8_darc.h.t.cpp
		crc8_dvbs2.h.t.cpp
		crc8_ebu.h.t.cpp
		crc8_icode.h.t.cpp
		crc8_itu.h.t.cpp
		crc8_j1850.h.t.cpp
		crc8_j1850_zero.h.t.cpp
		crc8_maxim.h.t.cpp
		crc8_rohc.h.t.cpp
		crc8_wcdma.h.t.cpp
		cyclic_value.h.t.cpp
		debounce.h.t.cpp
		debug_count.h.t.cpp
		delegate.h.t.cpp
		delegate_service.h.t.cpp
		deque.h.t.cpp
		endianness.h.t.cpp
		enum_type.h.t.cpp
		error_handler.h.t.cpp
		exception.h.t.cpp
		expected.h.t.cpp
		factorial.h.t.cpp
		fibonacci.h.t.cpp
		file_error_numbers.h.t.cpp
		fixed_iterator.h.t.cpp
		fixed_sized_memory_block_allocator.h.t.cpp
		flags.h.t.cpp
		flat_map.h.t.cpp
		flat_multimap.h.t.cpp
		flat_multiset.h.t.cpp
		flat_set.h.t.cpp
		fnv_1.h.t.cpp
		format_spec.h.t.cpp
		forward_list.h.t.cpp
		frame_check_sequence.h.t.cpp
		fsm.h.t.cpp
		function.h.t.cpp
		functional.h.t.cpp
		function_traits.h.t.cpp
		gamma.h.t.cpp
		gcd.h.t.cpp
		generic_pool.h.t.cpp
		hash.h.t.cpp
		hfsm.h.t.cpp
		histogram.h.t.cpp
		ihash.h.t.cpp
		imemory_block_allocator.h.t.cpp
		indirect_vector.h.t.cpp
		initializer_list.h.t.cpp
		instance_count.h.t.cpp
		integral_limits.h.t.cpp
		intrusive_forward_list.h.t.cpp
		intrusive_links.h.t.cpp
		intrusive_list.h.t.cpp
		intrusive_queue.h.t.cpp
		intrusive_stack.h.t.cpp
		invert.h.t.cpp
		io_port.h.t.cpp
		ipool.h.t.cpp
		ireference_counted_message_pool.h.t.cpp
		iterator.h.t.cpp
		jenkins.h.t.cpp
		largest.h.t.cpp
		lcm.h.t.cpp
		limiter.h.t.cpp
		limits.h.t.cpp
		list.h.t.cpp
		log.h.t.cpp
		macros.h.t.cpp
		map.h.t.cpp
		math.h.t.cpp
		math_constants.h.t.cpp
		mean.h.t.cpp
		memory.h.t.cpp
		memory_model.h.t.cpp
		mem_cast.h.t.cpp
		message.h.t.cpp
		message_broker.h.t.cpp
		message_bus.h.t.cpp
		message_packet.h.t.cpp
		message_router.h.t.cpp
		message_router_registry.h.t.cpp
		message_timer.h.t.cpp
		message_timer_atomic.h.t.cpp
		message_timer_interrupt.h.t.cpp
		message_timer_locked.h.t.cpp
		message_types.h.t.cpp
		multimap.h.t.cpp
		multiset.h.t.cpp
		multi_array.h.t.cpp
		multi_range.h.t.cpp
		multi_span.h.t.cpp
		multi_vector.h.t.cpp
		murmur3.h.t.cpp
		mutex.h.t.cpp
		negative.h.t.cpp
		nth_type.h.t.cpp
		nullptr.h.t.cpp
		null_type.h.t.cpp
		numeric.h.t.cpp
		observer.h.t.cpp
		optional.h.t.cpp
		overload.h.t.cpp
		packet.h.t.cpp
		parameter_pack.h.t.cpp
		parameter_type.h.t.cpp
		pearson.h.t.cpp
		permutations.h.t.cpp
		placement_new.h.t.cpp
		platform.h.t.cpp
		poly_span.h.t.cpp
		pool.h.t.cpp
		power.h.t.cpp
		priority_queue.h.t.cpp
		pseudo_moving_average.h.t.cpp
		quantize.h.t.cpp
		queue.h.t.cpp
		queue_lockable.h.t.cpp
		queue_mpmc_mutex.h.t.cpp
		queue_spsc_atomic.h.t.cpp
		queue_spsc_isr.h.t.cpp
		queue_spsc_locked.h.t.cpp
		radix.h.t.cpp
		random.h.t.cpp
		ratio.h.t.cpp
		reference_counted_message.h.t.cpp
		reference_counted_message_pool.h.t.cpp
		reference_counted_object.h.t.cpp
		reference_flat_map.h.t.cpp
		reference_flat_multimap.h.t.cpp
		reference_flat_multiset.h.t.cpp
		reference_flat_set.h.t.cpp
		rescale.h.t.cpp
		result.h.t.cpp
		rms.h.t.cpp
		scaled_rounding.h.t.cpp
		scheduler.h.t.cpp
		set.h.t.cpp
		shared_message.h.t.cpp
		singleton.h.t.cpp
		singleton_base.h.t.cpp
		smallest.h.t.cpp
		span.h.t.cpp
		sqrt.h.t.cpp
		stack.h.t.cpp
		standard_deviation.h.t.cpp
		state_chart.h.t.cpp
		static_assert.h.t.cpp
		string.h.t.cpp
		stringify.h.t.cpp
		string_stream.h.t.cpp
		string_utilities.h.t.cpp
		string_view.h.t.cpp
		successor.h.t.cpp
		task.h.t.cpp
		threshold.h.t.cpp
		timer.h.t.cpp
		to_arithmetic.h.t.cpp
		to_string.h.t.cpp
		to_u16string.h.t.cpp
		to_u32string.h.t.cpp
		to_u8string.h.t.cpp
		to_wstring.h.t.cpp
		tuple.h.t.cpp
		type_def.h.t.cpp
		type_lookup.h.t.cpp
		type_select.h.t.cpp
		type_traits.h.t.cpp
		u16format_spec.h.t.cpp
		u16string.h.t.cpp
		u16string_stream.h.t.cpp
		u32format_spec.h.t.cpp
		u32string.h.t.cpp
		u32string_stream.h.t.cpp
		u8format_spec.h.t.cpp
		u8string.h.t.cpp
		u8string_stream.h.t.cpp
		unaligned_type.h.t.cpp
		uncopyable.h.t.cpp
		unordered_map.h.t.cpp
		unordered_multimap.h.t.cpp
		unordered_multiset.h.t.cpp
		unordered_set.h.t.cpp
		user_type.h.t.cpp
		utility.h.t.cpp
		variance.h.t.cpp
		variant.h.t.cpp
		variant_pool.h.t.cpp
		vector.h.t.cpp
		version.h.t.cpp
		visitor.h.t.cpp
		wformat_spec.h.t.cpp
		wstring.h.t.cpp
		wstring_stream.h.t.cpp
        )
