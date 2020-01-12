#include <dreammakerfx.h>

fx_slicer   slice4(1000.0, 4);
fx_destructor dist1(0.1, 32, SMOOTH_CLIP);
fx_pitch_shift oct_down(0.5);
fx_mixer_4    mix4;
// Instances of the four bandpass (BPF) filters at different frequencies
fx_biquad_filter  filt1(200, 2, BIQUAD_TYPE_BPF, BIQUAD_ORDER_6);
fx_biquad_filter  filt2(400, 1.5, BIQUAD_TYPE_BPF, BIQUAD_ORDER_6);
fx_biquad_filter  filt3(600, 1, BIQUAD_TYPE_BPF, BIQUAD_ORDER_6);
fx_biquad_filter  filt4(800, 1, BIQUAD_TYPE_BPF, BIQUAD_ORDER_6);
fx_gain   out_gain(4.0);

void setup() {
  // put your setup code here, to run once:
  pedal.init(true);     // Init, debug enabled
  pedal.route_audio(pedal.instr_in, dist1.input);
  pedal.route_audio(dist1.output, slice4.input);
  pedal.route_audio(slice4.output_1, filt1.input);
  pedal.route_audio(slice4.output_2, filt2.input);
  pedal.route_audio(slice4.output_3, filt3.input);
  pedal.route_audio(slice4.output_4, filt4.input);
  pedal.route_audio(filt1.output, mix4.input_1);
  pedal.route_audio(filt2.output, mix4.input_2);
  pedal.route_audio(filt3.output, mix4.input_3);
  pedal.route_audio(filt4.output, mix4.input_4);
  pedal.route_audio(mix4.output, out_gain.input);
  pedal.route_audio(out_gain.output, pedal.amp_out);
  pedal.add_bypass_button(FOOTSWITCH_LEFT);

  // Right foot switch is tap loop length
  pedal.add_tap_interval_button(FOOTSWITCH_RIGHT, true);
  pedal.run();
}

float spread = 100.0;
float base_freq = 70.0;

void loop() {
  if (pedal.new_tap_interval()) { 
    slice4.set_period_ms(pedal.get_tap_interval_ms());
  } 
  
  if (pedal.pot_left.has_changed()) { 
    base_freq = 70.0 + pedal.pot_left.val* 600.0;
    filt1.set_freq(base_freq + spread);
    filt2.set_freq(base_freq + spread*2);
    filt3.set_freq(base_freq + spread*3);
    filt4.set_freq(base_freq + spread*4);      
  } 
  if (pedal.pot_center.has_changed()) {
    out_gain.set_gain( pedal.pot_center.val * 4.0);
  }
  if (pedal.pot_right.has_changed()) {
    spread = pedal.pot_right.val * 600.0;
    filt1.set_freq(base_freq + spread);
    filt2.set_freq(base_freq + spread*2);
    filt3.set_freq(base_freq + spread*3);
    filt4.set_freq(base_freq + spread*4);      
    
  }
  
  // Run pedal service to take care of stuff
  pedal.service();
}
