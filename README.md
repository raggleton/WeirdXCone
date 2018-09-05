# weird-xcone

Minimal working example of bizarre event where XCone/Njettiness not producing N jets as desired.

## Install

Install fastjet and fastjet-contrib. I used fastjet 3.2.1 and fj-contrib 1.033.
Ensure `fastjet-config` is on `$PATH`.

Compile the program with `make`

## Run

`./weird-xcone < weirdEvent.dat`

Should see a fatjet, with 2 subjets. Each subjet should have 3 jets, but one only has 2.
