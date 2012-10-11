#ifndef __MIDI_H__
#define __MIDI_H__

#include <stdint.h>

class _Midi {
	static const unsigned int baudRate = 31250;
	static const size_t dataBufferSize = 3;

	uint8_t channel;
	uint8_t currentMessage;
	uint8_t dataBuffer[dataBufferSize];
	size_t dataBufferPosition;

	void eventHandler(uint8_t data);
	void messageHandler(uint8_t status);
	friend void serialEvent();

	struct Messages {
		enum ChannelVoice {
			NoteOff               = 0x80,
			NoteOn                = 0x90,
			PolyphonicKeyPressure = 0xA0,
			ControlChange         = 0xB0,
			ProgramChange         = 0xC0,
			ChannelPressure       = 0xD0,
			PitchWheelChange      = 0xE0,
		};

		enum ChannelMode {
			AllSoundOff         = 0x78,
			ResetAllControllers = 0x79,
			LocalControl        = 0x7A,
			AllNotesOff         = 0x7B,
			OmniModeOff         = 0x7C,
			OmniModeOn          = 0x7D,
			MonoModeOn          = 0x7E,
			PolyModeOn          = 0x7F,
		};

		enum SystemCommon {
			SystemExclusive      = 0xF0,
			TimeCodeQuarterFrame = 0xF1,
			SongPositionPointer  = 0xF2,
			SongSelect           = 0xF3,
			// Undefined         = 0xF4,
			// Undefined         = 0xF5,
			TuneRequest          = 0xF6,
			EndOfExclusive       = 0xF7,
		};

		enum SystemRealTime {
			TimingClock   = 0xF8,
			// Undefined  = 0xF9,
			Start         = 0xFA,
			Continue      = 0xFB,
			Stop          = 0xFC,
			// Undefined  = 0xFD,
			ActiveSensing = 0xFE,
			Reset         = 0xFF,
		};
	};

public:
	struct Message {
		uint8_t status;
		// valid for Channel Voice messages only
		uint8_t channel;
		// enough for 3 byte SysEx manufacturer ID
		uint8_t data[_Midi::dataBufferSize];
	};

	struct Handlers {
		typedef void (*CallbackPtr)(_Midi::Message);

		CallbackPtr noteOff               = nullptr;
		CallbackPtr noteOn                = nullptr;
		CallbackPtr polyphonicKeyPressure = nullptr;
		CallbackPtr controlChange         = nullptr;
		CallbackPtr programChange         = nullptr;
		CallbackPtr channelPressure       = nullptr;
		CallbackPtr pitchWheelChange      = nullptr;

		CallbackPtr allSoundOff           = nullptr;
		CallbackPtr resetAllControllers   = nullptr;
		CallbackPtr localControl          = nullptr;
		CallbackPtr allNotesOff           = nullptr;
		CallbackPtr omniModeOff           = nullptr;
		CallbackPtr omniModeOn            = nullptr;
		CallbackPtr monoModeOn            = nullptr;
		CallbackPtr polyModeOn            = nullptr;

		CallbackPtr systemExclusive       = nullptr;
		CallbackPtr timeCodeQuarterFrame  = nullptr;
		CallbackPtr songPositionPointer   = nullptr;
		CallbackPtr songSelect            = nullptr;
		CallbackPtr tuneRequest           = nullptr;
		CallbackPtr endOfExclusive        = nullptr;

		CallbackPtr timingClock           = nullptr;
		CallbackPtr start                 = nullptr;
		CallbackPtr continue_             = nullptr;
		CallbackPtr stop                  = nullptr;
		CallbackPtr activeSensing         = nullptr;
		CallbackPtr reset                 = nullptr;
	} handlers;

	/**
	 * Begin receiving MIDI. Channel 0 means omni mode.
	 */
	void begin(int8_t channel = 0);

private:
	Handlers::CallbackPtr getControlChangeHandler();
};

// easier to remember and write
typedef _Midi::Message MidiMessage;

#ifndef MIDI_HOOK_SERIAL_EVENT
# define MIDI_HOOK_SERIAL_EVENT 1
#endif

extern _Midi Midi;

#endif
