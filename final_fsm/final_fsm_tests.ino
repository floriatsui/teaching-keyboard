///*
// * A struct to keep all three state inputs in one place
// */
/*typedef struct {
  mode curr_mode;
  int num_keys;
  int last_key;
  long mils;
} state_inputs;*/


///*
// * A struct to keep all 9 state variables in one place
// */

/*typedef struct {
    int saved_clock;
    int curr_song_index;
    int countdown;
    int num_correct_keys;
} state_vars;*/


//bool test_transition(state start_state,
//                     state end_state,
//                     state_inputs test_state_inputs,
//                     int display_fn, 
//                     state_vars start_state_vars,
//                     state_vars end_state_vars,
//                     bool verbos);


///*        
// * Helper function for printing states
// */
//char* s2str(state s) {
//  switch(s) {
//    case sDISP_COUNTDOWN:
//    return "(1) DISP_COUNTDOWN";
//    case sWAIT_AFTER_ROT:
//    return "(2) WAIT_AFTER_ROT";
//    case sMOV:
//    return "(3) MOV";
//    case sWAIT_AFTER_MOV:
//    return "(4) WAIT_AFTER_MOV";
//    case sROT:
//    return "(5) ROT";
//    case sWAIT_FOR_BUT:
//    return "(6) WAIT_FOR_BUT";
//    case sGAME_OVER:
//    return "(7) GAME_OVER";
//    default:
//    return "???";
//  }
//}
//
///*
// * Helper function for printing orientations
// */
//char* o2str(orientation o) {
//  switch(o) {
//    case UP:
//    return "UP";
//    case RIGHT:
//    return "RIGHT";
//    case DOWN:
//    return "DOWN";
//    case LEFT:
//    return "LEFT";
//    default:
//    return "???";
//  }
//}
//
///*
// * Given a start state, inputs, and starting values for state variables, tests that
// * update_fsm returns the correct end state and updates the state variables correctly
// * returns true if this is the case (test passed) and false otherwise (test failed)
// * 
// * Need to use "verbos" instead of "verbose" because verbose is apparently a keyword
// */
bool test_transition(state start_state,
                     state end_state,
                     state_inputs test_state_inputs,
                     state_vars start_state_vars,
                     state_vars end_state_vars,
                     bool verbos) {

    saved_clock = start_state_vars.saved_clock;
    curr_song_index = start_state_vars.curr_song_index;
    countdown = start_state_vars.countdown;
    num_correct_keys = start_state_vars.num_correct_keys;

  state result_state = update_fsm(start_state, test_state_inputs.mils, test_state_inputs.num_keys, test_state_inputs.last_key, test_state_inputs.curr_mode);
  bool passed_test = (end_state == result_state and
                      saved_clock == end_state_vars.saved_clock and
                      curr_song_index == end_state_vars.curr_song_index and
                      countdown == end_state_vars.countdown and
                      num_correct_keys == end_state_vars.num_correct_keys);

  if (! verbos) {
    return passed_test;
  }
  else {
    // something like this
    // for debugging purposes
    Serial.println(end_state);
    Serial.println(result_state);
    Serial.println(countdown);
    Serial.println(end_state_vars.countdown);
    Serial.println(curr_song_index);
    Serial.println(end_state_vars.curr_song_index);
    Serial.println(num_correct_keys);
    Serial.println(end_state_vars.num_correct_keys);
    Serial.println(saved_clock);
    Serial.println(end_state_vars.saved_clock);
    Serial.println("start state:" + String(start_state) + ", end state: " + String(end_state));
    return passed_test;
  }
 }


// input: curr_mode, num_keys, last_key, mils
// in_vars/out_vars: saved_clock, curr_song_index, countdown, num_correct_keys

//LEARNING tests
//test_transition(test_states_in[i], test_states_out[i], test_input[i], display_fns[i], test_in_vars[i], test_out_vars[i], true)

bool test_all_tests() {
 
  bool one = test_transition(sLEARNING_COUNTDOWN, sLEARNING_COUNTDOWN, {LEARNING, 0, -1, 6000}, {4800, 0, 2, 0}, {6000, 0, 1, 0}, false); // 3-3
  bool two = test_transition(sLEARNING_COUNTDOWN, sWAIT_FOR_KEY_LEARNING, {LEARNING, 0, -1, 6000}, {4800, 0, -1, 0}, {6000, 0, -1, 0}, true); // 3-4
  bool three = test_transition(sWAIT_FOR_KEY_LEARNING, sWAIT_FOR_MODE, {LEARNING, 2, 2, 6000}, {4800, 17, -1, 0}, {6000, 17, 3, 0}, false);  // 4-1
  bool four = test_transition(sWAIT_FOR_KEY_LEARNING, sWAIT_FOR_KEY_LEARNING, {LEARNING, 0, -1, 9000}, {3800, 5, -1, 0}, {9000, 6, -1, 0}, false); // 4-4
  bool five = test_transition(sWAIT_FOR_KEY_LEARNING, sKEY_PRESSED_LEARNING, {LEARNING, 1, NOTE_A5, 6000}, {5000, 1, -1, 0}, {5000, 1, -1, 0}, true); // 4-5

  if (!one){
    Serial.println("test one failed");
  } if (!two){
    Serial.println("test two failed");
  } if (!three){
    Serial.println("test three failed");
  } if (!four){
    Serial.println("test four failed");
  } if (!five){
    Serial.println("test five failed");
  }
  
  return one and two and three and four and five;

  // 1, 2
  // 1 no transition
  // normal, learning
  bool one = test_transition(sWAIT_FOR_MODE, sWAIT_FOR_MODE, {LEARNING, 0, -1, 1000}, {0000, 0, 0, 0}, {0000, 0, 0, 0}, false);
  // 1-2
  bool onetwo = test_transition(sWAIT_FOR_MODE, sDEMO, {LEARNING, 0, -1, 11000}, {0000, 0, 0, 0}, {11010, 0, 0, 0}, false);
  // 1-6
  bool onesix = test_transition(sWAIT_FOR_MODE, sTESTING_COUNTDOWN, {TESTING, 0, -1, 11000}, {0000, 0, 0, 0}, {11010, 0, 0, 0}, false);

    int song_end = 17;
    // 2 none
  bool two = test_transition(sDEMO, sDEMO, {LEARNING, 0, -1, 11020}, {11010, 0, 0, 0}, {11010, 0, 0, 0}, false);
    // 2-2
  bool twotwo = test_transition(sDEMO, sDEMO, {LEARNING, 0, -1, 12000}, {11010, 0, 0, 0}, {12010, 1, 0, 0}, false);
    // 2-3
  bool twothree = test_transition(sDEMO, sLEARNING_COUNTDOWN, {LEARNING, 0, -1, 20000}, {19010, song_end, 0, 0}, {20010, 0, 3, 0}, false);

}

//TESTING tests
//test_transition(test_states_in[i], test_states_out[i], test_input[i], test_in_vars[i], test_out_vars[i], true)

// test_transition inputs: start state, end state, inputs, start var, end var, verbos 
// INPUT 
// curr_mode, num_keys, last_key, long mils;
// VAR 
// saved_clock, curr_song_index, countdown, num_correct_keys

// STATES 5-6 tests 
// transition from 5-4 
bool test_54 = test_transition(5, 4, {LEARNING, 1, NOTE_A5, 4000}, 
                {1500, 1, 3, 0}, {4000, 2, 3, 0}, true); 
// no state change from 5 
bool test_55 = transition(5, 5, {LEARNING, 1, NOTE_B5, 3000}, 
                {1500, 2, 3, 0}, {1500, 2, 3, 0}, true); 

// transition from 6-6 
bool test_66_self = test_transition(6, 6, {TESTING, 0, -1, 2100}, 
                {1000, 0, 3, 0}, {2000, 0, 2, 0}, true); 
// transition from 6-7
bool test_67 = test_transition(6, 7, {TESTING, 0, -1, 3000}, 
                {2000, 0, 0, 0}, {3000, 0, 3, 0}, true); 
// no state change from 6 
bool test_66 = test_transition(6, 6, {TESTING, 0, -1 2100}, 
                {2000, 0, 1, 0}, {2000, 0, 1, 0}, true); 


// 7-8(a) transition
bool test_78a = test_transition(sWAIT_FOR_KEY_TESTING, sKEY_PRESSED_TESTING, {TESTING, 1, NOTE_B5, 1000}, {0, 0, -1, 0}, {0, 0, -1, 1}, true);

// 7-8(b) transition
bool test_78b = test_transition(sWAIT_FOR_KEY_TESTING, sKEY_PRESSED_TESTING, {TESTING, 1, NOTE_G5, 1000}, {0, 0, -1, 0}, {0, 0, -1, 0}, true);

// 7-9 transition
bool test_79 = test_transition(sWAIT_FOR_KEY_TESTING, sNO_KEY_PRESSED_TESTING, {TESTING, 0, NOTE_G5, 1000}, {0, 0, -1, 0}, {0, 0, -1, 0}, true);

// 7-10 transition
bool test_710 = test_transition(sWAIT_FOR_KEY_TESTING, sGAME_OVER, {TESTING, 0, NOTE_G5, 2000}, {0, 17, -1, 0}, {0, 0, -1, 0}, true);

// 7-7 no transition
bool test_77 = test_transition(sWAIT_FOR_KEY_TESTING, sWAIT_FOR_KEY_TESTING, {TESTING, 0, NOTE_B5, 1000}, {0, 0, -1, 0}, {0, 0, -1, 0}, true);

// 8-7 transition
bool test_87 = test_transition(sKEY_PRESSED_TESTING, sWAIT_FOR_KEY_TESTING, {TESTING, 1, NOTE_G5, 2500}, {0, 0, -1, 0}, {2500, 1, -1, 0}, true);

// 8-8 no transition
bool test_88 = test_transition(sKEY_PRESSED_TESTING, sKEY_PRESSED_TESTING, {TESTING, 1, NOTE_G5, 2000}, {0, 0, -1, 0}, {0, 0, -1, 0}, true);

//const state test_states_in[24] = {(state) 1, (state) 1, (state) 1, (state) 1, (state) 2, (state) 2, (state) 2, (state) 3, (state) 3, (state) 4, (state) 4, (state) 4, (state) 5, (state) 5, (state) 5, (state) 6, (state) 6, (state) 6, (state) 7, (state) 1, (state) 1, (state) 4, (state) 5, (state) 6};
//const state test_states_out[24] = {(state) 1, (state) 1, (state) 2, (state) 6, (state) 2, (state) 3, (state) 3, (state) 4, (state) 7, (state) 4, (state) 5, (state) 5, (state) 2, (state) 6, (state) 7, (state) 6, (state) 1, (state) 7, (state) 7, (state) 2, (state) 6, (state) 5, (state) 2, (state) 7};
//const state_inputs test_input[24] = {{1,DOWN,600}, {2,UP,1600}, {4,DOWN,1600}, {3,UP,1600}, {3,DOWN,1500}, {1,LEFT,2000}, {0,LEFT,2000}, {0,RIGHT,2000}, {1,LEFT,0}, {1,UP,1500}, {1,RIGHT,2000}, {0,RIGHT,2000}, {4,DOWN,2000}, {4,LEFT,2000}, {0,DOWN,4}, {4,LEFT,2000}, {1,RIGHT,1500}, {0,RIGHT,2000}, {3,LEFT,4}, {3,DOWN,1600}, {3,LEFT,1600}, {1,RIGHT,2000}, {3,DOWN,1600}, {1,DOWN,1500}};
//const state_vars test_in_vars[24] = {{1,1,RIGHT,4,3,1,3,500,4}, {1,0,DOWN,2,3,2,3,1000,0}, {0,3,UP,0,13,1,2,1000,-1}, {0,3,LEFT,0,13,1,0,1000,-1}, {1,0,RIGHT,3,0,0,1000,1000,3}, {0,3,UP,0,13,2,1000,1000,1}, {0,3,UP,0,13,0,1000,1000,2}, {0,0,UP,0,13,1,2,1000,3}, {1,1,DOWN,16,16,3,2,4,4}, {3,2,LEFT,3,4,0,1000,1000,4}, {0,3,UP,0,13,0,1000,1000,1}, {0,3,UP,0,13,0,1000,1000,1}, {0,3,UP,0,13,0,3,1000,0}, {0,0,UP,0,13,2,2,1000,2}, {3,4,DOWN,7,7,5,0,1,1}, {3,1,RIGHT,0,2,4,1000,2000,0}, {15,0,RIGHT,1,16,2,1000,500,-1}, {15,0,RIGHT,1,16,3,1000,1000,-1}, {1,3,DOWN,0,4,4,2,2,1}, {0,0,DOWN,0,16,4,4,1000,-1}, {0,0,UP,0,16,0,0,1000,-1}, {0,3,UP,0,16,3,1000,1000,0}, {0,0,DOWN,0,16,4,1,1000,0}, {15,0,RIGHT,1,16,2,1000,500,-1}};
//const state_vars test_out_vars[24] = {{1,1,RIGHT,4,3,1,3,500,4}, {1,0,DOWN,2,3,2,3,1600,-1}, {0,3,UP,0,13,1,2,1600,-1}, {0,3,LEFT,0,13,1,0,1600,-1}, {1,0,RIGHT,3,0,0,1000,1000,3}, {0,2,UP,1,13,2,1000,1000,1}, {0,2,UP,0,13,0,1000,1000,2}, {0,0,UP,0,13,1,2,2000,3}, {1,1,DOWN,16,16,3,2,4,4}, {3,2,LEFT,3,4,0,1000,1000,4}, {0,3,RIGHT,1,13,0,1000,1000,1}, {0,3,RIGHT,0,13,0,1000,1000,1}, {0,3,UP,0,13,0,3,2000,0}, {0,0,UP,0,13,2,2,2000,2}, {3,4,DOWN,7,7,5,0,1,1}, {3,1,RIGHT,0,2,4,1000,2000,0}, {1,0,UP,1,16,3,950,1500,2}, {15,0,RIGHT,1,16,3,1000,1000,-1}, {1,3,DOWN,0,4,4,2,2,1}, {0,0,DOWN,0,16,4,4,1600,-1}, {0,0,UP,0,16,0,0,1600,-1}, {0,3,RIGHT,1,16,3,1000,1000,0}, {0,0,DOWN,0,16,4,1,1600,0}, {15,0,RIGHT,1,16,2,1000,500,-1}};
//const int display_fns[24] = {0, 3, 1, 1, 0, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 0, 3, 2, 0, 1, 1, 1, 1, 2};
//const int num_tests = 24;

//
///*
// * Runs through all the test cases defined above
// */
//bool test_all_tests() {
//  for (int i = 0; i < num_tests; i++) {
//    Serial.print("Running test ");
//    Serial.println(i);
//    if (!test_transition(test_states_in[i], test_states_out[i], test_input[i], display_fns[i], test_in_vars[i], test_out_vars[i], true)) {
//      return false;
//    }
//    Serial.println();
//  }
//  Serial.println("All tests passed!");
//  return true;
//}
