
message .title -width 10c -justify center \
    -text "Waveform Interface" 
pack .title
set formula 0
radiobutton .f0 -text "none" -variable formula -value 0
radiobutton .f1 -text "sine" -variable formula -value 1
radiobutton .f2 -text "cosine" -variable formula -value 2
pack .f0 .f1 .f2
set waveform ""
message .tw -width 10c -justify center -text "waveform"
entry .w -width 25 -relief sunken -borderwidth 2 \
    -textvariable waveform
pack .tw .w
set output ""
message .to -width 10c -justify center -text "output"
entry .o -width 25 -relief sunken -borderwidth 2 \
    -textvariable output
pack .to .o
set speed -1
message .ts -width 10c -justify center -text "speed"
entry .s -width 25 -relief sunken -borderwidth 2 \
     -textvariable speed
pack .ts .s
set duration -1
message .tt -width 10c -justify center -text "duration"
entry .t -width 25 -relief sunken -borderwidth 2 \
     -textvariable duration
pack .tt .t
button .ok -text "OK" -command do_it
button .bad -text "exit" -command exit
pack .ok -side left
pack .bad -side right

proc do_it {} {
    global formula; global waveform
    global output;
    global speed; global duration 
    set cc(0) "./waveform"
    set cnt 1
    if { $formula == 1} {
        set cc(1) "-f"
        set cc(2) "sine"
        set cnt 3
    } else {
        if { $formula == 2} {
            set cc(1) "-f"
            set cc(2) "cosine"
            set cnt 3
        }
    }
    if { $waveform != ""} {
        set cc($cnt) "-w"
        incr cnt 1
        set cc($cnt) "$waveform"
        incr cnt 1
    }
    if { $output != ""} {
        set cc($cnt) "-o"
        incr cnt 1
        set cc($cnt) "$output"
        incr cnt 1
    }
    if { $speed != -1} {
        set cc($cnt) "-s"
        incr cnt 1
        set cc($cnt) "$speed"
        incr cnt 1
    }
    if { $duration != -1} {
        set cc($cnt) "-t"
        incr cnt 1
        set cc($cnt) "$duration"
        incr cnt 1
    }
   
    if { $cnt == 1 } {
        exec $cc(0)
    } else {
        if { $cnt == 3 } {
            exec $cc(0) $cc(1) $cc(2)
        } else {
            if { $cnt == 5 } {
                exec $cc(0) $cc(1) $cc(2) $cc(3) $cc(4)
            } else {
                if { $cnt == 7 } {
                    exec $cc(0) $cc(1) $cc(2) $cc(3) \
                         $cc(4) $cc(5) $cc(6)
                } else {
                    if { $cnt == 9 } {
                        exec $cc(0) $cc(1) $cc(2) $cc(3) \
                             $cc(4) $cc(5) $cc(6) $cc(7) \
                             $cc(8)
                    } else {
                        if { $cnt == 11 } {
                            exec $cc(0) $cc(1) $cc(2) $cc(3) \
                                 $cc(4) $cc(5) $cc(6) $cc(7) \
                                 $cc(8) $cc(9) $cc(10)
                        }
                    }
                }
            }
        }
    }
    tk_messageBox -message "Success! Output file: $output" -icon info
   
}
