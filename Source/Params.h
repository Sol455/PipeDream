/*
  ==============================================================================

    Params.h
    Created: 20 Mar 2023 1:12:32pm
    Author:  Sol Harter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


namespace Params {
enum Names {
    Pitch_Sel_1,
    Pitch_Sel_2,
    Pitch_Sel_3,
    Pitch_Sel_4,
    Pitch_Sel_5,
    
    Gain_Out_1,
    Gain_Out_2,
    Gain_Out_3,
    Gain_Out_4,
    Gain_Out_5,
    
    Chord_Sel,
    Root_Sel,
    
    Chord_Hold,
    
    Dry_Wet,
    Decay_Time,
    
    Low_Pass_Cut_Off,
    High_Pass_Cut_Off,
    
    IR_select,
    
    Gain_In_Master,
    Gain_Out_Master
    
    
    
};
inline const std::map<Names, juce::String>& GetParams()
{
    static std::map<Names, juce::String> params =
    {
        
        {Pitch_Sel_1, "Pitch_Sel_1"},
        {Pitch_Sel_2, "Pitch_Sel_2"},
        {Pitch_Sel_3, "Pitch_Sel_3"},
        {Pitch_Sel_4, "Pitch_Sel_4"},
        {Pitch_Sel_5, "Pitch_Sel_5"},
        
        {Gain_Out_1, "Gain_Out_1"},
        {Gain_Out_2, "Gain_Out_2"},
        {Gain_Out_3, "Gain_Out_3"},
        {Gain_Out_4, "Gain_Out_4"},
        {Gain_Out_5, "Gain_Out_5"},
        
        {Chord_Sel, "Chord_Sel"},
        {Root_Sel, "Root_Sel"},
        {Chord_Hold, "Chord_Hold"},
        {Dry_Wet, "Dry_Wet"},
        {Decay_Time, "Decay_Time"},
        
        {Low_Pass_Cut_Off, "Low_Pass_Cut_Off"},
        {High_Pass_Cut_Off, "High_Pass_Cut_Off"},
        
        {IR_select, "IR_select"},
        
        {Gain_In_Master, "Gain_In_Master"},
        {Gain_Out_Master, "Gain_Out_Master"},
    
        
        
    };
    
    return params;
}
}
