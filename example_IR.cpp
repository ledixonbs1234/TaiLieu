
#include <Arduino.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// ==================== start of TUNEABLE PARAMETERS ====================

// The GPIO an IR detector/demodulator is connected to. Recommended: 14 (D5)
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 14;

// GPIO to use to control the IR LED circuit. Recommended: 4 (D2).
const uint16_t kIrLedPin = 4;

// The Serial connection baud rate.
// NOTE: Make sure you set your Serial Monitor to the same speed.
const uint32_t kBaudRate = 115200;

// As this program is a special purpose capture/resender, let's use a larger
// than expected buffer so we can handle very large IR messages.
// i.e. Up to 512 bits.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
const uint8_t kTimeout = 50;  // Milli-Seconds

// kFrequency is the modulation frequency all messages will be replayed at.
const uint16_t kFrequency = 38000;  // in Hz. e.g. 38kHz.

// ==================== end of TUNEABLE PARAMETERS ====================

// The IR transmitter.
IRsend irsend(kIrLedPin);
// The IR receiver.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
// Somewhere to store the captured message.
decode_results results;

// This section of code runs only once at start-up.
void setup() {
  irrecv.enableIRIn();  // Start up the IR receiver.
  irsend.begin();       // Start up the IR sender.

  Serial.begin(kBaudRate, SERIAL_8N1);
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();

  Serial.print("DumbIRRepeater is now running and waiting for IR input "
               "on Pin ");
  Serial.println(kRecvPin);
  Serial.print("and will retransmit it on Pin ");
  Serial.println(kIrLedPin);
}

// The repeating section of the code
void loop() {
  // Check if an IR message has been received.
  if (irrecv.decode(&results)) {  // We have captured something.
    // The capture has stopped at this point.

    // Convert the results into an array suitable for sendRaw().
    // resultToRawArray() allocates the memory we need for the array.
    uint16_t *raw_array = resultToRawArray(&results);
    // Find out how many elements are in the array.
    uint16_t length = getCorrectedRawLength(&results);
    // Send it out via the IR LED circuit.
    irsend.sendRaw(raw_array, length, kFrequency);
    // Resume capturing IR messages. It was not restarted until after we sent
    // the message so we didn't capture our own message.
    irrecv.resume();
    // Deallocate the memory allocated by resultToRawArray().
    delete [] raw_array;

    // Display a crude timestamp & notification.
    uint32_t now = millis();
    Serial.printf(
        "%06u.%03u: A message that was %d entries long was retransmitted.\n",
        now / 1000, now % 1000, length);
  }
  yield();  // Or delay(milliseconds); This ensures the ESP doesn't WDT reset.
}