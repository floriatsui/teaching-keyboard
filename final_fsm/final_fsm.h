#include <CapacitiveSensor.h>
#include <LiquidCrystal.h>
#include <ezButton.h>

int cap_sensors[3];
int thresholds[3];

/*
 * Initialize capsensors on pins 7, 8, 9 with 11 being the driving pin for all
 */
CapacitiveSensor G = CapacitiveSensor(11, 7);
CapacitiveSensor A = CapacitiveSensor(11, 8);
CapacitiveSensor B = CapacitiveSensor(11, 9);
ezButton toggleSwitch(10);

/*
 * Type definitions for states, modes, and notes
 * 
 */
typedef enum {
  sWAIT_FOR_MODE = 1,
  sDEMO = 2,
  sLEARNING_COUNTDOWN = 3,
  sWAIT_FOR_KEY_LEARNING = 4,
  sKEY_PRESSED_LEARNING = 5,
  sTESTING_COUNTDOWN = 6,
  sWAIT_FOR_KEY_TESTING = 7,
  sKEY_PRESSED_TEACHING = 8,
  sNO_KEY_PRESSED_TEACHING = 9,
  sGAME_OVER = 10,
} state;

typedef enum {
  LEARNING = 0,
  TEACHING = 1,
} mode;

typedef enum {
  G = 0;
  A = 1;
  B = 2;
  NO_NOTE = -1; 
} note;

typedef enum {
  GREEN = 0; 
  RED = 1; 
} color; 

/*
 * Variables to keep track of inputs
 */
mode curr_mode;
note last_key;
int num_keys;

bool test_all_tests(); // TODO

/*
 * Setup functions
 */
void initialize_system();
void calibrate();
void test_calibration();
void parse_song(); // TODO

/*
 * Read in keyboard inputs and update corresponding variables
 */
void update_inputs();

/*
 * Read in mode switch input and set mode
 */
void set_mode();

/*
 * Helper function definitions
 */
void reset_keys();
void display_curr_index(int curr_index, int total_notes);
void display_message(String message);
void light_led(note curr_note, int duration, String color, int frequency); // TODO
void dim_led(note curr_note, int duration, String color); // TODO
void play_note(note curr_note, int duration); // TODO

state update_fsm(state cur_state, long mils, int num_keys, int last_key, mode curr_mode);
