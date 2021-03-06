#include <CapacitiveSensor.h>
#include <LiquidCrystal.h>
#include <ezButton.h>
#include "pitches.h"

/*
 * Initialize capsensors on pins 7, 8, 9 with 11 being the driving pin for all
 */
CapacitiveSensor G = CapacitiveSensor(11, 8);
CapacitiveSensor A = CapacitiveSensor(11, 9);
CapacitiveSensor B = CapacitiveSensor(11, 10);


int cap_sensors[3]; 
int thresholds[3];
int keys[3]; 

int Ggreen = 3;
int Gred = 0;
int Agreen = 2;
int Ared = 1;
int Bgreen = 7;
int Bred = 6;

int PB_PIN = 12;

int switchPin = 4;

ezButton toggleSwitch(switchPin);

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
  sKEY_PRESSED_TESTING = 8,
  sNO_KEY_PRESSED_TESTING = 9,
  sGAME_OVER = 10,
} state;

typedef enum {
  LEARNING = 0,
  TESTING = 1,
} mode;

typedef enum {
  GREEN = 0,
  RED = 1, 
} color; 

typedef struct {
  mode curr_mode;
  int num_keys;
  int last_key;
  long mils;
} state_inputs;


///*
// * A struct to keep all 9 state variables in one place
// */

typedef struct {
    int saved_clock;
    int curr_song_index;
    int countdown;
    int num_correct_keys;
} state_vars;

/*
 * Variables to keep track of inputs
 */
mode curr_mode;
int last_key;
int num_keys;
int flag;

bool test_all_tests(); 

/*
 * Setup functions
 */
void initialize_system();
void calibrate();
void test_calibration();

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
void light_led(int curr_note, color c, int frequency);
void play_note(int curr_note, int duration, int saved_clock); 

state update_fsm(state cur_state, long mils, int num_keys, int last_key, mode curr_mode);
