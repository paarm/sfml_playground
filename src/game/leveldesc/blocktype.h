#pragma once

enum class BlockType {
	Nothing=0,
	FixedSingle,
	Fixed,
	Gravity,
	Spike
};

enum class SpikeDirection {
	None=0,
	Left,
	Right,
	Bottom,
	Top
};
