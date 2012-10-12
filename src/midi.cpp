// Auduino Midi, a simple client implementation
//
// by Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:
// 12 Oct 2012: System Common and Real time made optional

#include <Arduino.h>
#include "midi.h"
#include "debug.h"

// The Instance
_Midi Midi;

//---------------------------------------------------------//
// Choose serial here (Serial, Serial1, Serial2 or Serial3)
static auto &serial = Serial;

#if MIDI_HOOK_SERIAL_EVENT
// Choose serial event callback here
void serialEvent() {
//void serialEvent1() {
//void serialEvent2() {
//void serialEvent3() {
	while (serial.available()) {
		Midi.eventHandler(serial.read());
	}
}
#endif
//---------------------------------------------------------//

inline constexpr uint8_t denseIndexFromStatus(uint8_t status) {
	return status < 0xF0
		// Channel Voice: 0x00 - 0x06
		? (status >> 4) - 0x08
		// Common, Real time: 0x07 - 0x16
		: (status & 0x0F) + 0x07;
}

inline constexpr uint8_t channelFromStatus(uint8_t status) {
	return (status & 0x0F) + 1;
}

void _Midi::begin(int8_t channel_) {
	DEBUG_WRITE("_Midi::begin(channel = ");
	DEBUG_WRITE(channel_);
	DEBUG_WRITE(")\n");

	serial.begin(baudRate);
	channel = channel_;
	dataBufferPosition = 0;
	currentMessage = 0;
}

_Midi::Handlers::CallbackPtr _Midi::getControlChangeHandler() {
	Handlers::CallbackPtr handler = nullptr;
	// first data byte has controller number
#if MIDI_CHANNEL_MODE
	switch (dataBuffer[0]) {
		case Messages::AllSoundOff:         handler = handlers.allSoundOff;         break;
		case Messages::ResetAllControllers: handler = handlers.resetAllControllers; break;
		case Messages::LocalControl:        handler = handlers.localControl;        break;
		case Messages::AllNotesOff:         handler = handlers.allNotesOff;         break;
		case Messages::OmniModeOff:         handler = handlers.omniModeOff;         break;
		case Messages::OmniModeOn:          handler = handlers.omniModeOn;          break;
		case Messages::MonoModeOn:          handler = handlers.monoModeOn;          break;
		case Messages::PolyModeOn:          handler = handlers.polyModeOn;          break;
		default:
#else
	if (databuffer[0] < 120) {
#endif
			handler = handlers.controlChange;
	}

	return handler;
}

void _Midi::messageHandler(uint8_t status) {
	// Remember that channel is only valid for Channel Voice Messages
	uint8_t statusChannel = channelFromStatus(status);
	// ...so check if this is a Channel Voice etc message and this
	// is our channel, or return
	if (channel && status < 0xF0 && channel != statusChannel) {
		return;
	}

	Handlers::CallbackPtr handler = nullptr;

	switch (denseIndexFromStatus(status)) {
		case denseIndexFromStatus(Messages::NoteOff):               handler = handlers.noteOff;               break;
		case denseIndexFromStatus(Messages::NoteOn):                handler = handlers.noteOn;                break;
		case denseIndexFromStatus(Messages::PolyphonicKeyPressure): handler = handlers.polyphonicKeyPressure; break;
		case denseIndexFromStatus(Messages::ControlChange):         handler = getControlChangeHandler();      break;
		case denseIndexFromStatus(Messages::ProgramChange):         handler = handlers.programChange;         break;
		case denseIndexFromStatus(Messages::ChannelPressure):       handler = handlers.channelPressure;       break;
		case denseIndexFromStatus(Messages::PitchWheelChange):      handler = handlers.pitchWheelChange;      break;
#if MIDI_SYSTEM_COMMON
// NOTE: disabling System Common, but enabling SystemRealTime will
// make this switch/case sparse, could cause crappier compiler results?
		case denseIndexFromStatus(Messages::SystemExclusive):       handler = handlers.systemExclusive;       break;
		case denseIndexFromStatus(Messages::TimeCodeQuarterFrame):  handler = handlers.timeCodeQuarterFrame;  break;
		case denseIndexFromStatus(Messages::SongPositionPointer):   handler = handlers.songPositionPointer;   break;
		case denseIndexFromStatus(Messages::SongSelect):            handler = handlers.songSelect;            break;
		case denseIndexFromStatus(0xF4):                            /* Undefined */                           break;
		case denseIndexFromStatus(0xF5):                            /* Undefined */                           break;
		case denseIndexFromStatus(Messages::TuneRequest):           handler = handlers.tuneRequest;           break;
		case denseIndexFromStatus(Messages::EndOfExclusive):        handler = handlers.endOfExclusive;        break;
#endif
#if MIDI_SYSTEM_REAL_TIME
		case denseIndexFromStatus(Messages::TimingClock):           handler = handlers.timingClock;           break;
		case denseIndexFromStatus(0xF9):                            /* Undefined */                           break;
		case denseIndexFromStatus(Messages::Start):                 handler = handlers.start;                 break;
		case denseIndexFromStatus(Messages::Continue):              handler = handlers.continue_;             break;
		case denseIndexFromStatus(Messages::Stop):                  handler = handlers.stop;                  break;
		case denseIndexFromStatus(0xFD):                            /* Undefined */                           break;
		case denseIndexFromStatus(Messages::ActiveSensing):         handler = handlers.activeSensing;         break;
		case denseIndexFromStatus(Messages::Reset):                 handler = handlers.reset;                 break;
#endif
	}

	if (handler) {
		Message message = {
			status,
			statusChannel,
			{ dataBuffer[0], dataBuffer[1], dataBuffer[2] },
		};

		handler(message);
	}
}

void _Midi::eventHandler(uint8_t data) {
	if (data & 0x80) {
		messageHandler(currentMessage);
		// Reset data buffer
		dataBufferPosition = 0;
		currentMessage = data;
	} else if (dataBufferPosition < dataBufferSize) {
		dataBuffer[dataBufferPosition++] = data;
	}
}
