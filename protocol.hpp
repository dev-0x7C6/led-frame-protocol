#pragma once

// development draft

#include <externals/common/types.hpp>

namespace ledframe {
namespace proto {

enum class command : u8 {
	undef = 0x00,
	init,
	sync,
	info,
	push,
	clear,
	done,
};

enum class response {
	none,
	ready,
	synced,
};

struct command_query {
	constexpr command_query(const command cmd)
			: cmd(static_cast<u8>(cmd)) {
	}

	const char header[3]{'L', 'F', '_'};
	const u8 cmd;
	const char end_header = '\0';
} __attribute__((__packed__));


struct push_flags {
	enum class id {
		interlaced,
		interlaced_frame_even,
	};

	constexpr void set(const id flag, const bool value = true) noexcept {
		if (value)
			flags |= 0x01 << static_cast<u8>(flag);
		else
			flags &= ~(0x01 << static_cast<u8>(flag));
	}

	constexpr bool test(const id flag) const noexcept {
		return (flags >> static_cast<u8>(flag)) & 0x01;
	}

private:
	u16 flags{};
};

struct command_push_params {
	push_flags flags;
	u16 led_count;
	u16 sum;
};

struct command_query_push {
	command_query header{command::push};
	__attribute__((__packed__)) command_push_params params{};
};

enum class position : u8 {
	left,
	top,
	right,
	bottom,
	center
};

enum class order : u8 {
	clockwise,
	counter_clockwise,
};

struct strip_param {
	u8 palette : 4;
	u8 ord : 1;
	u8 pos : 3;
	u16 count;
} __attribute__((__packed__));

static_assert(sizeof(strip_param) == 3);

struct command_info_params {
	u8 version{};
	u8 sequences{};
	char name[32]{};
	strip_param directions[16]{};
};

// static tests
static_assert(sizeof(command_query(command::undef)) == 5);
static_assert(sizeof(command_query_push) == 11);

static_assert(sizeof(strip_param) == 3);
static_assert(sizeof(command_info_params) == 82);

}
}
