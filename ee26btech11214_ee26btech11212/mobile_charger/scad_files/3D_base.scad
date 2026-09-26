//below dimensions are in units of millimeter(mm)


union(){
    difference(){
       cube([162.5,87,57]);/*outer cuboid*/
       translate([4,4,4]){cube([154.5,79,53]);}/*cavity inside outer cuboid*/
       translate([0,31,52]){cube([4,25,5]);}/*hole for transformer input*/
       translate([158.5,21.5,27]){cube([4,15.5,7]);}/*hole for USB-A*/}
    translate([55.5,4,4]){cube([10,79,42]);} /*middle partetion*/  
    
    /*platform for PCB*/
    
    translate([88.5,18.5,4]){      
           difference(){
                 cube([70,50,23]);
               translate([2,2,0]){cube([66,46,23]);}
               translate([0,15,19]){cube([2,20,4]);}}
           }
       }           