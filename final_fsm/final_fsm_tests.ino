///*
// * A struct to keep all three state inputs in one place
// */
//typedef struct {
//  int num_buttons;
//  orientation last_button;
//  long mils;
//} state_inputs;
//
///*
// * A struct to keep all 9 state variables in one place
// */
//typedef struct {
//  byte x;
//  byte y;
//  orientation o;
//  int lxb;
//  int uxb;
//  int level;
//  int time_step;
//  int saved_clock;
//  int countdown;
//} state_vars;
//
//extern int mocked_x;
//extern int mocked_y;
//extern orientation mocked_o;
//extern int mocked_lxb;
//extern int mocked_uxb;
//extern int mocked_level;
//extern int mocked_countdown;
//
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
//bool test_transition(state start_state,
//                     state end_state,
//                     state_inputs test_state_inputs,
//                     int display_fn,
//                     state_vars start_state_vars,
//                     state_vars end_state_vars,
//                     bool verbos) {
//  x = start_state_vars.x;
//  y = start_state_vars.y;
//  o = start_state_vars.o;
//  lxb = start_state_vars.lxb;
//  uxb = start_state_vars.uxb;
//  level = start_state_vars.level;
//  time_step = start_state_vars.time_step;
//  saved_clock = start_state_vars.saved_clock;
//  countdown = start_state_vars.countdown;
//  state result_state = update_fsm(start_state, test_state_inputs.mils, test_state_inputs.num_buttons, test_state_inputs.last_button);
//  bool passed_test = (end_state == result_state and
//                      x == end_state_vars.x and
//                      y == end_state_vars.y and
//                      o == end_state_vars.o and
//                      //lxb == end_state_vars.lxb and
//                      //uxb == end_state_vars.uxb and
//                      level == end_state_vars.level and
//                      time_step == end_state_vars.time_step and
//                      saved_clock == end_state_vars.saved_clock and
//                      countdown == end_state_vars.countdown and 
//                      uxb - lxb == end_state_vars.uxb - end_state_vars.lxb);
//  bool passed_display = true;
//  if (display_fn == 1) {
//    passed_display = (
//      mocked_x == start_state_vars.x and 
//      mocked_y == start_state_vars.y and
//      mocked_o == start_state_vars.o and
//      mocked_lxb == start_state_vars.lxb and
//      mocked_uxb == start_state_vars.uxb);
//  } else if (display_fn == 2) {
//      passed_display = (mocked_level == level);
//  } else if (display_fn == 3) {
//      passed_display = (mocked_level == level and mocked_countdown == countdown + 1);
//  }
//  if (! verbos) {
//    return passed_test and passed_display;
//  } else if (passed_test and passed_display) {
//    char s_to_print[200];
//    sprintf(s_to_print, "Test from %s to %s PASSED", s2str(start_state), s2str(end_state));
//    Serial.println(s_to_print);
//    return true;
//  } else {
//    char s_to_print[200];
//    Serial.println(s2str(start_state));
//    sprintf(s_to_print, "Test from %s to %s FAILED", s2str(start_state), s2str(end_state));
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "End state expected: %s | actual: %s", s2str(end_state), s2str(result_state));
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "Inputs: mils %ld | num_buttons %d | last_button %s", test_state_inputs.mils, test_state_inputs.num_buttons, o2str(test_state_inputs.last_button));
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "          %2s | %2s | %5s | %3s | %3s | %5s | %9s | %11s | %9s", "x", "y", "o", "lxb", "uxb", "level", "time_step", "saved_clock", "countdown");
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "expected: %2d | %2d | %5s | %3d | %3d | %5d | %9d | %11d | %9d", end_state_vars.x, end_state_vars.y, o2str(end_state_vars.o), end_state_vars.lxb, end_state_vars.uxb, end_state_vars.level, end_state_vars.time_step, end_state_vars.saved_clock, end_state_vars.countdown);
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "actual:   %2d | %2d | %5s | %3d | %3d | %5d | %9d | %11d | %9d", x, y, o2str(o), lxb, uxb, level, time_step, saved_clock, countdown);
//    Serial.println(s_to_print);
//    return false;
//  }
//}
//
///*
// * REPLACE THE FOLLOWING 6 LINES WITH YOUR TEST CASES
// */
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
