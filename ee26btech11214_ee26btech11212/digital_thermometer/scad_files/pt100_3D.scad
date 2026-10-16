//below mentioned dimensions are in mm
union(){
    difference(){
        cube([181,91,109]);
        translate([4,4,4]){cube([173,83,105]);}
        translate([20,0,26]){cube([13,4,12]);}
        translate([108,87,21]){cube([6,4,6]);}}
        difference(){
            translate([5,4,4]){cube([65,80,15]);}
                translate([8,7,4]){cube([59,74,15]);}}
                difference(){
                    translate([80,4,4]){cube([70,50,15]);}
                    translate([83,7,4]){cube([64,44,15]);}}
                    translate([140,4,40]){cube([5,80,69]);}}