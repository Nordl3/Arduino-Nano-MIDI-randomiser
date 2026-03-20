/*
  SY-1 MIDI OUT TEST
  Arduino Nano -> MIDI OUT only

  Assumes SY-1 is in MIDI Mode 2
  Base MIDI channel = 1

  Left drum  = MIDI channel 1
  Right drum = MIDI channel 2

  Sends two fixed 8-step sequences with varying pitch.
*/

const byte CH_LEFT  = 0;   // MIDI channel 1 (0-based internally)
const byte CH_RIGHT = 1;   // MIDI channel 2

const unsigned long STEP_MS = 160;   // adjust tempo here
const unsigned long NOTE_LEN_MS = 40;

const byte STEPS = 8;

// 0 = no trigger on that step
// 1 = trigger on that step
byte trigLeft[STEPS]  = {1, 0, 1, 0, 1, 1, 0, 1};
byte trigRight[STEPS] = {0, 1, 0, 1, 1, 0, 1, 1};

// Valid SY-1 note range in Mode 2 is 21..84
byte pitchLeft[STEPS]  = {24, 31, 40, 28, 52, 36, 60, 44};
byte pitchRight[STEPS] = {72, 64, 55, 79, 48, 83, 33, 69};

byte stepIndex = 0;
unsigned long lastStepTime = 0;

bool leftNoteOn = false;
bool rightNoteOn = false;
byte currentLeftNote = 36;
byte currentRightNote = 36;
unsigned long leftOffTime = 0;
unsigned long rightOffTime = 0;

void setup() {
  Serial.begin(31250);   // MIDI baud rate
}

void loop() {
  unsigned long now = millis();

  // advance sequencer
  if (now - lastStepTime >= STEP_MS) {
    lastStepTime = now;

    // LEFT
    if (trigLeft[stepIndex]) {
      currentLeftNote = pitchLeft[stepIndex];
      sendNoteOn(CH_LEFT, currentLeftNote, 110);
      leftNoteOn = true;
      leftOffTime = now + NOTE_LEN_MS;
    }

    // RIGHT
    if (trigRight[stepIndex]) {
      currentRightNote = pitchRight[stepIndex];
      sendNoteOn(CH_RIGHT, currentRightNote, 110);
      rightNoteOn = true;
      rightOffTime = now + NOTE_LEN_MS;
    }

    stepIndex++;
    if (stepIndex >= STEPS) {
      stepIndex = 0;
    }
  }

  // note offs
  if (leftNoteOn && now >= leftOffTime) {
    sendNoteOff(CH_LEFT, currentLeftNote, 0);
    leftNoteOn = false;
  }

  if (rightNoteOn && now >= rightOffTime) {
    sendNoteOff(CH_RIGHT, currentRightNote, 0);
    rightNoteOn = false;
  }
}

void sendNoteOn(byte channel, byte note, byte velocity) {
  Serial.write(0x90 | (channel & 0x0F));
  Serial.write(note & 0x7F);
  Serial.write(velocity & 0x7F);
}

void sendNoteOff(byte channel, byte note, byte velocity) {
  Serial.write(0x80 | (channel & 0x0F));
  Serial.write(note & 0x7F);
  Serial.write(velocity & 0x7F);
}