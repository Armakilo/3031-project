# 3031-project
Repository for all things related to the ECE 3031 term project. The main things that will be put in this repository are arduino sketch files. The milestone word/excel documents should be stored on teams.

## Documentation
We will attach prefixes to data when sending or reciving data from the Elegoo.
Prefixes are 3 charachters then a colon (:).

### Sending Data
mXs: - Motor X Position  
* :fwd - Makes the position increaase  
* :rev - Makes the position increase

gxs: - Gripper x-coordinate Position  
gys: - Gripper y-coordinate Position  
gzs: - Gripper z-coordinate Position
* :inc - increase coordinate  
* :dec - decrease coordinate

cmd: - Specific Command  
* :diag - diagonal line  
* :obst - move over obstacle  

### Reciving Data
mXp: - Motor X Position  
mXv: - Motor X Velocity  
* All these just have the position/velocity in degrees/rads/sec following them

