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

// 3-3 transition
bool test_all_tests() {
  return test_transition(sLEARNING_COUNTDOWN, sLEARNING_COUNTDOWN, {LEARNING, 0, -1, 6000}, {4800, 0, 2, 0}, {6000, 0, 1, 0}, true);

}

// 3-4 transition

// 4-1 transition

// 4-4 transition

// 4-5 transiton


//TESTING tests
//test_transition(test_states_in[i], test_states_out[i], test_input[i], test_in_vars[i], test_out_vars[i], true)

// 7-8(a) transition
bool test_78a = test_transition(sWAIT_FOR_KEY_TESTING, sKEY_PRESSED_TESTING, {TESTING, 1, NOTE_B5, 1000}, {0, 0, -1, 0}, {1000, 0, -1, 1}, true);

// 7-8(b) transition
bool test_78b = test_transition(sWAIT_FOR_KEY_TESTING, sKEY_PRESSED_TESTING, {TESTING, 1, NOTE_G5, 1000}, {0, 0, -1, 0}, {1000, 0, -1, 0}, true);

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
